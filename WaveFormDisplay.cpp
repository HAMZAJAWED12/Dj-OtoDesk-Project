/*
  ==============================================================================

    WaveFormDisplay.cpp
    Created: 21 Feb 2025 2:56:36pm
    Author:  LAPTOP WORLD

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveFormDisplay.h"

//==============================================================================
WaveFormDisplay::WaveFormDisplay(juce::AudioFormatManager& formatManagerToUse,
	juce::AudioThumbnailCache& cacheToUse)
	:audionail(1000, formatManagerToUse, cacheToUse), 
    isloaded(false)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audionail.addChangeListener(this);

}

WaveFormDisplay::~WaveFormDisplay()
{
}

void WaveFormDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);  // Background
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);


    if (isloaded) {
        audionail.drawChannel(g, getLocalBounds(), 0, audionail.getTotalLength(), 0, 1.0f);


        for (size_t i = 0; i < cuePoints.size(); ++i) {
            float xPos = getWidth() * (cuePoints[i] / audionail.getTotalLength());

            if (i == currentCueIndex) {
                g.setColour(juce::Colours::red);  // Highlight active cue
                g.fillEllipse(xPos - 6, getHeight() - 12, 12, 12);
            }
            else {
                g.setColour(juce::Colours::yellow);
                g.fillEllipse(xPos - 5, getHeight() - 10, 10, 10);
            }
        }
    }
    else {
        g.setFont(juce::Font(22.0f));
        g.drawText("File Not Loaded Yet", getLocalBounds(), juce::Justification::centred, true);
    }
}

void WaveFormDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveFormDisplay::loadURL(juce::URL& audioURL) {
	audionail.clear();
	isloaded = audionail.setSource(new juce::URLInputSource(audioURL));
	if (isloaded) {
		juce::Logger::outputDebugString("WaveFormDisplay::loadURL: Audio Loaded");
	}
    else {
		juce::Logger::outputDebugString("WaveFormDisplay::loadURL: Audio Not Loaded");
	}
}

void WaveFormDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
	juce::Logger::outputDebugString("WaveFormDisplay::changeListenerCallback]\n");
	repaint();
}


void WaveFormDisplay::addCuePoint(double position) {
    cuePoints.push_back(position);
    repaint();
}

void WaveFormDisplay::clearCuePoints() {
    cuePoints.clear();
    repaint();
}

double WaveFormDisplay::getTrackLength()
{
    return audionail.getTotalLength();
}

double WaveFormDisplay::getPositionRelative()
{
    if (audionail.getTotalLength() > 0 && position >= 0.0)
        return position / audionail.getTotalLength();
    else
        return 0.0;
}


void WaveFormDisplay::setPositionRelative(double pos)
{
    if (pos >= 0.0 && pos <= 1.0 && audionail.getTotalLength() > 0)
    {
        position = pos * audionail.getTotalLength();  // Update position
        repaint();  // Redraw waveform
    }
}

