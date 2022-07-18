#pragma once

namespace jusc {
  // Utility functions to help JUCE play nice with SuperCollider
  template <class SampleType>
  juce::dsp::AudioBlock<SampleType> copyInputToBlock(const SampleType* input, int nSamples, SampleType* const channels[]) {
    juce::dsp::AudioBlock<SampleType> block (channels, 1, 0, nSamples);
    for (int i = 0; i < nSamples; ++i) {
      block.setSample(0, i, input[i]);
    }
    return block;
  }
}
