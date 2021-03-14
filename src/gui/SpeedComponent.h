#pragma once
#include <JuceHeader.h>
#include "TwoValueSliderAttachment.h"

class RandomSpeedComponent : public juce::Component {
  public:
    RandomSpeedComponent(juce::AudioProcessorValueTreeState& vts)
      :
      delay_range_slider(juce::Slider::TwoValueHorizontal, juce::Slider::NoTextBox),
      delay_roc_slider(juce::Slider::LinearHorizontal, juce::Slider::TextBoxBelow),
      delay_roc_attachment(vts, "delay_rate_of_change", delay_roc_slider),
      delay_range_attachment(vts, "delay_range_lower_bound", "delay_range_upper_bound", delay_range_slider)
    {
      delay_range_label.setText("Speed Range", juce::dontSendNotification);
      delay_range_label.setJustificationType(juce::Justification::centred);
      delay_roc_label.setText("Rate of Change", juce::dontSendNotification);
      delay_roc_label.setJustificationType(juce::Justification::centred);

      addAndMakeVisible(delay_range_slider);
      addAndMakeVisible(delay_roc_slider);
    };
    void resized() override {
      juce::FlexBox fb;
      fb.flexWrap = juce::FlexBox::Wrap::wrap;
      fb.justifyContent = juce::FlexBox::JustifyContent::center;
      fb.alignContent = juce::FlexBox::AlignContent::center;
      fb.items.add(juce::FlexItem(delay_roc_slider).withMinWidth(300.0f).withMinHeight(50.0f).withFlex(2));
      fb.items.add(juce::FlexItem(delay_range_slider).withMinWidth(300.0f).withMinHeight(50.0f).withFlex(2));
      fb.performLayout(getLocalBounds().toFloat());
    };
  private:
    juce::Slider delay_range_slider;
    juce::Slider delay_roc_slider;
    juce::AudioProcessorValueTreeState::SliderAttachment delay_roc_attachment;
    TwoValueSliderAttachment delay_range_attachment;
    juce::Label delay_range_label;
    juce::Label delay_roc_label;
};


class SpeedComponent : public juce::Component {
  public:
    SpeedComponent(juce::AudioProcessorValueTreeState& vts) 
      : 
      tabs(juce::TabbedButtonBar::Orientation::TabsAtTop),
      random_speed_component(vts),
      delay_speed_slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox),
      delay_speed_attachment(vts, "delay_speed", delay_speed_slider),
      delay_mode_attachment(*vts.getParameter("delay_mode"), [&](float value) {
        auto* mode = static_cast<juce::AudioParameterChoice*>(vts.getParameter("delay_mode"));
        auto values = mode->getAllValueStrings();
        auto current = mode->getCurrentValueAsText();
        auto index = values.indexOf(current);
        tabs.setCurrentTabIndex(index);
      })
    {
      delay_speed_label.setText("Speed", juce::dontSendNotification);
      delay_speed_label.setJustificationType(juce::Justification::centred);

      tabs.addTab(
        "Manual",
        juce::Colour(51, 69, 84),
        &delay_speed_slider,
        false
      );
      tabs.addTab(
        "S&H",
        juce::Colour(89, 79, 79),
        &random_speed_component,
        false
      );
      tabs.setTabBarDepth(40);
      tabs.setOutline(0);
      tabs.setLookAndFeel(&tabLookAndFeel);
      delay_mode_attachment.sendInitialUpdate();
      addAndMakeVisible(tabs);
    };

    void resized() override {
      tabs.setBounds(getLocalBounds());
    };

  private:
    class TabLookAndFeel : public juce::LookAndFeel_V4
    {
      public:
        TabLookAndFeel() {};

        int getTabButtonBestWidth(juce::TabBarButton& button, int tabDepth) override {
          return button.getTabbedButtonBar().getWidth() / button.getTabbedButtonBar().getNumTabs();
        };
    } tabLookAndFeel;
    juce::TabbedComponent tabs;
    juce::Slider delay_speed_slider;
    RandomSpeedComponent random_speed_component;

    juce::AudioProcessorValueTreeState::SliderAttachment delay_speed_attachment;
    juce::ParameterAttachment delay_mode_attachment;

    juce::Label delay_speed_label;
};
