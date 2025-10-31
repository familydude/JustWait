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

    // Set up the wait time label
    waitMsLabel.setText("Wait For...", juce::dontSendNotification);
    waitMsLabel.setJustificationType(juce::Justification::centred);
    waitMsLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    addAndMakeVisible(waitMsLabel);

    // Set up the wait time value label (shows current value)
    waitMsValueLabel.setJustificationType(juce::Justification::centred);
    waitMsValueLabel.setFont(juce::Font(14.0f));
    addAndMakeVisible(waitMsValueLabel);

    // Set up the likelihood slider (knob)
    likelihoodSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    likelihoodSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    likelihoodSlider.setPopupDisplayEnabled(true, true, this);
    likelihoodSlider.setTextValueSuffix(" %");
    addAndMakeVisible(likelihoodSlider);

    // Set up the likelihood label
    likelihoodLabel.setText("Likelihood", juce::dontSendNotification);
    likelihoodLabel.setJustificationType(juce::Justification::centred);
    likelihoodLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    addAndMakeVisible(likelihoodLabel);

    // Set up the likelihood value label
    likelihoodValueLabel.setJustificationType(juce::Justification::centred);
    likelihoodValueLabel.setFont(juce::Font(14.0f));
    addAndMakeVisible(likelihoodValueLabel);

    // Attach sliders to parameters
    waitMsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "waitMs", waitMsSlider);

    likelihoodAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "likelihood", likelihoodSlider);

    // Update wait time value label when slider changes
    waitMsSlider.onValueChange = [this]()
    {
        auto value = waitMsSlider.getValue();

        // Format the display value
        if (value >= 1000.0f)
        {
            waitMsValueLabel.setText(juce::String(value / 1000.0f, 2) + " s",
                                     juce::dontSendNotification);
        }
        else
        {
            waitMsValueLabel.setText(juce::String(static_cast<int>(value)) + " ms",
                                     juce::dontSendNotification);
        }
    };

    // Update likelihood value label when slider changes
    likelihoodSlider.onValueChange = [this]()
    {
        auto value = likelihoodSlider.getValue();
        likelihoodValueLabel.setText(juce::String(static_cast<int>(value)) + " %",
                                      juce::dontSendNotification);
    };

    // Set initial value displays
    waitMsSlider.onValueChange();
    likelihoodSlider.onValueChange();

    // Set editor size (wider to accommodate two knobs)
    setSize (500, 300);
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
    auto labelHeight = 25;
    auto valueLabelHeight = 20;
    auto knobSize = 140;
    auto spacing = 10;

    // Split the area into two columns
    auto leftColumn = bounds.removeFromLeft(bounds.getWidth() / 2);
    auto rightColumn = bounds;

    // Layout Wait For knob (left column)
    leftColumn.removeFromTop(spacing);
    waitMsLabel.setBounds(leftColumn.removeFromTop(labelHeight));
    leftColumn.removeFromTop(spacing);

    auto waitKnobBounds = leftColumn.removeFromTop(knobSize);
    waitKnobBounds = waitKnobBounds.withSizeKeepingCentre(knobSize, knobSize);
    waitMsSlider.setBounds(waitKnobBounds);

    leftColumn.removeFromTop(spacing);
    waitMsValueLabel.setBounds(leftColumn.removeFromTop(valueLabelHeight));

    // Layout Likelihood knob (right column)
    rightColumn.removeFromTop(spacing);
    likelihoodLabel.setBounds(rightColumn.removeFromTop(labelHeight));
    rightColumn.removeFromTop(spacing);

    auto likelihoodKnobBounds = rightColumn.removeFromTop(knobSize);
    likelihoodKnobBounds = likelihoodKnobBounds.withSizeKeepingCentre(knobSize, knobSize);
    likelihoodSlider.setBounds(likelihoodKnobBounds);

    rightColumn.removeFromTop(spacing);
    likelihoodValueLabel.setBounds(rightColumn.removeFromTop(valueLabelHeight));
}
