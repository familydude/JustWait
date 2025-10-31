#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
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
    juce::Label waitMsValueLabel;

    juce::Slider likelihoodSlider;
    juce::Label likelihoodLabel;
    juce::Label likelihoodValueLabel;

    juce::Slider maxPolyphonySlider;
    juce::Label maxPolyphonyLabel;
    juce::Label maxPolyphonyValueLabel;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> waitMsAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> likelihoodAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> maxPolyphonyAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JustWaitAudioProcessorEditor)
};
