
#pragma once

#include "SC_PlugIn.hpp"
#include <juce_dsp/juce_dsp.h>
#include "Phasor.h"
#include "BufferWriter.h"

namespace cts {

class ChaoticDelay : public SCUnit {
public:
    ChaoticDelay();

private:
    void next(int nSamples);
    float interpolateRead(const double index, const int channel, juce::dsp::AudioBlock<float> block, const int block_length);
    InterpolatingMonoBufferWriter writer;
    Phasor phasor;

    float ring[192000];
    float* const ringContainer[1] { ring };
    juce::dsp::AudioBlock<float> ringBlock;

    double phase[192000];
    double* const phaseContainer[1] { phase };
    juce::dsp::AudioBlock<double> phaseBlock;

    juce::Random random;
    double speed = 1.0;
};
}
