#pragma once
#include <JuceHeader.h>
#include "Ring.h"

class InterpolatingMonoBufferWriter {
   public:
    typedef juce::dsp::AudioBlock<float> AudioBlock;
    typedef juce::dsp::AudioBlock<double> PhaseBlock;
    InterpolatingMonoBufferWriter(Ring ring) : ring(ring) {}

    void process(AudioBlock input, PhaseBlock phase, PhaseBlock offset);
    void writeDirect(const float from_value, const float to_value, const float from_index, const float to_index,
                     const bool end_inclusive = false, const double feedback_offset = 0.0);
    void writeOtherWay(const float from_value, const float to_value, const float from_index, const float to_index,
                       const bool end_inclusive = false, const double feedback_offset = 0.0);
    void writeLine(const float from_value, const float chunk_size, const int from_index, const int step,
                   const int num_steps, const double feedback_offset = 0.0);
    void writeNewSample(const int write_index, const float write_value, const double feedback_offset = 0.0);
    void setFeedback(float value);

   private:
    double previous_phase_index = -1.0;
    float accumulated_value = 0.0f;
    int accumulated_counter = 0;
    float feedback = 0.0f;
    Ring ring;
};

class InterpolatingStereoBufferWriter {
   public:
    typedef juce::dsp::AudioBlock<float> AudioBlock;
    typedef juce::dsp::AudioBlock<double> PhaseBlock;
    InterpolatingStereoBufferWriter(Ring ring) {
        for (int i = 0; i < 2; ++i) {
            auto channel_ring = ring.getSingleChannelBlock(i);
            channel_writers.emplace_back(channel_ring);
        }
    }

    void process(AudioBlock input, PhaseBlock phase, PhaseBlock offset) {
        for (int i = 0; i < 2; ++i) {
            auto subInput = input.getSingleChannelBlock(i);
            auto& writer = channel_writers.at(i);
            writer.process(subInput, phase, offset);
        }
    }

    void setFeedback(float value) {
        channel_writers.at(0).setFeedback(value);
        channel_writers.at(1).setFeedback(value);
    }

   private:
    std::vector<InterpolatingMonoBufferWriter> channel_writers;
};
