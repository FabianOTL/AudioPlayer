#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent() : state(TransportState::Stopped),
								 stateBeforeDrag(TransportState::Stopped),
								 recentFiles()
{
    setSize (1000, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

	// inits
	
	transportSource.addChangeListener(this);
	formatManager.registerBasicFormats();

	addAndMakeVisible(&nameLabel);
	nameLabel.setBounds(150, 25, 700, 40);

    // Button to open an audio file
    addAndMakeVisible(&openButton);
	openButton.onClick = [this] { openButtonClicked(); };
    openButton.setBounds(25, 25, 100, 40);
    openButton.setButtonText("Open");
    openButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);

    // Button to play the loaded file 
	addAndMakeVisible(&playButton);
	playButton.onClick = [this] { playButtonClicked(); } ;
	playButton.setBounds(250, 500, 100, 40);
	playButton.setButtonText("Play");
	playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);	
	playButton.setEnabled(false);	

    // Button to pause the loaded file 
	addAndMakeVisible(&stopButton);
	stopButton.onClick = [this] { stopButtonClicked(); } ; 
	stopButton.setBounds(400, 500, 100, 40);
	stopButton.setButtonText("Stop");
	stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
	stopButton.setEnabled(false);

    // Timeline 
	timeline.addListener(this);
	timeline.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	timeline.setRange(0.0f, 1.0f);
	timeline.setValue(0.0f);
	timeline.setBounds(100, 450, 500, 50);
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
	volumeSlider.setBounds(600, 450, 125, 50);
    volumeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 50, 50);
	addAndMakeVisible(&volumeSlider);
	
    // Add the recent files menu 
	recentFiles.setBounds(800, 0, 200, 600);
	recentFiles.loadFile = [this] (const juce::File& file) // make RecentFiles able to load a file into our AudioSource
	{
		loadFileInSource(file);	
	};
	addAndMakeVisible(&recentFiles); 

	startTimerHz(30); // Used to update the timeline GUI

    // Playlists Menu 
    playlists.setBounds(100, 100, 675, 300);
    addAndMakeVisible(&playlists);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}


void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) 
{
	if(source == &transportSource)
	{
		if(transportSource.isPlaying())
			changeState(Playing);
		else 
			changeState(Stopped);
	}	
}

void MainComponent::sliderValueChanged(juce::Slider* source)
{
	if(source == &timeline && timeline.isMouseButtonDown())
	{
		transportSource.setPosition(timeline.getValue());
	}
	
	if(source == &volumeSlider) 
	{
		transportSource.setGain(volumeSlider.getValue());
	}

}

void MainComponent::sliderDragStarted(juce::Slider* source)
{	
	if(source == &timeline) {
		stateBeforeDrag = state; 
		transportSource.stop(); // pause the sound while the user is changing the timeline
	}
}

void MainComponent::sliderDragEnded(juce::Slider* source)
{
	if(source == &timeline && stateBeforeDrag == TransportState::Playing)
		transportSource.start(); // play the sound again when the user changed the timeline
}

void MainComponent::timerCallback()
{
	if(state == TransportState::Playing)
		timeline.setValue(transportSource.getCurrentPosition()); // update the timeline when the audio is playing 
}

void MainComponent::changeState(TransportState newState)
{
	if(state == newState) return;

	state = newState;

	switch(newState)
	{
		case Playing: 
			stopButton.setEnabled(true);
			break;
		case Starting: 
			transportSource.start();
			playButton.setEnabled(false); 
			break;
		case Stopped:
			playButton.setEnabled(true);
			stopButton.setEnabled(false);
			break;
		case Stopping:
			transportSource.stop();	
			break;
	}

}

// takes a file and load it into AudioSource 
void MainComponent::loadFileInSource(juce::File file)
{
	auto* reader = formatManager.createReaderFor(file);
    if(reader != nullptr) 
	{
		auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		playButton.setEnabled(true);
		readerSource.reset(newSource.release());

		//adjust the timeline 
		timeline.setRange(0.0f, transportSource.getLengthInSeconds());
		timeline.setValue(0.0f);
		//show file name
		const juce::String fileName = file.getFileName();
     	nameLabel.setText(fileName, juce::NotificationType::dontSendNotification);
		transportSource.sendChangeMessage(); // for when the change accures outside the main comp. 
	}       
}

// opens a file and loads it in the AudioSource 
void MainComponent::openButtonClicked()
{
	chooser = std::make_unique<juce::FileChooser> ("Select wav file...", juce::File {}, "*.wav;*.mp3");
    auto chooserFlag = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

	auto open = [this] (const juce::FileChooser& fc) 
	{	
		auto file = fc.getResult();

		if(file != juce::File {})
		{
			recentFiles.addFile(file);
			loadFileInSource(file);	
		}
	};

	chooser -> launchAsync(chooserFlag, open);  
}

void MainComponent::playButtonClicked()
{
	changeState(Starting);
}

void MainComponent::stopButtonClicked()
{
	changeState(Stopping);
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
   	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate); 
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
	if(readerSource.get() == nullptr) 
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}
	
	transportSource.getNextAudioBlock(bufferToFill);   
}

void MainComponent::releaseResources()
{
 	transportSource.releaseResources();  
} 

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    
}
