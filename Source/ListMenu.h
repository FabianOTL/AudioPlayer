/*
  ==============================================================================

    ListMenu.h
    Created: 8 Sep 2026 6:02:44pm
    Author:  fabi

  ==============================================================================
*/

#pragma once

#include "consts.h"

class ListMenu  : public juce::Component
{
public:
    std::function<void(const MenuState)> changeState;

    ListMenu()  
    {
        setSize(675, 300);

        // Button to add a new playlist 
        newPlaylistButton.setBounds(25, 25, 50, 50);
        juce::Image newPlaylistImage = juce::ImageFileFormat::loadFrom(juce::File(projectDir + "/Resources/add.png"));

        newPlaylistButton.setImages(
        false,true,false,
        newPlaylistImage, 1.0f, juce::Colours::transparentWhite,
        newPlaylistImage, 1.0f, juce::Colours::transparentWhite,
        newPlaylistImage, 1.0f, juce::Colours::transparentWhite);

        newPlaylistButton.onClick = [this] () {
            changeState(MenuState::Add);
        };

        addAndMakeVisible(&newPlaylistButton);
    }

    ~ListMenu() override
    {
    }

    void paint (juce::Graphics& g) override
    {
    }

    void resized() override
    {
    }

private:
    juce::ImageButton newPlaylistButton;
    juce::Component listView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListMenu)
};
