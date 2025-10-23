#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JustWaitAudioProcessor::JustWaitAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()),
      currentSamplePosition(0),
      currentSampleRate(44100.0)
{
}

JustWaitAudioProcessor::~JustWaitAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout JustWaitAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // WaitMS parameter: 0-10000ms in 50ms increments
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "waitMs",                                    // parameter ID
        "Wait For (ms)",                             // parameter name
        juce::NormalisableRange<float>(
            0.0f,                                    // min value
            10000.0f,                                // max value
            50.0f                                    // interval (50ms increments)
        ),
        0.0f                                         // default value
    ));

    return layout;
}

//==============================================================================
const juce::String JustWaitAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JustWaitAudioProcessor::acceptsMidi() const
{
    return true;
}

bool JustWaitAudioProcessor::producesMidi() const
{
    return true;
}

bool JustWaitAudioProcessor::isMidiEffect() const
{
    return true;
}

double JustWaitAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JustWaitAudioProcessor::getNumPrograms()
{
    return 1;
}

int JustWaitAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JustWaitAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String JustWaitAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void JustWaitAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void JustWaitAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);
    currentSampleRate = sampleRate;
    currentSamplePosition = 0;

    // Clear any pending delayed events
    while (!delayedEvents.empty())
        delayedEvents.pop();
}

void JustWaitAudioProcessor::releaseResources()
{
    // Clear delayed events when audio processing stops
    while (!delayedEvents.empty())
        delayedEvents.pop();
}

bool JustWaitAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Support any layout since we're a MIDI effect
    juce::ignoreUnused(layouts);
    return true;
}

void JustWaitAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Clear the output buffer (we're a MIDI effect, no audio processing)
    buffer.clear();

    // Get the wait time parameter value in milliseconds
    auto waitMs = apvts.getRawParameterValue("waitMs")->load();

    // Convert wait time to samples
    auto delaySamples = static_cast<juce::int64>(waitMs * currentSampleRate / 1000.0);

    // Process incoming MIDI events and add them to the delay queue
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        auto samplePosition = currentSamplePosition + metadata.samplePosition;
        auto scheduledSample = samplePosition + delaySamples;

        delayedEvents.emplace(message, scheduledSample);
    }

    // Clear the incoming MIDI buffer
    midiMessages.clear();

    // Process delayed events that are ready to be played
    auto blockEndSample = currentSamplePosition + buffer.getNumSamples();

    while (!delayedEvents.empty() && delayedEvents.top().scheduledSample < blockEndSample)
    {
        const auto& event = delayedEvents.top();

        // Calculate the sample position within this block
        auto sampleInBlock = static_cast<int>(event.scheduledSample - currentSamplePosition);

        // Ensure the sample position is within bounds
        sampleInBlock = juce::jlimit(0, buffer.getNumSamples() - 1, sampleInBlock);

        // Add the delayed event to the output MIDI buffer
        midiMessages.addEvent(event.message, sampleInBlock);

        // Remove the event from the queue
        delayedEvents.pop();
    }

    // Update the current sample position
    currentSamplePosition += buffer.getNumSamples();
}

//==============================================================================
bool JustWaitAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* JustWaitAudioProcessor::createEditor()
{
    return new JustWaitAudioProcessorEditor(*this);
}

//==============================================================================
void JustWaitAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save parameter state
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void JustWaitAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Restore parameter state
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JustWaitAudioProcessor();
}
