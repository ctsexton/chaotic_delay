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
      delay_mode_button("Delay Mode"),
      dry_wet_slider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox),
      gain_slider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox),
      feedback_slider(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox),
      dry_wet_attachment(vts, "dry_wet", dry_wet_slider),
      gain_attachment(vts, "gain", gain_slider),
      feedback_attachment(vts, "delay_feedback", feedback_slider),
      delay_mode_attachment(*vts.getParameter("delay_mode"), [&](float value) {
        auto* mode = static_cast<juce::AudioParameterChoice*>(vts.getParameter("delay_mode"));
        delay_mode_label.setText(mode->getCurrentChoiceName(), juce::dontSendNotification);
      })
{
    dry_wet_label.setText("Dry/Wet", juce::dontSendNotification);
    dry_wet_label.setJustificationType(juce::Justification::centred);
    gain_label.setText("Gain", juce::dontSendNotification);
    gain_label.setJustificationType(juce::Justification::centred);
    feedback_label.setText("Feedback", juce::dontSendNotification);
    feedback_label.setJustificationType(juce::Justification::centred);

    delay_mode_attachment.sendInitialUpdate();
    delay_mode_button.onClick = [&]() {
      juce::AudioProcessorParameterWithID* mode = vts.getParameter("delay_mode");
      auto values = mode->getAllValueStrings();
      auto current = mode->getCurrentValueAsText();
      auto index = values.indexOf(current);
      auto length = mode->getNumSteps();
      auto nextIndex = (index + 1) % length;
      auto nextText = values[nextIndex];
      auto newValue = mode->getValueForText(nextText);
      mode->beginChangeGesture();
      mode->setValueNotifyingHost(newValue);
      mode->endChangeGesture();
    };

    addAndMakeVisible(speed_component);
    addAndMakeVisible(delay_mode_button);
    addAndMakeVisible(delay_mode_label);
    addAndMakeVisible(dry_wet_slider);
    addAndMakeVisible(gain_slider);
    addAndMakeVisible(feedback_slider);
    addAndMakeVisible(dry_wet_label);
    addAndMakeVisible(gain_label);
    addAndMakeVisible(feedback_label);
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
        SliderDetail{dry_wet_slider, dry_wet_label},
        SliderDetail{feedback_slider, feedback_label},
        SliderDetail{gain_slider, gain_label}
    };

    for (auto& sd : sliderDetails) {
        sd.container.flexDirection = juce::FlexBox::Direction::column;
        sd.container.items.add(juce::FlexItem(sd.slider).withMinWidth(300.0f).withMinHeight(30.0f).withFlex(1));
        sd.container.items.add(juce::FlexItem(sd.label).withMinWidth(300.0f).withMinHeight(20.0f).withFlex(1));
        fb.items.add(juce::FlexItem(sd.container).withMinWidth(300.0f).withMinHeight(50.0f).withFlex(2));
    }

    juce::FlexBox mode_container;
    mode_container.items.add(juce::FlexItem(delay_mode_button).withMinWidth(300.0f).withMinHeight(30.0f).withFlex(1));
    mode_container.items.add(juce::FlexItem(delay_mode_label).withMinWidth(300.0f).withMinHeight(30.0f).withFlex(1));

    fb.items.add(juce::FlexItem(mode_container).withMinWidth(300.0f).withMinHeight(50.0f).withFlex(2));
    fb.items.add(juce::FlexItem(speed_component).withMinWidth(300.0f).withMinHeight(200.0f).withFlex(2));
    fb.performLayout(getLocalBounds().toFloat());
}
