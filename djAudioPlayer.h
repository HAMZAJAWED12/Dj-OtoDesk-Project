/*
  ==============================================================================

    djAudioPlayer.h
    Created: 14 Feb 2025 3:09:14pm
    Author:  LAPTOP WORLD

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

// DJAudioPlayer class declaration inheriting from juce::AudioSource
class DJAudioPlayer : public juce::AudioSource {
public:

    // Constructor
    DJAudioPlayer();

    // Destructor
    ~DJAudioPlayer();

    // Prepares the audio player for playback
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    // Fills the buffer with the next block of audio samples
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Releases any resources used by the audio player
    void releaseResources() override;

    // Loads an audio file from a URL
    void LoadURL(juce::URL audioURL);

    // Sets the volume level (gain) of the audio
    void setGain(double gain);

    // Sets the playback speed of the audio
    void setspeed(double ratio);

    // Sets the playback position in seconds
    void setPosition(double posInSecs);

    // Sets the playback position relative to the track length (0 to 1)
    void setPositionRelative(double pos);

    // Starts audio playback
    void start();

    // Stops audio playback
    void stop();

    // Returns the current playback position in seconds
    double getPosition();

    // Returns the total length of the track in seconds
    double getLengthInSeconds();

private:
    // Manages different audio formats
    juce::AudioFormatManager formatManager;

    // Manages reading audio files
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;

    // Manages playback transport (play, stop, etc.)
    juce::AudioTransportSource transportSource;

    // Manages resampling to adjust playback speed
    juce::ResamplingAudioSource resamplingSource{ &transportSource, false, 2 };

};
