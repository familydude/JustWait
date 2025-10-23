#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 * JustWait MIDI Plugin Editor
 * Provides a simple UI with a single knob to control the wait time
 */
class JustWaitAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    JustWaitAudioProcessorEditor (JustWaitAudioProcessor&);
    ~JustWaitAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JustWaitAudioProcessor& audioProcessor;

    // UI Components
    juce::Slider waitMsSlider;
    juce::Label waitMsLabel;
    juce::Label valueLabel;

    // Parameter attachment
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> waitMsAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustWaitAudioProcessorEditor)
};
