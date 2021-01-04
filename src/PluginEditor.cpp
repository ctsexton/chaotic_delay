#include "PluginEditor.h"

#include "PluginProcessor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p,
                                                                 juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p),
      processorRef(p),
      valueTreeState(vts),
      dry_wet_slider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox),
      gain_slider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox),
      feedback_slider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox),
      delay_range_slider(juce::Slider::TwoValueHorizontal, juce::Slider::NoTextBox),
      delay_roc_slider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox),
      dry_wet_attachment(vts, "dry_wet", dry_wet_slider),
      gain_attachment(vts, "gain", gain_slider),
      feedback_attachment(vts, "delay_feedback", gain_slider),
      delay_roc_attachment(vts, "delay_rate_of_change", delay_roc_slider),
      delay_range_attachment(vts, "delay_range_lower_bound", "delay_range_upper_bound", delay_range_slider) {
    dry_wet_label.setText("Dry/Wet", juce::dontSendNotification);
    dry_wet_label.setJustificationType(juce::Justification::centred);
    gain_label.setText("Gain", juce::dontSendNotification);
    gain_label.setJustificationType(juce::Justification::centred);
    feedback_label.setText("Feedback", juce::dontSendNotification);
    feedback_label.setJustificationType(juce::Justification::centred);
    delay_range_label.setText("Speed Range", juce::dontSendNotification);
    delay_range_label.setJustificationType(juce::Justification::centred);
    delay_roc_label.setText("Rate of Change", juce::dontSendNotification);
    delay_roc_label.setJustificationType(juce::Justification::centred);

    delay_roc_slider.setSkewFactor(0.25);

    addAndMakeVisible(dry_wet_slider);
    addAndMakeVisible(gain_slider);
    addAndMakeVisible(feedback_slider);
    addAndMakeVisible(delay_range_slider);
    addAndMakeVisible(delay_roc_slider);
    addAndMakeVisible(dry_wet_label);
    addAndMakeVisible(gain_label);
    addAndMakeVisible(feedback_label);
    addAndMakeVisible(delay_range_label);
    addAndMakeVisible(delay_roc_label);
    setResizable(true, true);
    setSize(1000, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void AudioPluginAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    auto labelArea = area.removeFromTop(area.getHeight() / 2);
    auto sliderWidth = area.getWidth() / 5;
    auto sliderHeight = 30;
    auto labelHeight = 20;
    auto labelWidth = sliderWidth;

    dry_wet_slider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight));
    dry_wet_label.setBounds(labelArea.removeFromLeft(labelWidth).removeFromBottom(labelHeight));

    gain_slider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight));
    gain_label.setBounds(labelArea.removeFromLeft(labelWidth).removeFromBottom(labelHeight));

    feedback_slider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight));
    feedback_label.setBounds(labelArea.removeFromLeft(labelWidth).removeFromBottom(labelHeight));

    delay_range_slider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight));
    delay_range_label.setBounds(labelArea.removeFromLeft(labelWidth).removeFromBottom(labelHeight));

    delay_roc_slider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight));
    delay_roc_label.setBounds(labelArea.removeFromLeft(labelWidth).removeFromBottom(labelHeight));
}
