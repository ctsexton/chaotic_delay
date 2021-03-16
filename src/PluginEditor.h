#pragma once

#include "PluginProcessor.h"
#include "gui/SpeedComponent.h"
#include "gui/knob.h"
#include "gui/font.h"

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
    SpeedComponent speed_component;

    juce::Colour bottomBackgroundColour { 20, 20, 30 };
    KnobLookAndFeel knobLookAndFeel;
    KnobLookAndFeel timeKnobLF {
          juce::Colours::grey,
          juce::Colour(237, 174, 73),
          juce::Colours::black,
          bottomBackgroundColour,
          juce::Colour(80, 80, 80)
    };
    KnobLookAndFeel feedbackKnobLF {
          juce::Colours::grey,
          juce::Colour(209, 73, 91),
          juce::Colours::black,
          bottomBackgroundColour,
          juce::Colour(80, 80, 80)
    };
    KnobLookAndFeel drywetKnobLF {
          juce::Colours::grey,
          juce::Colour(30, 255, 188),
          juce::Colours::black,
          bottomBackgroundColour,
          juce::Colour(80, 80, 80)
    };
    KnobLookAndFeel gainKnobLF {
          juce::Colours::grey,
          juce::Colour(48, 150, 255),
          juce::Colours::black,
          bottomBackgroundColour,
          juce::Colour(80, 80, 80)
    };

    juce::Slider dry_wet_slider;
    juce::Slider gain_slider;
    juce::Slider feedback_slider;
    juce::Slider time_slider;

    juce::AudioProcessorValueTreeState::SliderAttachment dry_wet_attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment gain_attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment feedback_attachment;
    juce::AudioProcessorValueTreeState::SliderAttachment time_attachment;

    juce::Label dry_wet_label;
    juce::Label gain_label;
    juce::Label feedback_label;
    juce::Label time_label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
