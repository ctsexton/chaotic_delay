#include "PluginEditor.h"
#include <math.h>

#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p,
                                                                 juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p),
      processorRef(p),
      valueTreeState(vts),
      speed_component(vts),
      knobLookAndFeel(
          juce::Colours::grey,
          juce::Colours::grey,
          juce::Colours::white,
          bottomBackgroundColour
      ),
      dry_wet_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      gain_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      feedback_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      time_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      dry_wet_attachment(vts, "dry_wet", dry_wet_slider),
      gain_attachment(vts, "gain", gain_slider),
      feedback_attachment(vts, "delay_feedback", feedback_slider)
{
    dry_wet_label.setText("Dry/Wet", juce::dontSendNotification);
    dry_wet_label.setJustificationType(juce::Justification::centred);
    gain_label.setText("Gain", juce::dontSendNotification);
    gain_label.setJustificationType(juce::Justification::centred);
    feedback_label.setText("Feedback", juce::dontSendNotification);
    feedback_label.setJustificationType(juce::Justification::centred);
    time_label.setText("Time", juce::dontSendNotification);
    time_label.setJustificationType(juce::Justification::centred);

    feedback_slider.setLookAndFeel(&knobLookAndFeel);
    dry_wet_slider.setLookAndFeel(&knobLookAndFeel);
    gain_slider.setLookAndFeel(&knobLookAndFeel);
    time_slider.setLookAndFeel(&knobLookAndFeel);
    feedback_label.setColour(juce::Label::backgroundColourId, bottomBackgroundColour);
    dry_wet_label.setColour(juce::Label::backgroundColourId, bottomBackgroundColour);
    gain_label.setColour(juce::Label::backgroundColourId, bottomBackgroundColour);
    time_label.setColour(juce::Label::backgroundColourId, bottomBackgroundColour);

    addAndMakeVisible(speed_component);
    addAndMakeVisible(dry_wet_slider);
    addAndMakeVisible(gain_slider);
    addAndMakeVisible(feedback_slider);
    addAndMakeVisible(time_slider);
    addAndMakeVisible(dry_wet_label);
    addAndMakeVisible(gain_label);
    addAndMakeVisible(feedback_label);
    addAndMakeVisible(time_label);
    setResizable(true, true);
    setSize(600, 500);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
    g.fillAll(juce::Colours::white);
}

void AudioPluginAudioProcessorEditor::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Item = juce::GridItem;

    grid.templateRows = { Track(1_fr), Track(100_px), Track(20_px) };
    grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr) };

    grid.items.addArray({
        Item(speed_component).withArea(1, 1, 2, 5),
        Item(time_slider).withArea(2, 1, 3, 2),
        Item(feedback_slider).withArea(2, 2, 3, 3),
        Item(dry_wet_slider).withArea(2, 3, 3, 4),
        Item(gain_slider).withArea(2, 4, 3, 5),
        Item(time_label).withArea(3, 1, 4, 2),
        Item(feedback_label).withArea(3, 2, 4, 3),
        Item(dry_wet_label).withArea(3, 3, 4, 4),
        Item(gain_label).withArea(3, 4, 4, 5),
    });

    grid.performLayout(getLocalBounds());
}
