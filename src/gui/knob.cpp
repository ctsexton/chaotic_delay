/*
  ==============================================================================

    knob.cpp
    Created: 23 Dec 2020 12:39:33am
    Author:  cts

  ==============================================================================
*/

#include "knob.h"

KnobLookAndFeel::KnobLookAndFeel() {};

KnobLookAndFeel::KnobLookAndFeel(juce::Colour backgroundColour, juce::Colour outlineColour, juce::Colour pointerColour, juce::Colour baseColour, juce::Colour trackColour) :
  backgroundColour(backgroundColour),
  outlineColour(outlineColour),
  pointerColour(pointerColour),
  trackColour(trackColour),
  baseColour(baseColour)
  {}

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, 
  const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
  auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
  auto centreX = (float) x + (float) width  * 0.5f;
  auto centreY = (float) y + (float) height * 0.5f;
  auto rx = centreX - radius;
  auto ry = centreY - radius;
  auto rw = radius * 2.0f;
  auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

  // fill behind
  g.setColour (baseColour);
  g.fillRect(x, y, width, height);

  // outline
  g.setColour (outlineColour);
  g.fillEllipse (rx + (radius * 0.4), ry + (radius * 0.4), rw * 0.6, rw * 0.6);

  Path arcTrack;
  g.setColour (trackColour);
  arcTrack.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.8);
  g.fillPath (arcTrack);

  Path filledArc;
  g.setColour (outlineColour);
  filledArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, angle, 0.8);
  g.fillPath (filledArc);


  juce::Path p;
  auto pointerLength = radius * 0.33f;
  auto pointerThickness = 4.0f;
  p.addRectangle (-pointerThickness * 0.5f, -radius * 0.6, pointerThickness, pointerLength);
  p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));

  // pointer
  g.setColour (pointerColour);
  g.fillPath (p);
}

void KnobLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                           bool, bool isButtonDown)
{
    auto buttonArea = button.getLocalBounds();
    auto edge = 4;

    buttonArea.removeFromLeft (edge);
    buttonArea.removeFromTop (edge);

    // shadow
    g.setColour (juce::Colours::darkgrey.withAlpha (0.5f));
    g.fillRect (buttonArea);

    auto offset = isButtonDown ? -edge / 2 : -edge;
    buttonArea.translate (offset, offset);

    g.setColour (baseColour);
    g.fillRect (buttonArea);
}

void KnobLookAndFeel::drawButtonText (juce::Graphics& g, juce::TextButton& button, bool, bool isButtonDown)
{
    auto font = getTextButtonFont (button, button.getHeight());
    g.setFont (font);
    g.setColour (button.findColour (button.getToggleState() ? juce::TextButton::textColourOnId
                                                            : juce::TextButton::textColourOffId)
                       .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    auto yIndent = juce::jmin (4, button.proportionOfHeight (0.3f));
    auto cornerSize = juce::jmin (button.getHeight(), button.getWidth()) / 2;

    auto fontHeight = juce::roundToInt (font.getHeight() * 0.6f);
    auto leftIndent  = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft()  ? 4 : 2));
    auto rightIndent = juce::jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;

    auto edge = 4;
    auto offset = isButtonDown ? edge / 2 : 0;

    if (textWidth > 0)
        g.drawFittedText (button.getButtonText(),
                          leftIndent + offset, yIndent + offset, textWidth, button.getHeight() - yIndent * 2 - edge,
                          juce::Justification::centred, 2);
}
