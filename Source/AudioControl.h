/*
  ==============================================================================

    AudioControl.h
    Created: 14 Sep 2026 8:34:37am
    Author:  fabi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class AudioControl  : public juce::Component, public juce::SliderListener< juce::Slider>, public juce::ChangeListener, public juce::Timer 
{
public: 
	enum TransportState
	{
		Stopped,
 		Stopping,
		Starting,
		Playing
	};
	
	TransportState state, stateBeforeDrag;
    juce::TextButton playButton, stopButton;
    juce::Slider timeline, volumeSlider;
    juce::AudioTransportSource* transportSource;

    AudioControl(juce::AudioTransportSource* ts)
    {
        setSize(800, 200);

        transportSource = ts;
        transportSource->addChangeListener(this);

        // Button to play the loaded file 
        addAndMakeVisible(&playButton);
        playButton.onClick = [this] { changeState(Starting); } ;
        playButton.setBounds(200, 75, 100, 40);
        playButton.setButtonText("Play");
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);	
        playButton.setEnabled(false);	
       
        // Button to pause the loaded file 
        addAndMakeVisible(&stopButton);
        stopButton.onClick = [this] { changeState(Stopping); } ; 
        stopButton.setBounds(350, 75, 100, 40);
        stopButton.setButtonText("Stop");
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        stopButton.setEnabled(false);

        // Timeline 
        timeline.addListener(this);
        timeline.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        timeline.setRange(0.0f, 1.0f);
        timeline.setValue(0.0f);
        timeline.setBounds(50, 0, 500, 50);
        timeline.setNumDecimalPlacesToDisplay(2);
        addAndMakeVisible(&timeline);
        timeline.textFromValueFunction = [](double value) 
        {
            int totalSeconds = static_cast<int>(value);
            int h = totalSeconds / 3600;
            int m = totalSeconds % 3600 / 60;
            int s = totalSeconds % 60;

            if (h > 0)
                return juce::String::formatted("%02d:%02d:%02d", h, m, s);
            return juce::String::formatted("%02d:%02d", m, s); 
        };
    
        // Volume slider 
        volumeSlider.addListener(this);
        volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        volumeSlider.setRange(0.0f, 1.0f);
        volumeSlider.setValue(0.5f);
        volumeSlider.setBounds(550, 0, 125, 50);
        volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 50, 50);
        addAndMakeVisible(&volumeSlider);

        startTimerHz(30); // Used to update the timeline GUI
    }

    ~AudioControl() override
    {
    }

    void paint (juce::Graphics& g) override
    {
    
    }

    void resized() override
    {

    }

    void changeState(TransportState newState)
    {
        if(state == newState) return;

        state = newState;

        switch(newState)
        {
            case Playing: 
                stopButton.setEnabled(true);
                break;
            case Starting: 
                transportSource->start();
                playButton.setEnabled(false); 
                break;
            case Stopped:
                playButton.setEnabled(true);
                stopButton.setEnabled(false);
                break;
            case Stopping:
                transportSource->stop();	
                break;
        }

    }


    void sliderValueChanged(juce::Slider* source)
    {
        if(source == &timeline && timeline.isMouseButtonDown())
        {
            transportSource->setPosition(timeline.getValue());

        }
        
        if(source == &volumeSlider) 
        {
            transportSource->setGain(volumeSlider.getValue());
        }

    }

    void sliderDragStarted(juce::Slider* source)
    {	
        if(source == &timeline) {
            stateBeforeDrag = state; 
            transportSource->stop(); // pause the sound while the user is changing the timeline
        }
    }

    void sliderDragEnded(juce::Slider* source)
    {
        if(source == &timeline && stateBeforeDrag == TransportState::Playing)
            transportSource->start(); // play the sound again when the user changed the timeline
    }

    void timerCallback()
    {
        if(state == TransportState::Playing)
            timeline.setValue(transportSource->getCurrentPosition()); // update the timeline when the audio is playing 
    }


    void changeListenerCallback(juce::ChangeBroadcaster* source) 
    {
        if(source == transportSource)
        {
            if(transportSource->isPlaying())
                changeState(Playing);
            else 
                changeState(Stopped);
        }	
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioControl)
};
