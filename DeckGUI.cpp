/*
  ==============================================================================

    DeckGUI.cpp
    Created: 14 Feb 2025 3:50:28pm
    Author: LAPTOP WORLD

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _Player,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : player(_Player), waveDisplay(formatManagerToUse, cacheToUse)
{
    // Add and make visible all components
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(waveDisplay);
    addAndMakeVisible(setCueButton);
    addAndMakeVisible(jumpCueButton);

    // Button listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    setCueButton.addListener(this);
    jumpCueButton.addListener(this);

    // Slider listeners
    volSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);

    // Slider ranges
    volSlider.setRange(0, 1);
    speedSlider.setRange(0, 2);
    positionSlider.setRange(0, 1);

    // Slider styles - Knob type
    volSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    positionSlider.setSliderStyle(juce::Slider::Rotary);

    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    positionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    // Updated Neon Slider Colors
    volSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(0, 153, 255));
    speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(255, 32, 78));
    positionSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(255, 215, 0));




    // Add and make visible labels for sliders
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(positionLabel);

    // Set labels for sliders with soft white and subtle glow
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(230, 230, 250));  // Soft White

    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(230, 230, 250));  // Soft White

    positionLabel.setText("Position", juce::dontSendNotification);
    positionLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(230, 230, 250));  // Soft White


    // Button styling example
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff0099ff)); // Blue
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 32, 78)); // Red
    loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(32, 199, 255)); // Cyan
    setCueButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 215, 0)); // Yellow
    jumpCueButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(211, 68, 255)); // Purple

    // Start timer for GUI animations at 30 frames per second
    startTimerHz(30);


}

DeckGUI::~DeckGUI() {}

void DeckGUI::paint(juce::Graphics& g)
{
    // Background gradient
    juce::ColourGradient backgroundGradient(
        juce::Colour(20, 20, 40), 0.0f, 0.0f,
        juce::Colour(2, 2, 10), 0.0f, static_cast<float>(getHeight()), false);
    g.setGradientFill(backgroundGradient);
    g.fillAll();

    // Border
    g.setColour(juce::Colours::darkgrey);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(3.0f), 8.0f, 2.0f);

    // Animated Logo explicitly positioned safely
    float alphaLogo = 0.6f + 0.4f * std::sin(juce::Time::getMillisecondCounterHiRes() * 0.003f);
    g.setColour(juce::Colours::white.withAlpha(alphaLogo));
    g.setFont(juce::Font("Arial Black", 18.0f, juce::Font::bold));
    g.drawText("Otodecks DJ PRO",
        0.0f,
        5.0f,
        static_cast<float>(getWidth()),
        30.0f,
        juce::Justification::centred);

    // Defensive and safe calculation for relativePosition
    float relativePosition = waveDisplay.getPositionRelative();

    // Debugging: Print relative position to console to ensure it's updating
    DBG("Relative Position: " << relativePosition);

    // Safety Check to ensure range [0,1]
    if (relativePosition < 0.0f || relativePosition > 1.0f || std::isnan(relativePosition))
        relativePosition = 0.0f;

    drawProgressBar(g, relativePosition);

    // Defensive check for beat indicator (prevent negative positions)
    double pos = player->getPosition();

    // Debugging: Print player position to console to ensure it's updating
    DBG("Player Position: " << pos);

    if (pos < 0.0)
        pos = 0.0;  // Ensure no negative positions

    bool isBeat = (pos > 0.0) && (static_cast<int>(pos * 4) % 2 == 0);
    drawBeatIndicator(g, isBeat);
}


void DeckGUI::resized()
{
    auto padding = 8;
    auto buttonHeight = 30;
    int sliderSize = (getWidth() - 4 * padding) / 3;

    // Play and Stop buttons slightly lower to show logo clearly
    playButton.setBounds(padding, 40, getWidth() / 2 - 1.5 * padding, buttonHeight);
    stopButton.setBounds(getWidth() / 2 + padding / 2, 40, getWidth() / 2 - 1.5 * padding, buttonHeight);

    // Waveform Display (adjusted)
    waveDisplay.setBounds(padding, playButton.getBottom() + padding, getWidth() - 2 * padding, getHeight() / 3);

    // Sliders positioned just below waveform
    int sliderY = waveDisplay.getBottom() + padding;
    volSlider.setBounds(padding, sliderY, sliderSize, sliderSize);
    speedSlider.setBounds(volSlider.getRight() + padding, sliderY, sliderSize, sliderSize);
    positionSlider.setBounds(speedSlider.getRight() + padding, sliderY, sliderSize, sliderSize);

    // Slider labels below each slider
    volLabel.setBounds(volSlider.getX(), volSlider.getBottom(), sliderSize, 20);
    speedLabel.setBounds(speedSlider.getX(), speedSlider.getBottom(), sliderSize, 20);
    positionLabel.setBounds(positionSlider.getX(), positionSlider.getBottom(), sliderSize, 20);

    // Cue buttons positioned closer to sliders to reduce gap
    setCueButton.setBounds(padding, volLabel.getBottom() + padding, getWidth() / 2 - 1.5 * padding, buttonHeight);
    jumpCueButton.setBounds(setCueButton.getRight() + padding, volLabel.getBottom() + padding, getWidth() / 2 - 1.5 * padding, buttonHeight);

    // Load button positioned closer to cue buttons to remove large gap
    loadButton.setBounds(padding, setCueButton.getBottom() + padding, getWidth() - 2 * padding, buttonHeight);
}


void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        juce::Logger::outputDebugString("Play Button Was Clicked!");
        player->start();
    }
    if (button == &stopButton)
    {
        juce::Logger::outputDebugString("Stop Button Was Clicked!");
        player->stop();
    }

    if (button == &loadButton) {
        juce::Logger::outputDebugString("Load Button was Clicked");

        auto filechooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(filechooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File chosenFile = chooser.getResult();
                player->LoadURL(juce::URL{ chosenFile });
                waveDisplay.loadURL(juce::URL{ chosenFile });
            });
    }

    if (button == &setCueButton) {
        double currentPosition = player->getPosition();
        if (currentPosition > 0) {
            auto& cuePoints = waveDisplay.getCuePoints();
            if (std::find(cuePoints.begin(), cuePoints.end(), currentPosition) == cuePoints.end()) {
                waveDisplay.addCuePoint(currentPosition);
            }
        }
    }

    if (button == &jumpCueButton && !waveDisplay.getCuePoints().empty()) {
        auto& cuePoints = waveDisplay.getCuePoints();
        int cueIndex = waveDisplay.getCurrentCueIndex();
        cueIndex = (cueIndex + 1) % cuePoints.size();
        waveDisplay.setCurrentCueIndex(cueIndex);
        waveDisplay.repaint();
        player->setPosition(cuePoints[cueIndex]);
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider) player->setGain(slider->getValue());
    if (slider == &speedSlider) player->setspeed(slider->getValue());
    if (slider == &positionSlider) player->setPositionRelative(slider->getValue());
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) { return true; }

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (files.size() == 1) player->LoadURL(juce::URL{ juce::File{files[0]} });
}

void DeckGUI::timerCallback()
{
    // Get the current position in seconds from the player
    double currentPos = player->getPosition();
    double trackLength = player->getLengthInSeconds();

    // Update WaveFormDisplay's position relative to track length
    if (trackLength > 0)
        waveDisplay.setPositionRelative(currentPos / trackLength);

    repaint();  // Redraw the GUI
}


// Draw Beat Indicator (Small Pulsing circle synced to beats)
void DeckGUI::drawBeatIndicator(juce::Graphics& g, bool isBeat)
{
    float radius = 8.f;
    auto indicatorPos = juce::Point<float>(getWidth() - 20.f, 20.f);
    g.setColour(isBeat ? juce::Colours::lime : juce::Colours::grey);
    g.fillEllipse(indicatorPos.x - radius, indicatorPos.y - radius, radius * 2.f, radius * 2.f);
}

void DeckGUI::drawProgressBar(juce::Graphics& g, float progress)
{
    // Get the updated position relative to the track length
    progress = waveDisplay.getPositionRelative();  // Ensure it uses the latest value

    // Clamp progress explicitly between 0 and 1
    progress = juce::jlimit(0.0f, 1.0f, progress);

    int barWidth = getWidth() - 20;
    int yPos = waveDisplay.getBottom() + 8;

    // Debug: Check progress value
    DBG("Progress Bar Position: " << progress);

    // Background
    g.setColour(juce::Colours::darkgrey);
    g.fillRoundedRectangle(10.0f, static_cast<float>(yPos), static_cast<float>(barWidth), 5.0f, 2.0f);

    // Progress Indicator
    g.setColour(juce::Colours::cyan);
    g.fillRoundedRectangle(10.0f, static_cast<float>(yPos), barWidth * progress, 5.0f, 2.5f);
}


void DeckGUI::drawAnimatedLogo(juce::Graphics& g, float alphaValue)
{
    g.setColour(juce::Colours::white.withAlpha(alphaValue));
    g.setFont(juce::Font("Arial Black", 20.f, juce::Font::bold));
    g.drawText("Otodecks DJ PRO", getLocalBounds().removeFromTop(30), juce::Justification::centred);
}
