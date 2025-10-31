#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <queue>
#include <random>
#include <set>

//==============================================================================
/**
 * JustWait MIDI Plugin Processor
 * Delays MIDI events by a specified wait time (0-10000ms in 50ms increments)
 */
class JustWaitAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    JustWaitAudioProcessor();
    ~JustWaitAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Parameter management
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================
    struct DelayedMidiEvent
    {
        juce::MidiMessage message;
        juce::int64 scheduledSample;

        DelayedMidiEvent(const juce::MidiMessage& msg, juce::int64 sample)
            : message(msg), scheduledSample(sample) {}

        bool operator>(const DelayedMidiEvent& other) const
        {
            return scheduledSample > other.scheduledSample;
        }
    };

    std::priority_queue<DelayedMidiEvent,
                        std::vector<DelayedMidiEvent>,
                        std::greater<DelayedMidiEvent>> delayedEvents;

    juce::int64 currentSamplePosition;
    double currentSampleRate;

    // Random number generation for likelihood
    std::mt19937 randomGenerator;
    std::uniform_real_distribution<float> distribution;

    // Polyphony tracking
    // Stores active notes as (channel << 8) | noteNumber
    std::set<int> activeNotes;

    // Helper method to create note key from channel and note number
    inline int makeNoteKey(int channel, int noteNumber) const
    {
        return (channel << 8) | noteNumber;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustWaitAudioProcessor)
};
