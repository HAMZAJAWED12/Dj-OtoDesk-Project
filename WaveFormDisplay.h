/*
  ==============================================================================

    WaveFormDisplay.h
    Created: 21 Feb 2025 2:56:36pm
    Author:  LAPTOP WORLD

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveFormDisplay  : public juce::Component,
    public juce::ChangeListener
{
public:
    WaveFormDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveFormDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void loadURL(juce::URL& audioURL);

	void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void addCuePoint(double position);  //  Adds a cue point
    void clearCuePoints();  //Clears cue points
    const std::vector<double>& getCuePoints() const { return cuePoints; }  // Public getter
    int getCurrentCueIndex() const { return currentCueIndex; }
    void setCurrentCueIndex(int index) { currentCueIndex = index; }
    double getTrackLength();
    double getPositionRelative();
    void setPositionRelative(double pos); 





private:
	juce::AudioThumbnail audionail;
    bool isloaded;
    double position = 0.0;
    std::vector<double> cuePoints;
    int currentCueIndex = -1;  //  Keeps track of the last jumped cue point
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveFormDisplay)
};
