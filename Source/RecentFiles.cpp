/*
  ==============================================================================

    RecentFiles.cpp
    Created: 17 Aug 2026 7:08:33pm
    Author:  fabi

  ==============================================================================
*/

#include <JuceHeader.h>
#include <fstream>
#include "RecentFiles.h"

//==============================================================================
RecentFiles::RecentFiles()
{
	setSize(150, 600);
	title.setText("Recent Files", juce::dontSendNotification);
	addAndMakeVisible(&title);

	// Read the recents.txt file and add the files to the list

	std::ifstream fin(appData.toStdString() + "/recents.txt");

	char path[1024];

	while(fin.getline(path, 1024))
	{
		try{
			juce::File file = juce::File(juce::String(path));
			addFile(file);
		}
		catch(int err){
			std::cout << err << std::endl; 
		} 
	} 

	fin.close(); 
}

RecentFiles::~RecentFiles()
{
    // Save the changes made to recents.txt
	std::ofstream fout(appData.toStdString() + "/recents.txt");
	
	while(filesQ.size())
	{
		fout << filesQ.front().getFullPathName() << '\n';
		filesQ.pop();
	}
}

// Adds a new button and insert file into queue 
void RecentFiles::addFile(juce::File file) 
{
	if(filesSet.count(file) == 0 && file.existsAsFile()) {
		filesQ.push(file);
		filesSet.insert(file);

		auto& but = buttons.emplace_back(std::make_unique<juce::TextButton>(file.getFileName()));;
		addAndMakeVisible(but.get());

		but.get() -> onClick = [this, file] { 
			loadFile(file);	
		};

		resized();
	}

    // Remove a file from the queue when there are too many
	if(filesQ.size() > MaxSize) {	
        
        filesSet.erase(filesQ.front());
        filesQ.pop();
    }
}

void RecentFiles::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
}

// Arrange the buttons 
void RecentFiles::resized()
{
	auto area = getLocalBounds();
	auto itemH = 50;

	title.setBounds(area.removeFromTop(itemH));
	for(auto& it : buttons) 
		it.get() -> setBounds(area.removeFromTop(itemH));
	

}
