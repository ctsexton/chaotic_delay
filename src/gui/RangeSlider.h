#pragma once
#include <JuceHeader.h>

class RangeSliderLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    RangeSliderLookAndFeel() {};

    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                                           float sliderPos,
                                           float minSliderPos,
                                           float maxSliderPos,
                                           const Slider::SliderStyle style, Slider& slider) override
    {
        auto isTwoVal = true;
        auto trackWidth = jmin (20.0f, (float) height * 0.25f);

        Point<float> startPoint ((float) x, (float) y + (float) height * 0.5f);
        Point<float> endPoint ((float) (width + x), startPoint.y);

        Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (juce::Colour(200, 200, 200));
        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::mitered, PathStrokeType::butt });

        Path valueTrack;
        Point<float> minPoint, maxPoint, thumbPoint;

        minPoint = { minSliderPos, (float) height * 0.5f };

        maxPoint = { maxSliderPos, (float) height * 0.5f };

        auto thumbWidth = getSliderThumbRadius (slider);

        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (maxPoint);
        g.setColour(juce::Colour(195, 100, 255));
        g.strokePath (valueTrack, { trackWidth, PathStrokeType::mitered, PathStrokeType::butt });

        auto sr = jmin (trackWidth, (float) height * 0.4f);
        /* auto pointerColour = slider.findColour (Slider::thumbColourId); */
        auto pointerColour = juce::Colour(253, 184, 51);

        drawPointer (g, minSliderPos,
                     (float) y + (float) height * 0.5f - trackWidth * 2.0f,
                     trackWidth * 2.0f, pointerColour, -1);

        drawPointer (g, maxSliderPos,
                     (float) y + (float) height * 0.5f - trackWidth * 2.0f,
                     trackWidth * 2.0f, pointerColour, 1);
    }

    void drawPointer (Graphics& g, const float x, const float y, const float diameter,
                                      const Colour& colour, const int direction) noexcept
    {
        Path p;
        p.startNewSubPath (x + diameter * 0.25f * direction, y + diameter * 2.0f);
        p.lineTo (x, y + diameter * 2.0f);
        p.lineTo (x, y);
        p.lineTo (x + diameter * 0.25f * direction, y);

        g.setColour (colour);
        g.strokePath (p, PathStrokeType(1.0f));
    }
};
