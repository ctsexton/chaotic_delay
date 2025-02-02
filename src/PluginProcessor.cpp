#include "PluginProcessor.h"

#include "Parameters.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, juce::Identifier("Chaotic Delay"), createLayout()) {
    dry_wet = parameters.getRawParameterValue("dry_wet");
    gain_level = parameters.getRawParameterValue("gain");
    delay_feedback = parameters.getRawParameterValue("delay_feedback");
    delay_rate_of_change = parameters.getRawParameterValue("delay_rate_of_change");
    delay_chance_of_change = parameters.getRawParameterValue("delay_chance_of_change");
    delay_range_upper_bound = parameters.getRawParameterValue("delay_range_upper_bound");
    delay_range_lower_bound = parameters.getRawParameterValue("delay_range_lower_bound");
    delay_mode = parameters.getRawParameterValue("delay_mode");
    delay_speed = parameters.getRawParameterValue("delay_speed");
    delay_time = parameters.getRawParameterValue("delay_time");

    chain.get<gain>().setRampDurationSeconds(0.01f);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const { return JucePlugin_Name; }

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioPluginAudioProcessor::getNumPrograms() {
    return 1;  // NB: some hosts don't cope very well if you tell them there are 0
               // programs, so this should be at least 1, even if you're not
               // really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() { return 0; }

void AudioPluginAudioProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String& newName) {
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec{sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2};
    chain.prepare(spec);
    current_sample_rate = sampleRate;
}

void AudioPluginAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) return false;
#endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    countdown_length = (1.0 / *delay_rate_of_change) * current_sample_rate;

    chain.get<gain>().setGainLinear(*gain_level);
    chain.get<delay>().setFeedback(*delay_feedback);
    chain.get<delay>().setDryWet(*dry_wet);
    chain.get<delay>().setDelayTime(*delay_time);
    
    const float mode = *delay_mode;

    juce::dsp::AudioBlock<float> block(buffer);

    if (mode > 0.0f) {
      chain.get<delay>().setSpeed(*delay_speed);
      juce::dsp::ProcessContextReplacing<float> context(block);
      chain.process(context);
    }

    auto remaining_samples = buffer.getNumSamples();
    while (mode == 0.0f && remaining_samples > 0) {
      auto should_update_rate = false;
      auto block_size = remaining_samples;
      const auto start_sample_index = buffer.getNumSamples() - remaining_samples;

      if (countdown > remaining_samples) {
        countdown = countdown - remaining_samples;
        remaining_samples = 0;
      } else {
        block_size = countdown;
        remaining_samples = remaining_samples - countdown;
        countdown = countdown_length;
        should_update_rate = true;
      }
      if (countdown_length = 0) {
        break;
      }

      if (block_size > 0) {
        juce::dsp::AudioBlock<float> sub_block(block.getSubBlock(start_sample_index, block_size));
        juce::dsp::ProcessContextReplacing<float> context(sub_block);
        chain.process(context);
      }

      if (should_update_rate) {
        const float lower_bound = *delay_range_lower_bound;
        const float upper_bound = *delay_range_upper_bound;
        if (random.nextFloat() < *delay_chance_of_change) {
            const auto new_speed = juce::jmap<double>(random.nextFloat(), 0, 1, lower_bound, upper_bound);
            chain.get<delay>().setSpeed(new_speed);
        }
      }
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const {
    return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor() {
    return new AudioPluginAudioProcessorEditor(*this, parameters);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new AudioPluginAudioProcessor(); }
