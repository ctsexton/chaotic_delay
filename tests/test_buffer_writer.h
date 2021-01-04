#pragma once
#include <JuceHeader.h>

#include "../src/BufferWriter.h"

class InterpBufTest : public UnitTest {
   public:
    InterpBufTest() : UnitTest("InterpBufTest", "sextone_tests") {}

    void runTest() override {
        beginTest("Mono Test");
        test_processing(&InterpBufTest::test_mono_process);
        beginTest("Mono Test Reverse");
        test_processing(&InterpBufTest::test_mono_process_reverse);
        beginTest("Test Write Line");
        test_write_line();
        beginTest("Test Write Line Reverse");
        test_write_line_reverse();
        beginTest("Test Write Direct");
        test_write_direct();
        beginTest("Test Write Direct Reverse");
        test_write_direct_reverse();
        beginTest("Test Write Other Way");
        test_write_other_way();
        beginTest("Test Write Other Way Reverse");
        test_write_other_way_reverse();
    }

    void test_write_line() {
        AudioBuffer<float> output_buffer{1, 4};
        juce::dsp::AudioBlock<float> output(output_buffer);
        InterpolatingMonoBufferWriter writer;

        auto from_value = 0.0f;
        auto chunk_size = 0.25;
        auto from_index = 0;
        auto step = 1;
        auto num_steps = 4;

        writer.writeLine(from_value, chunk_size, from_index, step, num_steps, output);
        check_block<float>(output, 0, {0.0, 0.25, 0.5, 0.75});
    }

    void test_write_line_reverse() {
        AudioBuffer<float> output_buffer{1, 4};
        juce::dsp::AudioBlock<float> output(output_buffer);
        InterpolatingMonoBufferWriter writer;

        auto from_value = 0.0f;
        auto chunk_size = 0.25;
        auto from_index = 3;
        auto step = -1;
        auto num_steps = 4;

        writer.writeLine(from_value, chunk_size, from_index, step, num_steps, output);
        check_block<float>(output, 0, {0.75, 0.5, 0.25, 0.0});
    }

    void test_write_direct() {
        AudioBuffer<float> output_buffer{1, 10};
        juce::dsp::AudioBlock<float> output(output_buffer);
        fill_block<float>(output, std::vector<float>(10, 0));
        InterpolatingMonoBufferWriter writer;

        auto from_value = 0.25;
        auto to_value = 0.5;
        auto from_index = 2;
        auto to_index = 6;

        // writes up to, but not including, the final value/index;
        writer.writeDirect(from_value, to_value, from_index, to_index, output);
        check_block<float>(output, 0, {0, 0, 0.25, 0.3125, 0.375, 0.4375, 0, 0, 0, 0});

        fill_block<float>(output, std::vector<float>(10, 0));
        writer.writeDirect(from_value, to_value, from_index, to_index, output, true);
        check_block<float>(output, 0, {0, 0, 0.25, 0.3125, 0.375, 0.4375, 0.5, 0, 0, 0});
    }

    void test_write_direct_reverse() {
        AudioBuffer<float> output_buffer{1, 10};
        juce::dsp::AudioBlock<float> output(output_buffer);
        fill_block<float>(output, std::vector<float>(10, 0));
        InterpolatingMonoBufferWriter writer;

        auto from_value = 0.25;
        auto to_value = 0.5;
        auto from_index = 6;
        auto to_index = 2;

        // writes up to, but not including, the final value/index;
        writer.writeDirect(from_value, to_value, from_index, to_index, output);
        check_block<float>(output, 0, {0, 0, 0, 0.4375, 0.375, 0.3125, 0.25, 0, 0, 0});

        fill_block<float>(output, std::vector<float>(10, 0));
        writer.writeDirect(from_value, to_value, from_index, to_index, output, true);
        check_block<float>(output, 0, {0, 0, 0.5, 0.4375, 0.375, 0.3125, 0.25, 0, 0, 0});
    }

    void test_write_other_way() {
        AudioBuffer<float> output_buffer{1, 10};
        juce::dsp::AudioBlock<float> output(output_buffer);
        fill_block<float>(output, std::vector<float>(10, 0));
        InterpolatingMonoBufferWriter writer;

        auto from_value = 0.25f;
        auto to_value = 0.625f;
        auto from_index = 2;
        auto to_index = 6;

        // writes up to, but not including, the final value/index;
        writer.writeOtherWay(from_value, to_value, from_index, to_index, output);
        check_block<float>(output, 0, {0.375, 0.3125, 0.25, 0, 0, 0, 0, 0.5625, 0.5, 0.4375});

        fill_block<float>(output, std::vector<float>(10, 0));
        writer.writeOtherWay(from_value, to_value, from_index, to_index, output, true);
        check_block<float>(output, 0, {0.375, 0.3125, 0.25, 0, 0, 0, 0.625, 0.5625, 0.5, 0.4375});
    }

