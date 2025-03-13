#pragma once

#include <JuceHeader.h>
#include "djAudioPlayer.h"
#include "DeckGUI.h"

// MainComponent class
// Manages the main application interface, including deck GUIs and audio management
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    // Constructor: Initializes MainComponent and sets up audio channels
    MainComponent();

    // Destructor: Cleans up audio resources
    ~MainComponent() override;

    //==============================================================================
    // prepareToPlay: Called before audio playback starts
    // Configures sample rate and buffer size
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    // getNextAudioBlock: Called repeatedly to fill buffers with audio data
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // releaseResources: Releases resources allocated for audio playback
    void releaseResources() override;

    //==============================================================================
    // paint: Draws the background and other UI elements
    void paint(juce::Graphics& g) override;

    // resized: Arranges UI components when the window size changes
    void resized() override;

private:
    //==============================================================================
    // Audio format manager: Handles audio file formats (e.g., WAV, MP3)
    juce::AudioFormatManager formatManager;

    // Audio thumbnail cache: Caches waveforms for faster display
    juce::AudioThumbnailCache thumbnailCache{ 100 };

    // Audio players for each deck
    DJAudioPlayer player1;  // Manages playback for deck 1
    DJAudioPlayer player2;  // Manages playback for deck 2

    // GUI components for each deck
    DeckGUI GUI1{ &player1, formatManager, thumbnailCache };  // GUI for deck 1
    DeckGUI GUI2{ &player2, formatManager, thumbnailCache };  // GUI for deck 2

    // Mixer source: Combines audio from both decks for output
    juce::MixerAudioSource mixerSource;

    // Prevents copying and assignment of MainComponent
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
