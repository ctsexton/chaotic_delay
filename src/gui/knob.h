/*
  ==============================================================================

    knob.h
    Created: 23 Dec 2020 12:39:33am
    Author:  cts

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class KnobLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    KnobLookAndFeel();

    KnobLookAndFeel(juce::Colour backgroundColour, juce::Colour outlineColour, juce::Colour pointerColour, juce::Colour baseColour, juce::Colour trackColour);

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, 
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;

    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool, bool isButtonDown) override;

    void drawButtonText (juce::Graphics& g, juce::TextButton& button, bool, bool isButtonDown) override;

  private:
    juce::Colour backgroundColour = juce::Colours::grey;
    juce::Colour baseColour = juce::Colours::red;
    juce::Colour outlineColour = juce::Colours::black;
    juce::Colour pointerColour = juce::Colours::white;
    juce::Colour trackColour = juce::Colours::grey;
};
