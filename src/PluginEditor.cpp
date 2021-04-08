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
          juce::Colours::white,
          juce::Colours::black,
          bottomBackgroundColour,
          juce::Colour(80, 80, 80)
      ),
      dry_wet_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      gain_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      feedback_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      time_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      dry_wet_attachment(vts, "dry_wet", dry_wet_slider),
      gain_attachment(vts, "gain", gain_slider),
      feedback_attachment(vts, "delay_feedback", feedback_slider),
      time_attachment(vts, "delay_time", time_slider)
{
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Source Code Pro");

    std::vector<juce::Label*> labels { &dry_wet_label, &gain_label, &feedback_label, &time_label };
    for (auto& label : labels) {
      label->setJustificationType(juce::Justification::centred);
      label->setFont(20);
      addAndMakeVisible(*label);
    }
    std::vector<juce::Slider*> sliders { &dry_wet_slider, &gain_slider, &feedback_slider, &time_slider };
    for (auto& slider : sliders) {
      addAndMakeVisible(*slider);
    }
    time_slider.setLookAndFeel(&timeKnobLF);
    feedback_slider.setLookAndFeel(&feedbackKnobLF);
    dry_wet_slider.setLookAndFeel(&drywetKnobLF);
    gain_slider.setLookAndFeel(&gainKnobLF);

    dry_wet_label.setText("dry/wet", juce::dontSendNotification);
    gain_label.setText("gain", juce::dontSendNotification);
    feedback_label.setText("feedback", juce::dontSendNotification);
    time_label.setText("time", juce::dontSendNotification);

    addAndMakeVisible(speed_component);
    setResizable(true, true);
    setSize(600, 500);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
    g.fillAll(bottomBackgroundColour);
}

void AudioPluginAudioProcessorEditor::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Item = juce::GridItem;

    grid.templateRows = { Track(1_fr), Track(10_px), Track(100_px), Track(30_px) };
    grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr) };

    auto tabStartY = 1;
    auto tabEndY = 2;
    auto slidersStartY = 3;
    auto slidersEndY = 4;
    auto labelsStartY = 4;
    auto labelsEndY = 5;

    grid.items.addArray({
        Item(speed_component).withArea(tabStartY, 1, tabEndY, 5),
        Item(time_slider).withArea(slidersStartY, 1, slidersEndY, 2),
        Item(feedback_slider).withArea(slidersStartY, 2, slidersEndY, 3),
        Item(dry_wet_slider).withArea(slidersStartY, 3, slidersEndY, 4),
        Item(gain_slider).withArea(slidersStartY, 4, slidersEndY, 5),
        Item(time_label).withArea(labelsStartY, 1, labelsEndY, 2),
        Item(feedback_label).withArea(labelsStartY, 2, labelsEndY, 3),
        Item(dry_wet_label).withArea(labelsStartY, 3, labelsEndY, 4),
        Item(gain_label).withArea(labelsStartY, 4, labelsEndY, 5),
    });

    grid.performLayout(getLocalBounds());
}
