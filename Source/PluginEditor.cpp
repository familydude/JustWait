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

    // Set up the max polyphony slider (knob)
    maxPolyphonySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    maxPolyphonySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    maxPolyphonySlider.setPopupDisplayEnabled(true, true, this);
    maxPolyphonySlider.setTextValueSuffix(" notes");
    addAndMakeVisible(maxPolyphonySlider);

    // Set up the max polyphony label
    maxPolyphonyLabel.setText("Max Notes", juce::dontSendNotification);
    maxPolyphonyLabel.setJustificationType(juce::Justification::centred);
    maxPolyphonyLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    addAndMakeVisible(maxPolyphonyLabel);

    // Set up the max polyphony value label
    maxPolyphonyValueLabel.setJustificationType(juce::Justification::centred);
    maxPolyphonyValueLabel.setFont(juce::Font(14.0f));
    addAndMakeVisible(maxPolyphonyValueLabel);

    // Attach sliders to parameters
    waitMsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "waitMs", waitMsSlider);

    likelihoodAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "likelihood", likelihoodSlider);

    maxPolyphonyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "maxPolyphony", maxPolyphonySlider);

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

    // Update max polyphony value label when slider changes
    maxPolyphonySlider.onValueChange = [this]()
    {
        auto value = static_cast<int>(maxPolyphonySlider.getValue());
        if (value >= 128)
        {
            maxPolyphonyValueLabel.setText("Unlimited", juce::dontSendNotification);
        }
        else
        {
            maxPolyphonyValueLabel.setText(juce::String(value) + " notes",
                                            juce::dontSendNotification);
        }
    };

    // Set initial value displays
    waitMsSlider.onValueChange();
    likelihoodSlider.onValueChange();
    maxPolyphonySlider.onValueChange();

    // Set editor size (wider to accommodate three knobs)
    setSize (700, 300);
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
    auto knobSize = 130;
    auto spacing = 10;

    // Split the area into three columns
    auto columnWidth = bounds.getWidth() / 3;
    auto leftColumn = bounds.removeFromLeft(columnWidth);
    auto middleColumn = bounds.removeFromLeft(columnWidth);
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

    // Layout Likelihood knob (middle column)
    middleColumn.removeFromTop(spacing);
    likelihoodLabel.setBounds(middleColumn.removeFromTop(labelHeight));
    middleColumn.removeFromTop(spacing);

    auto likelihoodKnobBounds = middleColumn.removeFromTop(knobSize);
    likelihoodKnobBounds = likelihoodKnobBounds.withSizeKeepingCentre(knobSize, knobSize);
    likelihoodSlider.setBounds(likelihoodKnobBounds);

    middleColumn.removeFromTop(spacing);
    likelihoodValueLabel.setBounds(middleColumn.removeFromTop(valueLabelHeight));

    // Layout Max Polyphony knob (right column)
    rightColumn.removeFromTop(spacing);
    maxPolyphonyLabel.setBounds(rightColumn.removeFromTop(labelHeight));
    rightColumn.removeFromTop(spacing);

    auto maxPolyphonyKnobBounds = rightColumn.removeFromTop(knobSize);
    maxPolyphonyKnobBounds = maxPolyphonyKnobBounds.withSizeKeepingCentre(knobSize, knobSize);
    maxPolyphonySlider.setBounds(maxPolyphonyKnobBounds);

    rightColumn.removeFromTop(spacing);
    maxPolyphonyValueLabel.setBounds(rightColumn.removeFromTop(valueLabelHeight));
}
