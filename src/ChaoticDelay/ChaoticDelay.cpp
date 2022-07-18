#include "SC_PlugIn.hpp"
#include "ChaoticDelay.h"
#include "utils.h"

static InterfaceTable* ft;

namespace cts {

ChaoticDelay::ChaoticDelay() {
    mCalcFunc = make_calc_function<ChaoticDelay, &ChaoticDelay::next>();
    ringBlock = juce::dsp::AudioBlock<float>(ringContainer, 1, 0, sampleRate());
    ringBlock.clear();
    phaseBlock = juce::dsp::AudioBlock<double>(phaseContainer, 1, 0, sampleRate());
    phaseBlock.clear();
    phasor.setMax(sampleRate());
    phasor.reset();
    writer.setFeedback(0.1);
}

void ChaoticDelay::next(int nSamples) {
    float tmp[2048];
    float* const input[1] { tmp };
    auto inBlock = jusc::copyInputToBlock<float>(in(0), nSamples, input);

    float* const output[1] { out(0) };
    juce::dsp::AudioBlock<float> outBlock (output, 1, 0, nSamples);
    auto ringLength = ringBlock.getNumSamples();

    const float feedback = in0(1);
    writer.setFeedback(feedback);

    const float rate = in0(2);
    if (random.nextFloat() < rate) {
        speed = juce::jmap<double>(random.nextFloat(), 0, 1, -1, 1);
    }

    for (int i = 0; i < nSamples; ++i) {
        auto phase_value = phasor.advance(speed);
        auto sampleValue = interpolateRead(phase_value, 0, ringBlock, ringLength);
        outBlock.setSample(0, i, sampleValue);
        phaseBlock.setSample(0, i, phase_value);
    }

    writer.process(inBlock, phaseBlock, ringBlock);
}

float ChaoticDelay::interpolateRead(const double index, const int channel, juce::dsp::AudioBlock<float> block,
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

}

PluginLoad(ChaoticDelayUGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<cts::ChaoticDelay>(ft, "ChaoticDelay", false);
}
