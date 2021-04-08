#pragma once
#include <JuceHeader.h>

juce::AudioProcessorValueTreeState::ParameterLayout createLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout parameters{
        std::make_unique<juce::AudioParameterFloat>("dry_wet", "Dry/Wet", 0.0f, 1.0f, 0.4f),
        std::make_unique<juce::AudioParameterFloat>("delay_feedback", "Feedback", 0.0f, 1.0f, 0.3f),
        std::make_unique<juce::AudioParameterFloat>(
            "delay_chance_of_change", 
            "Chance of Change", 
            0.0f,
            1.0f,
            1.0f
        ),
        std::make_unique<juce::AudioParameterFloat>(
            "delay_rate_of_change", 
            "Rate of Change", 
            juce::NormalisableRange<float>(0.2f, 20.0f, 0.0005f, 0.5f),
            1.0f
        ),
        std::make_unique<juce::AudioParameterFloat>("delay_range_lower_bound", "Delay Speed Lower Bound", -2.0f, 2.0f,
                                                    -1.0f),
        std::make_unique<juce::AudioParameterFloat>("delay_range_upper_bound", "Delay Speed Upper Bound", -2.0f, 2.0f,
                                                    1.0f),
        std::make_unique<juce::AudioParameterFloat>("delay_speed", "Delay Speed", -2.0f, 2.0f, 1.00000f),
        std::make_unique<juce::AudioParameterFloat>("delay_time", "Delay Time", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterChoice>("delay_mode", "Mode", juce::StringArray("crazy", "manual"), 1),
        std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.75f),
    };
    return parameters;
}
