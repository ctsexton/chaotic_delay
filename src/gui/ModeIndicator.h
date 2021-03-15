#pragma once
#include <JuceHeader.h>

class ModeIndicator : public juce::Component {
  public:
    ModeIndicator(juce::AudioProcessorValueTreeState& vts)
      :
        mode_attachment(*vts.getParameter("delay_mode"), [&](float value) {
        auto* mode = static_cast<juce::AudioParameterChoice*>(vts.getParameter("delay_mode"));
        auto current = mode->getCurrentValueAsText();
        auto index = values.indexOf(current);
      })
    {
      auto* mode = static_cast<juce::AudioParameterChoice*>(vts.getParameter("delay_mode"));
      auto text_values = mode->getAllValueStrings();
      for (auto text : text_values) {
        labels.push_back(std::make_unique<juce::Label>(text, text));
      }


      mode_attachment.sendInitialUpdate();
      for (auto* label : labels) {
        addAndMakeVisible(*label);
      }
    }
  private:
    juce::ParameterAttachment mode_attachment;
    std::vector<juce::Label*> labels;
};
