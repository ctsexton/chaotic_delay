#pragma once
#include <JuceHeader.h>


class Ring {
  public:
    Ring(juce::dsp::AudioBlock<float> block)
      : block(block) {}

    void setSample(const int channel, const int index, const float value) {
      block.setSample(channel, index, value);
    }

    float getSample(const int channel, const int index) {
      return block.getSample(channel, index);
    }

    Ring getSingleChannelBlock(const int channel) {
      return Ring(block.getSingleChannelBlock(channel));
    }

    int getNumSamples() {
      return block.getNumSamples();
    }

    float at(const double phase, const int channel) {
      return interpolateRead(wrap(phase), channel);
    }

    double wrap(double index) {
      const auto max = block.getNumSamples();
      auto value = index;
      while (value >= max) {
          value = value - max;
      }
      while (value < 0) {
          value = value + max;
      }
      return value;
    }

    float interpolateRead(const double index, const int channel) {
        double index_floor;
        auto weight_left = std::modf(index, &index_floor);
        if (weight_left < 0.01) {
            return block.getSample(channel, index);
        }
        const auto first = block.getSample(channel, index_floor);
        const auto second = block.getSample(channel, (int)(index_floor + 1) % block.getNumSamples());
        return first * weight_left + second * (1 - weight_left);
    }
  private:
    juce::dsp::AudioBlock<float> block;
};
