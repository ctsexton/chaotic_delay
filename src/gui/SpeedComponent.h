#pragma once
#include <JuceHeader.h>
#include "TwoValueSliderAttachment.h"
#include "knob.h"

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
      random_speed_component(vts),
      knobLookAndFeel(
          juce::Colours::grey,
          juce::Colours::black,
          juce::Colours::white,
          juce::Colour(),
          juce::Colour(200, 200, 200)
      ),
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

      tabs.TabChangedFunc = [&](int newIndex, const juce::String& newName) {
        juce::AudioProcessorParameterWithID* mode = vts.getParameter("delay_mode");
        auto newValue = mode->getValueForText(newName);
        mode->beginChangeGesture();
        mode->setValueNotifyingHost(newValue);
        mode->endChangeGesture();
      };

      delay_speed_slider.setLookAndFeel(&knobLookAndFeel);
      auto* mode = static_cast<juce::AudioParameterChoice*>(vts.getParameter("delay_mode"));
      auto modes = mode->getAllValueStrings();
      tabs.addTab(
        modes[0],
        juce::Colour(87, 76, 77),
        &random_speed_component,
        false
      );
      tabs.addTab(
        modes[1],
        juce::Colours::white,
        &delay_speed_slider,
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
        TabLookAndFeel() { };

        int getTabButtonOverlap (int tabDepth)
        {
            return 0;
        }

        int getTabButtonSpaceAroundImage()
        {
            return 0;
        }

        void createTabButtonShape (juce::TabBarButton& button, juce::Path& p, bool /*isMouseOver*/, bool /*isMouseDown*/)
        {
            const bool isFrontTab = button.isFrontTab();

            auto activeArea = button.getActiveArea();
            auto w = (float) activeArea.getWidth();
            auto h = (float) activeArea.getHeight();

            auto length = w;
            auto depth = h;

            if (button.getTabbedButtonBar().isVertical())
                std::swap (length, depth);

            const float indent = (float) getTabButtonOverlap ((int) depth);
            const float overhang = 0.0f;

            p.startNewSubPath (0.0f, 0.0f);
            p.lineTo (indent, h);
            p.lineTo (w - indent, h);
            p.lineTo (w, 0.0f);
            p.lineTo (w + overhang, -overhang);
            p.lineTo (-overhang, -overhang);
            p.closeSubPath();
        };

        void createTabButtonOutline (juce::TabBarButton& button, juce::Path& p, bool /*isMouseOver*/, bool /*isMouseDown*/)
        {
            const auto index = button.getIndex();
            const auto numTabs = button.getTabbedButtonBar().getNumTabs();
            auto activeArea = button.getActiveArea();
            auto w = (float) activeArea.getWidth();
            auto h = (float) activeArea.getHeight();

            auto length = w;
            auto depth = h;

            if (button.getTabbedButtonBar().isVertical())
                std::swap (length, depth);

            const float indent = (float) getTabButtonOverlap ((int) depth);
            const float overhang = 0.0f;

            if (button.isFrontTab()) {
              p.startNewSubPath (indent, h);
              p.lineTo (w - indent, h);
            }
            if (index > 0) {
              p.startNewSubPath(0.0f, 0.0f);
              p.lineTo(0.0f, h);
            }
            if (index < numTabs - 1) {
              p.startNewSubPath(w - indent, h);
              p.lineTo(w - indent, 0.0f);
            }
        };

        int getTabButtonBestWidth(juce::TabBarButton& button, int tabDepth) override {
          return button.getTabbedButtonBar().getWidth() / button.getTabbedButtonBar().getNumTabs();
        };

        void drawTabButton (juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown)
        {
            juce::Path tabShape;
            createTabButtonShape (button, tabShape, isMouseOver, isMouseDown);

            juce::Path tabOutline;
            createTabButtonOutline (button, tabOutline, isMouseOver, isMouseDown);

            auto activeArea = button.getActiveArea();

            tabShape.applyTransform (juce::AffineTransform::translation ((float) activeArea.getX(),
                                                                   (float) activeArea.getY()));

            juce::DropShadow (juce::Colours::black.withAlpha (1.0f), 2, juce::Point<int> (0, 1)).drawForPath (g, tabShape);

            fillTabButtonShape (button, g, tabShape, isMouseOver, isMouseDown);
            strokeTabButtonShape (button, g, tabOutline);
            drawTabButtonText (button, g, isMouseOver, isMouseDown);
        };

        void fillTabButtonShape (juce::TabBarButton& button, juce::Graphics& g, const juce::Path& path,
                                                 bool /*isMouseOver*/, bool /*isMouseDown*/)
        {
            auto tabBackground = button.getTabBackgroundColour();
            const bool isFrontTab = button.isFrontTab();

            g.setColour (isFrontTab ? tabBackground : tabBackground.withMultipliedAlpha (0.8f));

            g.fillPath (path);
        };

        void strokeTabButtonShape (juce::TabBarButton& button, juce::Graphics& g, const juce::Path& path)
        {
            const bool isFrontTab = button.isFrontTab();
            g.setColour (button.findColour (isFrontTab ? juce::TabbedButtonBar::frontOutlineColourId
                                                       : juce::TabbedButtonBar::tabOutlineColourId, false)
                            .withMultipliedAlpha (button.isEnabled() ? 1.0f : 1.0f));

            g.strokePath (path, PathStrokeType (isFrontTab ? 1.0f : 1.0f));
        }
    } tabLookAndFeel;

    class MyTabComponent : public juce::TabbedComponent
    {
      public:
        MyTabComponent() : TabbedComponent(juce::TabbedButtonBar::TabsAtBottom)
        {
          TabChangedFunc = [](int, const String&) {};
        }
        void currentTabChanged(int index, const String& name) override
        {
          TabChangedFunc(index, name);
        }
        std::function<void(int, const String&)> TabChangedFunc;
    } tabs;
    KnobLookAndFeel knobLookAndFeel;
    juce::Slider delay_speed_slider;
    RandomSpeedComponent random_speed_component;

    juce::AudioProcessorValueTreeState::SliderAttachment delay_speed_attachment;
    juce::ParameterAttachment delay_mode_attachment;

    juce::Label delay_speed_label;
};
