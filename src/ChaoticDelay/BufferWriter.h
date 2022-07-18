#pragma once
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

class InterpolatingMonoBufferWriter {
   public:
    typedef juce::dsp::AudioBlock<float> AudioBlock;
    typedef juce::dsp::AudioBlock<double> PhaseBlock;
    InterpolatingMonoBufferWriter() {}

    void process(AudioBlock input, PhaseBlock phase, AudioBlock ringbuffer);
    void writeDirect(const float from_value, const float to_value, const float from_index, const float to_index,
                     AudioBlock ringbuffer, const bool end_inclusive = false);
    void writeOtherWay(const float from_value, const float to_value, const float from_index, const float to_index,
                       AudioBlock ringbuffer, const bool end_inclusive = false);
    void writeLine(const float from_value, const float chunk_size, const int from_index, const int step,
                   const int num_steps, AudioBlock ringbuffer);
    void writeNewSample(const int write_index, const float write_value, AudioBlock ringbuffer);
    void setFeedback(float value);

   private:
    double previous_phase_index = -1.0;
    float accumulated_value = 0.0f;
    int accumulated_counter = 0;
    float feedback = 0.0f;
};

class InterpolatingStereoBufferWriter {
   public:
    typedef juce::dsp::AudioBlock<float> AudioBlock;
    typedef juce::dsp::AudioBlock<double> PhaseBlock;
    InterpolatingStereoBufferWriter() : channelWriters(2) {}

    void process(AudioBlock input, PhaseBlock phase, AudioBlock ringbuffer) {
        for (int i = 0; i < 2; ++i) {
            auto subRing = ringbuffer.getSingleChannelBlock(i);
            auto subInput = input.getSingleChannelBlock(i);
            auto& writer = channelWriters.at(i);
            writer.process(subInput, phase, subRing);
        }
    };

    void setFeedback(float value) {
        channelWriters[0].setFeedback(value);
        channelWriters[1].setFeedback(value);
    }

   private:
    std::vector<InterpolatingMonoBufferWriter> channelWriters;
};