    void test_write_other_way_reverse() {
        AudioBuffer<float> output_buffer{1, 10};
        juce::dsp::AudioBlock<float> output(output_buffer);
        fill_block<float>(output, std::vector<float>(10, 0));
        InterpolatingMonoBufferWriter writer;

        auto from_value = 0.625;
        auto to_value = 0.25;
        auto from_index = 6;
        auto to_index = 2;

        // writes up to, but not including, the final value/index;
        writer.writeOtherWay(from_value, to_value, from_index, to_index, output);
        check_block<float>(output, 0, {0.375, 0.3125, 0, 0, 0, 0, 0.625, 0.5625, 0.5, 0.4375});

        fill_block<float>(output, std::vector<float>(10, 0));
        writer.writeOtherWay(from_value, to_value, from_index, to_index, output, true);
        check_block<float>(output, 0, {0.375, 0.3125, 0.25, 0, 0, 0, 0.625, 0.5625, 0.5, 0.4375});
    }

    void test_mono_process(juce::dsp::AudioBlock<float> input, juce::dsp::AudioBlock<double> phase,
                           juce::dsp::AudioBlock<float> ring, InterpolatingMonoBufferWriter& writer) {
        fill_block<float>(input, {0.25, 0.5, 0.7});
        fill_block<double>(phase, {1, 5, 7});
        fill_block<float>(ring, std::vector<float>(10, 0.0));

        writer.process(input, phase, ring);

        check_block<float>(ring, 0, {0.0, 0.25, 0.3125, 0.375, 0.4375, 0.5, 0.6, 0.0, 0.0, 0.0});
    }

    void test_mono_process_reverse(juce::dsp::AudioBlock<float> input, juce::dsp::AudioBlock<double> phase,
                                   juce::dsp::AudioBlock<float> ring, InterpolatingMonoBufferWriter& writer) {
        fill_block<float>(input, {0.25, 0.5, 0.7});
        fill_block<double>(phase, {2, 8, -1});
        fill_block<float>(ring, std::vector<float>(10, 0.0));

        writer.process(input, phase, ring);

        check_block<float>(ring, 0, {0.375, 0.3125, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.4375});
    }

    void test_processing(void (InterpBufTest::*test_function)(juce::dsp::AudioBlock<float> input,
                                                              juce::dsp::AudioBlock<double> phase,
                                                              juce::dsp::AudioBlock<float> ring,
                                                              InterpolatingMonoBufferWriter& writer)) {
        AudioBuffer<float> input_buffer{1, 3};
        juce::dsp::AudioBlock<float> input(input_buffer);

        AudioBuffer<double> phase_buffer{1, 3};
        juce::dsp::AudioBlock<double> phase(phase_buffer);

        AudioBuffer<float> ring_buffer{1, 10};
        juce::dsp::AudioBlock<float> ring(ring_buffer);

        fill_block<float>(input, {0.0, 0.0, 0.0});
        fill_block<double>(phase, {0, 0, 0});
        fill_block<float>(ring, std::vector<float>(10, 0.0));

        InterpolatingMonoBufferWriter writer;

        (this->*test_function)(input, phase, ring, writer);
    }

    template <class T>
    void fill_block(juce::dsp::AudioBlock<T> block, const std::vector<T>& samples) {
        for (auto& x : samples) {
            auto i = &x - &samples[0];
            for (int ch = 0; ch < (int)block.getNumChannels(); ++ch) {
                block.setSample(ch, i, x);
            }
        }
    }

    template <class T>
    void check_block(juce::dsp::AudioBlock<T> block, int channel, const std::vector<T>& expected_values) {
        for (auto& x : expected_values) {
            auto i = &x - &expected_values[0];
            auto sample = block.getSample(channel, i);
            expectEquals(sample, x);
        }
    }
};

// Creating a static instance will automatically add the instance to the array
// returned by UnitTest::getAllTests(), so the test will be included when you call
// UnitTestRunner::runAllTests()
static InterpBufTest test;
