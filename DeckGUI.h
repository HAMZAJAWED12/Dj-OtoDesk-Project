#pragma once

#include <JuceHeader.h>
#include "djAudioPlayer.h"
#include "WaveFormDisplay.h"

// DeckGUI class
// Manages the user interface for each deck, including buttons, sliders, and waveform display
// Also handles special effects and file drag-and-drop
class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    private juce::Timer  // Used for timing-based effects
{
public:
    // Constructor: Initializes the DeckGUI with a DJAudioPlayer instance
    DeckGUI(DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);

    // Destructor: Cleans up resources
    ~DeckGUI() override;

    // paint: Draws UI components and custom graphics
    void paint(juce::Graphics&) override;

    // resized: Arranges UI components when the window size changes
    void resized() override;

    // Handles button click events
    void buttonClicked(juce::Button*) override;

    // Handles slider value change events
    void sliderValueChanged(juce::Slider*) override;

    // File drag-and-drop support
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    //==============================================================================
    // Special Effects Methods

    // timerCallback: Called periodically to update UI animations
    void timerCallback() override;

    // drawBeatIndicator: Draws a visual indicator for beats
    void drawBeatIndicator(juce::Graphics& g, bool isBeat);

    // drawProgressBar: Draws a progress bar based on playback position
    void drawProgressBar(juce::Graphics& g, float progress);

    // drawAnimatedLogo: Draws an animated logo with varying transparency
    void drawAnimatedLogo(juce::Graphics& g, float alpha);

    //==============================================================================
    // UI Components

    // Buttons for playback control and cue points
    juce::TextButton playButton{ "Play" },
        stopButton{ "Stop" },
        loadButton{ "LOAD" },
        setCueButton{ "SET CUE" },
        jumpCueButton{ "JUMP CUE" };

    // Sliders for volume, speed, and position control with rotary style
    juce::Slider volSlider{ juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow },
        speedSlider{ juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow },
        positionSlider{ juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };

    // File chooser for loading audio files
    juce::FileChooser fChooser{ "Select a File.." };

    // Pointers to manage audio playback and waveform display
    DJAudioPlayer* player;
    WaveFormDisplay waveDisplay;

    // Labels for sliders to indicate function (Volume, Speed, Position)
    juce::Label volLabel{ {}, "Volume" },
        speedLabel{ {}, "Speed" },
        positionLabel{ {}, "Position" };

    // Prevents copying and assignment of DeckGUI
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
