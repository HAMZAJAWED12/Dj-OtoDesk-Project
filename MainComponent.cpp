

#include "MainComponent.h"

//==============================================================================
// Constructor for MainComponent
// Initializes the main interface, sets up audio sources, and configures UI elements
MainComponent::MainComponent()
{
    // Set the initial size of the main window
    setSize(800, 600);

    // Request microphone permission if required by the platform
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted)
            {
                // If permission is granted, open 2 input and 2 output channels; otherwise, no input
                setAudioChannels(granted ? 2 : 0, 2);
            });
    }
    else
    {
        // Open 2 input and 2 output channels if no permission is needed
        setAudioChannels(2, 2);
    }

    // Make GUI components visible
    addAndMakeVisible(GUI1);
    addAndMakeVisible(GUI2);

    // Register basic audio formats (e.g., WAV, MP3)
    formatManager.registerBasicFormats();
}

// Destructor for MainComponent
// Releases audio resources
MainComponent::~MainComponent()
{
    shutdownAudio();  // Clean up audio resources
}

//==============================================================================
// prepareToPlay: Prepares audio sources before playback begins
// Configures the mixer source to manage multiple audio streams
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Add audio players to the mixer source without taking ownership
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

    // Prepare mixer source with expected samples per block and sample rate
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

// getNextAudioBlock: Called repeatedly to supply audio data for playback
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);  // Fill buffer with audio data
}

// releaseResources: Releases resources allocated for audio playback
void MainComponent::releaseResources()
{
    player1.releaseResources();     // Release resources for player 1
    player2.releaseResources();     // Release resources for player 2

    mixerSource.releaseResources(); // Release resources for mixer source
}

//==============================================================================
// paint: Draws the background and other UI elements
void MainComponent::paint(juce::Graphics& g)
{
    // Fill background with a solid color
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

// resized: Arranges UI components when the window size changes
void MainComponent::resized()
{
    // Set bounds for GUI1 and GUI2 to divide the window into two halves
    GUI1.setBounds(0, 0, getWidth() / 2, getHeight());
    GUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
}
