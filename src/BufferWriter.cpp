#include "BufferWriter.h"

void InterpolatingMonoBufferWriter::process(AudioBlock input, PhaseBlock phase, PhaseBlock offset) {
    const auto numInputSamples = input.getNumSamples();
    jassert(numInputSamples == phase.getNumSamples());
    jassert(numInputSamples == offset.getNumSamples());

    for (int i = 0; i < numInputSamples; ++i) {
        const auto phase_index = std::floor(phase.getSample(0, i));
        const auto offset_index = offset.getSample(0, i);
        const auto input_sample = input.getSample(0, i);

        if (phase_index < 0.0 && previous_phase_index >= 0.0) {
            writeNewSample(previous_phase_index, accumulated_value / accumulated_counter);
            accumulated_value = 0.0f;
            previous_phase_index = phase_index;
            continue;
        } else if (phase_index < 0.0) {
            continue;
        }

        if (previous_phase_index < 0.0) {
            previous_phase_index = phase_index;
            accumulated_counter = 0;
        }

        if (phase_index == previous_phase_index) {
            accumulated_value += input_sample;
            accumulated_counter += 1;
            continue;
        }

        const auto direct_distance = phase_index - previous_phase_index;
        const auto step = direct_distance > 0 ? 1 : -1;
        const auto abs_direct_distance = direct_distance * step;
        const auto abs_alt_distance = ring.getNumSamples() - abs_direct_distance;
        const auto go_other_way = abs_direct_distance > abs_alt_distance;
        const auto from_value = accumulated_value / accumulated_counter;
        const auto to_value = input_sample;

        if (go_other_way) {
            writeOtherWay(from_value, to_value, previous_phase_index, phase_index, false, offset_index);
        } else {
            writeDirect(from_value, to_value, previous_phase_index, phase_index, false, offset_index);
        }

        accumulated_value = input_sample;
        accumulated_counter = 1;
        previous_phase_index = phase_index;
    }
}

void InterpolatingMonoBufferWriter::writeDirect(const float from_value, const float to_value, const float from_index,
                                                const float to_index, const bool end_inclusive, const double feedback_offset) {
    const auto direct_distance = to_index - from_index;
    const auto step = direct_distance > 0 ? 1 : -1;
    auto num_steps = direct_distance * step;
    const auto chunk_size = (to_value - from_value) / num_steps;

    if (end_inclusive) {
        num_steps += 1;
    }

    writeLine(from_value, chunk_size, from_index, step, num_steps, feedback_offset);
}

void InterpolatingMonoBufferWriter::writeOtherWay(const float from_value, const float to_value, const float from_index,
                                                  const float to_index,
                                                  const bool end_inclusive, const double feedback_offset) {
    const auto ringbuffer_length = ring.getNumSamples();
    const auto direct_distance = to_index - from_index;
    const auto direction = direct_distance > 0;
    const auto step = direction ? -1 : 1;  // Step in the opposite direction to the direct route
    const auto first_segment_steps = direction ? from_index + 1 : ringbuffer_length - from_index;
    auto second_segment_steps = direction ? ringbuffer_length - to_index - 1 : to_index;
    const auto abs_distance = first_segment_steps + second_segment_steps;
    const auto chunk_size = (to_value - from_value) / abs_distance;
    const auto second_from_index = direction ? ringbuffer_length - 1 : 0;
    const auto second_from_value = chunk_size * first_segment_steps + from_value;
    if (end_inclusive) {
        second_segment_steps += 1;
    }

    writeLine(from_value, chunk_size, from_index, step, first_segment_steps, feedback_offset);
    writeLine(second_from_value, chunk_size, second_from_index, step, second_segment_steps, feedback_offset);
}

void InterpolatingMonoBufferWriter::writeLine(const float from_value, const float chunk_size, const int from_index,
                                              const int step, const int num_steps, const double feedback_offset) {
    auto write_index = from_index;
    auto write_value = from_value;
    auto chunks_remaining = num_steps;
    while (chunks_remaining > 0) {
        writeNewSample(write_index, write_value, feedback_offset);
        write_index = write_index + step;
        write_value += chunk_size;
        --chunks_remaining;
    }
}

void InterpolatingMonoBufferWriter::writeNewSample(const int write_index, const float write_value,
                                                   const double feedback_offset) {
    const auto feedback_sample = ring.at(write_index + feedback_offset, 0) * feedback;
    const auto sample = write_value + feedback_sample;
    ring.setSample(0, write_index, sample);
}

void InterpolatingMonoBufferWriter::setFeedback(float value) {
    Range<float> range{0.0f, 0.95f};
    feedback = range.clipValue(value);
}
