#pragma once
#include <JuceHeader.h>

class DefaultLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    DefaultLookAndFeel() {
      setDefaultMonospaceTypefaceName("Courier New");
    };
};
