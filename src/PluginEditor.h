#pragma once

#include "PluginProcessor.h"
#include "gui/TwoValueSliderAttachment.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
   public:
    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

   private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;
    juce::AudioProcessorValueTreeState& valueTreeState;

    juce::Slider dry_wet_slider;
    juce::Slider gain_slider;
    juce::Slider feedback_slider;
    juce::Slider delay_range_slider;
    juce::Slider delay_roc_slider;

    juce::AudioProcessorValueTreeState::SliderAttachment dry_wet_attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment gain_attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment feedback_attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment delay_roc_attachment;
    TwoValueSliderAttachment delay_range_attachment;

    juce::Label dry_wet_label;
    juce::Label gain_label;
    juce::Label feedback_label;
    juce::Label delay_range_label;
    juce::Label delay_roc_label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
