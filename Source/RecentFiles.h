/*
  ==============================================================================

    RecentFiles.h
    Created: 17 Aug 2026 7:08:33pm
    Author:  fabi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <queue>
#include <set>

#include "consts.h"

//==============================================================================
/*
*/

class RecentFiles  : public juce::Component
{
public:
    RecentFiles();
    ~RecentFiles() override;

    void paint (juce::Graphics&) override;
    void resized() override;

	void addFile(juce::File file);
	std::function<void(const juce::File&)> loadFile;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RecentFiles)

	const int MaxSize = 10;
	juce::Label title;

	std::queue<juce::File> filesQ;
	std::set<juce::File> filesSet; 
	std::vector<std::unique_ptr<juce::TextButton>> buttons;	
};
