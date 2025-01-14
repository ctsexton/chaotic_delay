#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "Delay.h"

//==============================================================================
class AudioPluginAudioProcessor : public juce::AudioProcessor {
   public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

   private:
    enum { delay, gain };
    juce::AudioProcessorValueTreeState parameters;
    juce::dsp::ProcessorChain<HecticDelay, juce::dsp::Gain<float>> chain;
    std::atomic<float>* dry_wet = nullptr;
    std::atomic<float>* gain_level = nullptr;
    std::atomic<float>* delay_feedback = nullptr;
    std::atomic<float>* delay_rate_of_change = nullptr;
    std::atomic<float>* delay_chance_of_change = nullptr;
    std::atomic<float>* delay_range_lower_bound = nullptr;
    std::atomic<float>* delay_range_upper_bound = nullptr;
    std::atomic<float>* delay_speed = nullptr;
    std::atomic<float>* delay_mode = nullptr;
    std::atomic<float>* delay_time = nullptr;
    std::atomic<double> current_sample_rate {0};
    double countdown_length = 0;
    double countdown = 0;

    juce::Random random;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
