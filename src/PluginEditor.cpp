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
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::center;

    struct SliderDetail {
        juce::Slider& slider;
        juce::Label& label;
        juce::FlexBox container;
    };

    std::vector<SliderDetail> sliderDetails{
        SliderDetail{dry_wet_slider, dry_wet_label}, SliderDetail{feedback_slider, feedback_label},
        SliderDetail{delay_range_slider, delay_range_label}, SliderDetail{delay_roc_slider, delay_roc_label},
        SliderDetail{gain_slider, gain_label}};

    for (auto& sd : sliderDetails) {
        sd.container.flexDirection = juce::FlexBox::Direction::column;
        sd.container.items.add(juce::FlexItem(sd.slider).withMinWidth(300.0f).withMinHeight(30.0f).withFlex(1));
        sd.container.items.add(juce::FlexItem(sd.label).withMinWidth(300.0f).withMinHeight(20.0f).withFlex(1));
        fb.items.add(juce::FlexItem(sd.container).withMinWidth(300.0f).withMinHeight(50.0f).withFlex(2));
    }

    fb.performLayout(getLocalBounds().toFloat());
}
