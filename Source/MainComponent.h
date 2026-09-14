#pragma once

#include <JuceHeader.h>
#include "RecentFiles.h" 
#include "Playlists.h"
#include "AudioControl.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent{
public:

	juce::Label nameLabel;
	juce::TextButton openButton;
	juce::AudioTransportSource transportSource;
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;	
	std::unique_ptr<juce::FileChooser> chooser;
	RecentFiles recentFiles;	
    Playlists playlists;
    AudioControl audioControl;

	void openButtonClicked();
	void loadFileInSource(juce::File);

    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
