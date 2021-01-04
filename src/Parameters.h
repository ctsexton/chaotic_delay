#pragma once
#include <JuceHeader.h>

juce::AudioProcessorValueTreeState::ParameterLayout createLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout parameters{
        std::make_unique<juce::AudioParameterFloat>("dry_wet", "Dry/Wet", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("delay_feedback", "Feedback", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("delay_rate_of_change", "Rate of Change", 0.0f, 1.0f, 0.05f),
        std::make_unique<juce::AudioParameterFloat>("delay_range_lower_bound", "Delay Speed Lower Bound", -2.0f, 2.0f,
                                                    -1.0f),
        std::make_unique<juce::AudioParameterFloat>("delay_range_upper_bound", "Delay Speed Upper Bound", -2.0f, 2.0f,
                                                    1.0f),
        std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.0f),
    };
    return parameters;
}
