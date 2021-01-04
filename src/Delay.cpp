#include "Delay.h"

HecticDelay::HecticDelay() : drywet(4096) {
    // drywet.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
}

void HecticDelay::prepare(const juce::dsp::ProcessSpec& spec) {
    temp_buffer = std::make_unique<juce::AudioBuffer<float>>(2, spec.maximumBlockSize);
    temp_buffer->clear();
    temp = juce::dsp::AudioBlock<float>(*temp_buffer);

    ring_buffer = std::make_unique<juce::AudioBuffer<float>>(2, spec.sampleRate * 2);
    ring_buffer->clear();
    ring_block = juce::dsp::AudioBlock<float>(*ring_buffer);

    phase_buffer = std::make_unique<juce::AudioBuffer<double>>(1, spec.maximumBlockSize);
    phase_buffer->clear();
    phase_block = juce::dsp::AudioBlock<double>(*phase_buffer);

    drywet.prepare(spec);
    drywet.setWetLatency(0);

    const auto ring_samples = ring_block.getNumSamples();
    phasor.setMax(ring_samples);
    phasor.reset();
}

void HecticDelay::setDryWet(float value) { drywet.setWetMixProportion(value); }

void HecticDelay::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    auto audio = context.getOutputBlock();
    temp.copyFrom(audio);
    auto* inputSamples = audio.getChannelPointer(0);
    drywet.pushDrySamples(audio);

    const auto numSamples = audio.getNumSamples();
    const auto ringbuffer_length = ring_block.getNumSamples();

    for (int i = 0; i < numSamples; ++i) {
        auto phase_value = phasor.advance(speed);
        auto out_left = interpolateRead(phase_value, 0, ring_block, ringbuffer_length);
        auto out_right = interpolateRead(phase_value, 1, ring_block, ringbuffer_length);
        audio.setSample(0, i, out_left);
        audio.setSample(1, i, out_right);
        phase_block.setSample(0, i, phase_value);
    }
    drywet.mixWetSamples(audio);

    buffer_writer.process(temp, phase_block, ring_block);
    temp.fill(0);
}

float HecticDelay::interpolateRead(const double index, const int channel, juce::dsp::AudioBlock<float> block,
                                   const int block_length) {
    double index_floor;
    const auto weight_left = std::modf(index, &index_floor);
    if (weight_left == 0) {
        return block.getSample(channel, index);
    }
    const auto first = block.getSample(channel, index_floor);
    const auto second = block.getSample(channel, (int)(index_floor + 1) % block_length);
    return first * weight_left + second * (1 - weight_left);
}

void HecticDelay::setFrequency(double value) { chance_of_change = juce::jmap<double>(value, 0.0, 1.0, 0.001, 0.25); }

void HecticDelay::setFeedback(float value) { buffer_writer.setFeedback(value); }

void HecticDelay::setSpeed(float value) {
    Range<double> range{-2.0, 2.0};
    speed = range.clipValue(value);
}

void Phasor::setMax(double value) { max = value; }

double Phasor::advance(double increment) {
    auto new_phase = phase + increment;
    while (new_phase >= max) {
        new_phase = new_phase - max;
    }
    while (new_phase < 0) {
        new_phase = new_phase + max;
    }
    phase = new_phase;
    return phase;
}