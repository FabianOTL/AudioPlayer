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
    std::function<void(const MenuState, juce::ValueTree)> changeStateView;

    ListMenu(juce::ValueTree vt) : library(vt) 
    {
        setSize(675, 300);

        // Button to add a new playlist 
        juce::Image newPlaylistImage = juce::ImageFileFormat::loadFrom(juce::File(projectDir + "/Resources/add.png"));

        auto newPlaylistButton = std::make_unique<juce::ImageButton>("Button");
        newPlaylistButton->setImages(
        false,true,false,
        newPlaylistImage, 1.0f, juce::Colours::transparentWhite,
        newPlaylistImage, 1.0f, juce::Colours::transparentWhite,
        newPlaylistImage, 1.0f, juce::Colours::transparentWhite);

        newPlaylistButton->onClick = [this] () {
            changeState(MenuState::Add);
        };

        addAndMakeVisible(newPlaylistButton.get());
        playlistButtons.push_back(std::move(newPlaylistButton));
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
        auto area = getLocalBounds();
        auto row = area.removeFromTop(gap);
        auto labelRow = area;
        int count = 0;
   
        for(int i=0;i<playlistButtons.size();i++)
        {
            if(count%5 == 0) {count = 0; row = area.removeFromTop(size); labelRow = area.removeFromTop(gap); }
                count++;

            row.removeFromLeft(gap);
            labelRow.removeFromLeft(gap);
            playlistButtons[i]->setBounds(row.removeFromLeft(size));
            labels[i]->setBounds(labelRow.removeFromLeft(size));
        }
    }

    void createPlaylists()
    {
        // Save new playlist button
        auto newPlaylistButton = std::move(playlistButtons[0]);
        playlistButtons.clear();
        playlistButtons.push_back(std::move(newPlaylistButton));

        labels.clear();

        //Add the label for new playlist button 
        auto label = std::make_unique<juce::Label>("Label","New Playlist");
        label->setJustificationType(juce::Justification(36));
        addAndMakeVisible(label.get());
        labels.push_back(std::move(label));

        // Add the other buttons and labels 
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

            playlistButton->onClick = [this, playlistVT] () {
                changeStateView(MenuState::View, playlistVT);
            };

            addAndMakeVisible(playlistButton.get());
            playlistButtons.push_back(std::move(playlistButton));

            juce::String labelText = playlistVT.getPropertyAsValue(ID::PlaylistName, nullptr).toString();
            auto label = std::make_unique<juce::Label>("Label",labelText);
            label->setJustificationType(juce::Justification(36));
            addAndMakeVisible(label.get());
            labels.push_back(std::move(label));
        }

        resized();
    }

private:
    std::vector<std::unique_ptr<juce::ImageButton>> playlistButtons; 
    std::vector<std::unique_ptr<juce::Label>> labels;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListMenu)
};
