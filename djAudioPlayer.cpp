/*
  ==============================================================================

    djAudioPlayer.cpp
    Created: 14 Feb 2025 3:09:14pm
    Author:  LAPTOP WORLD

  ==============================================================================
*/

#include "djAudioPlayer.h"

// Constructor for DJAudioPlayer
DJAudioPlayer::DJAudioPlayer() {

}

// Destructor for DJAudioPlayer
DJAudioPlayer::~DJAudioPlayer() {

}

// Prepares the audio player to play by registering formats and preparing sources
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    formatManager.registerBasicFormats();
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

// Gets the next block of audio to play
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    resamplingSource.getNextAudioBlock(bufferToFill);
}

// Releases resources used by the audio player
void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resamplingSource.releaseResources();
}

// Loads an audio file from a given URL
void DJAudioPlayer::LoadURL(juce::URL audioURL) {
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

// Sets the gain (volume) level for the audio player
void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 1) {
        juce::Logger::outputDebugString("DJAudioPlayer::setGain should be between 0 and 1\n");
    }
    else {
        transportSource.setGain(gain);
    }
}

// Sets the playback speed of the audio
void DJAudioPlayer::setspeed(double ratio) {
    if (ratio <= 0 || ratio > 100) {
        juce::Logger::outputDebugString("DJAudioPlayer::setSpeed should be between 0 and 100\n");
    }
    else {
        resamplingSource.setResamplingRatio(ratio);
    }
}

// Sets the playback position in seconds
void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

// Sets the playback position relative to the track length (0 to 1)
void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos > 1) {
        juce::Logger::outputDebugString("DJAudioPlayer::setPositionRelative should be between 0 and 1\n");
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

// Starts playback
void DJAudioPlayer::start() {
    transportSource.start();
}

// Stops playback
void DJAudioPlayer::stop() {
    transportSource.stop();
}

// Returns the current playback position in seconds
double DJAudioPlayer::getPosition() {
    return transportSource.getCurrentPosition();
}

// Returns the length of the track in seconds
double DJAudioPlayer::getLengthInSeconds() {
    return transportSource.getLengthInSeconds();
}
