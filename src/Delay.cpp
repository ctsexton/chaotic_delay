#include "Delay.h"

HecticDelay::HecticDelay() : 
  drywet(4096),
  offset(0.0)
{
    drywet.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
    feedback.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
}

void HecticDelay::prepare(const juce::dsp::ProcessSpec& spec) {
    temp_buffer = std::make_unique<juce::AudioBuffer<float>>(2, spec.maximumBlockSize);
    temp_buffer->clear();
    temp = juce::dsp::AudioBlock<float>(*temp_buffer);

    ring_buffer = std::make_unique<juce::AudioBuffer<float>>(2, spec.sampleRate * 2);
    ring_buffer->clear();
    ring_block = juce::dsp::AudioBlock<float>(*ring_buffer);
    ring = std::make_unique<Ring>(ring_block);
    buffer_writer = std::make_unique<InterpolatingStereoBufferWriter>(*ring);

    phase_buffer = std::make_unique<juce::AudioBuffer<double>>(1, spec.maximumBlockSize);
    phase_buffer->clear();
    phase_block = juce::dsp::AudioBlock<double>(*phase_buffer);

    offset_buffer = std::make_unique<juce::AudioBuffer<double>>(1, spec.maximumBlockSize);
    offset_buffer->clear();
    offset_block = juce::dsp::AudioBlock<double>(*offset_buffer);

    drywet.prepare(spec);
    drywet.setWetLatency(0);
    feedback.prepare(spec);
    feedback.setWetLatency(0);

    offset.reset(spec.sampleRate, 0.25);

    const auto ring_samples = ring_block.getNumSamples();
    phasor.setMax(ring_samples);
    phasor.reset();
}

void HecticDelay::setDryWet(float value) { drywet.setWetMixProportion(value); }

void HecticDelay::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    auto audio = context.getOutputBlock();
    drywet.pushDrySamples(audio);

    for (int i = 0; i < audio.getNumSamples(); ++i) {
        phase_block.setSample(0, i, phasor.advance(speed));
        offset_block.setSample(0, i, offset.getNextValue());
    }

    buffer_writer->process(audio, phase_block, offset_block);

    temp.fill(0);
    for (int i = 0; i < audio.getNumSamples(); ++i) {
        const auto phase = phase_block.getSample(0, i) + offset_block.getSample(0, i);
        audio.setSample(0, i, ring->at(phase, 0));
        audio.setSample(1, i, ring->at(phase, 1));
    }
    drywet.mixWetSamples(audio);
}

void HecticDelay::setFrequency(double value) { chance_of_change = juce::jmap<double>(value, 0.0, 1.0, 0.001, 0.25); }

void HecticDelay::setFeedback(float value) { 
  feedback.setWetMixProportion(value * 0.5);
  buffer_writer->setFeedback(value);
}

void HecticDelay::setSpeed(float value) {
    Range<double> range{-2.0, 2.0};
    speed = range.clipValue(value);
}

void HecticDelay::setDelayTime(float value) {
  const auto ring_samples = ring_block.getNumSamples();
  Range<double> range{0.0, 1.0};
  offset.setTargetValue((1 - range.clipValue(value)) * ring_samples);
}

void Phasor::setMax(double value) { max = value; }

double Phasor::advance(double increment) {
    phase = wrap(phase + increment);
    return phase;
}

double Phasor::wrap(double input) {
    auto value = input;
    while (value >= max) {
        value = value - max;
    }
    while (value < 0) {
        value = value + max;
    }
    return value;
}
