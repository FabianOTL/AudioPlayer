#pragma once

#include <JuceHeader.h>
#include "RecentFiles.h" 

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::ChangeListener, public juce::SliderListener< juce::Slider>, public juce::Timer 
{
public:
    //==============================================================================

	enum TransportState
	{
		Stopped,
 		Stopping,
		Starting,
		Playing
	};
	
	TransportState state, stateBeforeDrag;
	juce::Label nameLabel;
	juce::TextButton playButton;
	juce::TextButton stopButton;
	juce::TextButton openButton;
	juce::Slider timeline, volumeSlider; 
	juce::AudioTransportSource transportSource;
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;	
	std::unique_ptr<juce::FileChooser> chooser;
	RecentFiles recentFiles;	

	void changeState(TransportState state);
	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();
	void loadFileInSource(juce::File);

    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
	void sliderValueChanged(juce::Slider* source) override;
	void sliderDragStarted(juce::Slider* source) override;
	void sliderDragEnded(juce::Slider* source) override;
	void timerCallback() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
