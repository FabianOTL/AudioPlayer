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
    juce::ValueTree library;
    std::function<void(const MenuState)> changeState;

    ListMenu(juce::ValueTree vt) : library(vt) 
    {
        setSize(675, 300);

        // Button to add a new playlist 
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
        const unsigned short size = 85, gap = 25;
        auto area = getLocalBounds(); area.removeFromTop(gap);
        auto row = area.removeFromTop(size); row.removeFromLeft(gap);
        int count = 1;
        newPlaylistButton.setBounds(row.removeFromLeft(size));
   
        for(int i=0;i<playlistButtons.size();i++)
        {
            if(count%5 == 0) {count = 0; area.removeFromTop(gap); row = area.removeFromTop(size); }
            count++;

            row.removeFromLeft(gap);
            playlistButtons[i]->setBounds(row.removeFromLeft(size));
        }
    }

    void createPlaylists()
    {
        playlistButtons.clear();
        for(int i = 0;i<library.getNumChildren();i++)
        {
            std::unique_ptr<juce::ImageButton> playlistButton = std::make_unique<juce::ImageButton>("Button");
            juce::ValueTree playlistVT = library.getChild(i);
            juce::String imagePath = playlistVT.getPropertyAsValue(ID::PlaylistCover, nullptr).toString();
            juce::Image playlistImage = juce::ImageFileFormat::loadFrom(juce::File(imagePath));

            playlistButton->setImages(
                false,true,false,
                playlistImage, 1.0f, juce::Colours::transparentWhite,
                playlistImage, 1.0f, juce::Colours::transparentWhite,
                playlistImage, 1.0f, juce::Colours::transparentWhite);

            addAndMakeVisible(playlistButton.get());
            playlistButtons.push_back(std::move(playlistButton));
        }

        resized();
    }

private:
    std::vector<std::unique_ptr<juce::ImageButton>> playlistButtons;
    juce::ImageButton newPlaylistButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListMenu)
};
