#include "MainComponent.h"

//==============================================================================

MainComponent::MainComponent() : audioControl(&transportSource) 
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
	
	formatManager.registerBasicFormats();

	addAndMakeVisible(&nameLabel);
	nameLabel.setBounds(150, 25, 700, 40);

 
    // Button to open an audio file
    addAndMakeVisible(&openButton);
    openButton.onClick = [this] { openButtonClicked(); };
    openButton.setBounds(25, 25, 100, 40);
    openButton.setButtonText("Open");
    openButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);

    // Add the recent files menu 
	recentFiles.setBounds(800, 0, 200, 600);
	recentFiles.loadFile = [this] (const juce::File& file) // make RecentFiles able to load a file into our AudioSource
	{
		loadFileInSource(file);	
	};
	addAndMakeVisible(&recentFiles); 


    // Playlists Menu 
    playlists.setBounds(100, 100, 675, 300);
    playlists.viewMenu.loadFile = [this] (const juce::File& file)
    {
       loadFileInSource(file); 
    };
    addAndMakeVisible(&playlists);
    
	// Audio Control 
    
    audioControl.setBounds(100, 400, 800, 200);
    addAndMakeVisible(&audioControl);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}


// takes a file and load it into AudioSource 
void MainComponent::loadFileInSource(juce::File file)
{
	auto* reader = formatManager.createReaderFor(file);
    if(reader != nullptr) 
	{
        recentFiles.addFile(file);

		auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
		transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		audioControl.playButton.setEnabled(true);
		readerSource.reset(newSource.release());

		//adjust the timeline 
		audioControl.timeline.setRange(0.0f, transportSource.getLengthInSeconds());
		audioControl.timeline.setValue(0.0f);
		//show file name
		const juce::String fileName = file.getFileName();
     	nameLabel.setText(fileName, juce::NotificationType::dontSendNotification);
		transportSource.sendChangeMessage(); // for when the change accures outside the main comp. 
	}       
}

// opens a file and loads it in the AudioSource 
void MainComponent::openButtonClicked()
{
	chooser = std::make_unique<juce::FileChooser> ("Select audio file...", juce::File {}, "*.wav;*.mp3");
    auto chooserFlag = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

	auto open = [this] (const juce::FileChooser& fc) 
	{	
		auto file = fc.getResult();

		if(file != juce::File {})
		{
			loadFileInSource(file);	
		}
	};

	chooser -> launchAsync(chooserFlag, open);  
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
