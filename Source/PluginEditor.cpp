#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JustWaitAudioProcessorEditor::JustWaitAudioProcessorEditor (JustWaitAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set up the wait time slider (knob)
    waitMsSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    waitMsSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    waitMsSlider.setPopupDisplayEnabled(true, true, this);
    waitMsSlider.setTextValueSuffix(" ms");
    addAndMakeVisible(waitMsSlider);

    // Set up the parameter label
    waitMsLabel.setText("Wait For...", juce::dontSendNotification);
    waitMsLabel.setJustificationType(juce::Justification::centred);
    waitMsLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    addAndMakeVisible(waitMsLabel);

    // Set up the value label (shows current value)
    valueLabel.setJustificationType(juce::Justification::centred);
    valueLabel.setFont(juce::Font(16.0f));
    addAndMakeVisible(valueLabel);

    // Attach the slider to the parameter
    waitMsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "waitMs", waitMsSlider);

    // Update value label when slider changes
    waitMsSlider.onValueChange = [this]()
    {
        auto value = waitMsSlider.getValue();

        // Format the display value
        if (value >= 1000.0f)
        {
            valueLabel.setText(juce::String(value / 1000.0f, 2) + " s",
                             juce::dontSendNotification);
        }
        else
        {
            valueLabel.setText(juce::String(static_cast<int>(value)) + " ms",
                             juce::dontSendNotification);
        }
    };

    // Set initial value display
    waitMsSlider.onValueChange();

    // Set editor size
    setSize (400, 300);
}

JustWaitAudioProcessorEditor::~JustWaitAudioProcessorEditor()
{
}

//==============================================================================
void JustWaitAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill background with a gradient
    g.fillAll(juce::Colour(0xff1a1a2e));

    auto bounds = getLocalBounds();

    // Draw a subtle gradient background
    juce::ColourGradient gradient(
        juce::Colour(0xff16213e), bounds.getWidth() * 0.5f, 0.0f,
        juce::Colour(0xff0f1626), bounds.getWidth() * 0.5f, bounds.getHeight(),
        false);
    g.setGradientFill(gradient);
    g.fillRect(bounds);

    // Draw a border
    g.setColour(juce::Colour(0xff4a5568));
    g.drawRect(bounds, 2);
}

void JustWaitAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto padding = 20;
    bounds.reduce(padding, padding);

    // Layout components
    auto labelHeight = 30;
    auto knobSize = 150;

    // Title label at the top
    waitMsLabel.setBounds(bounds.removeFromTop(labelHeight));
    bounds.removeFromTop(10); // spacing

    // Center the knob
    auto knobBounds = bounds.removeFromTop(knobSize);
    knobBounds = knobBounds.withSizeKeepingCentre(knobSize, knobSize);
    waitMsSlider.setBounds(knobBounds);

    bounds.removeFromTop(10); // spacing

    // Value label below the knob
    valueLabel.setBounds(bounds.removeFromTop(labelHeight));
}
