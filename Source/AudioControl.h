/*
  ==============================================================================

    AudioControl.h
    Created: 14 Sep 2026 8:34:37am
    Author:  fabi

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include <stack>
//==============================================================================

class Viewer : public juce::Component
{
public:

    void paint(juce::Graphics& g) 
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }
};

class Queue 
{
public:

    std::function<void(juce::File)> loadFile;

    Queue()
    {
        build();
    }

    ~Queue()
    {
    }

    void add(juce::ValueTree track)
    {
        juce::String path {track.getPropertyAsValue(ID::TrackPath, nullptr).toString()};
        q.push(path);

        auto label = std::make_unique<juce::Label>("Label", juce::File(path).getFileName());
        viewerLabels.push_back(std::move(label));

        build();
    }
    
    void nextTrack()
    {
        if(q.empty()) 
        {
            return;
        }

        juce::String path = q.front();
        q.pop();
        viewerLabels.erase(viewerLabels.begin());

        loadFile(juce::File(path));

        build();
    } 

    void clear()
    {
        while(!q.empty()){
            q.pop();
            viewerLabels.pop_back();
        }
    }

    bool empty()
    {
        return q.empty();
    }

    Viewer* getViewer()
    {
        return &viewer;
    }

private:
    
    void build()
    {
        viewer.removeAllChildren();

        if(empty())
            return;

        auto area = viewer.getLocalBounds();

        clearButton.setButtonText("Clear");
        clearButton.onClick = [this] ()
        {
            clear();
            build();
        };
        clearButton.setBounds(area.removeFromTop(50));
        viewer.addAndMakeVisible(&clearButton);

        for(auto& it: viewerLabels)
        {
            it -> setBounds(area.removeFromTop(50));
            viewer.addAndMakeVisible(it.get());
        }
    }
    std::queue<juce::String> q;
    Viewer viewer;
    juce::TextButton clearButton;
    std::vector<std::unique_ptr<juce::Label>> viewerLabels;
};

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
    juce::TextButton playButton, nextButton, prevButton;
    juce::Slider timeline, volumeSlider;
    juce::AudioTransportSource* transportSource;
    std::stack<juce::String> history;
    std::function<void(juce::File)> loadFile;
    Queue* playQ, *userQ;

    AudioControl(juce::AudioTransportSource* ts)
    {
        setSize(800, 200);

        transportSource = ts;
        transportSource->addChangeListener(this);

        playQ = new Queue;
        userQ = new Queue;

        // Button to play the loaded file 
        addAndMakeVisible(&playButton);
        playButton.onClick = [this] { 
            if(state == Stopped)
                changeState(Starting);
            else 
                changeState(Stopping);
        } ;
        playButton.setBounds(300, 70, 40, 40);
        playButton.setButtonText("||");
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);	
        playButton.setEnabled(false);	
       
        // Button to play the next song 
        addAndMakeVisible(&nextButton);
        nextButton.onClick = [this] () {
           if(userQ->empty())
               playQ->nextTrack();
           else 
               userQ->nextTrack();
        };
        nextButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);	
        nextButton.setEnabled(false);

        nextButton.setBounds(350, 70, 40, 40);
        nextButton.setButtonText(">");

        // Play previous song 
        addAndMakeVisible(&prevButton);
        prevButton.onClick = [this] () {
            if(history.empty()) return;
            loadFile(juce::File(history.top())); // here we add the prev song to history, which we don't want 
            history.pop();                       // so we pop the stack two times 
            history.pop();
            if(history.empty())
                prevButton.setEnabled(false);
        };
        prevButton.setBounds(250, 70, 40, 40);
        prevButton.setButtonText("<");
        prevButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);	
        prevButton.setEnabled(false);


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
        delete playQ;
        delete userQ;
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
            case Starting: 
                transportSource->start();
                break;
            case Stopping:
                transportSource->stop();	
                break;
        }
    }

    void sliderValueChanged(juce::Slider* source)
    {
        if(source == &timeline && timeline.isMouseButtonDown())
            transportSource->setPosition(timeline.getValue());

        if(source == &volumeSlider) 
            transportSource->setGain(volumeSlider.getValue());
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
        nextButton.setEnabled(!playQ->empty() || !userQ->empty());
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) 
    {
        if(source == transportSource)
        {
            if(transportSource->isPlaying())
                changeState(Playing);
            else 
                changeState(Stopped);

            if(transportSource->hasStreamFinished())
                playQ->nextTrack();

            nextButton.setEnabled(!playQ->empty());
        }	
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioControl)
};
