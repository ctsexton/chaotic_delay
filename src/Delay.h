#include "BufferWriter.h"

class Phasor {
   public:
    Phasor() {}

    double advance(double increment);
    double wrap(double input);
    void setMax(double value);
    void reset() { phase = 0; }

   private:
    double phase = 0;
    double max = 0;
};

class HecticDelay : public juce::dsp::ProcessorBase {
   public:
    HecticDelay();
    void prepare(const juce::dsp::ProcessSpec&) override;
    void process(const juce::dsp::ProcessContextReplacing<float>&) override;
    void reset() override{};
    void setFrequency(double value);
    void setFeedback(float value);
    void setSpeed(float value);
    void setDryWet(float value);
    void setDelayTime(float value);

   private:
    InterpolatingStereoBufferWriter buffer_writer;
    Phasor phasor;
    juce::dsp::DryWetMixer<float> drywet;
    juce::dsp::DryWetMixer<float> feedback;
    double speed;
    double offset;
    double chance_of_change = 0.05;
    juce::Random random;

    float interpolateRead(const double index, const int channel, juce::dsp::AudioBlock<float> block,
                          const int block_length);

    std::unique_ptr<juce::AudioBuffer<float>> temp_buffer;
    std::unique_ptr<juce::AudioBuffer<float>> ring_buffer;
    std::unique_ptr<juce::AudioBuffer<double>> phase_buffer;
    juce::dsp::AudioBlock<float> temp;
    juce::dsp::AudioBlock<float> ring_block;
    juce::dsp::AudioBlock<double> phase_block;
};
