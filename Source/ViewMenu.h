/*
  ==============================================================================

    ViewMenu.h
    Created: 8 Sep 2026 6:03:22pm
    Author:  fabi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ViewMenu  : public juce::Component
{
public:

    juce::ImageComponent imageComponent;
    juce::Label playlistNameLabel;
    juce::TextButton backButton;

    std::function<void(const MenuState)> changeState;

    ViewMenu()
    {
        setSize(625, 300);

        backButton.setButtonText("Back"); 
        addAndMakeVisible(&backButton);
    }

    ~ViewMenu() override
    {

    }

    void build(juce::ValueTree playlist)
    {
        juce::String imagePath = playlist.getPropertyAsValue(ID::PlaylistCover, nullptr).toString();
        juce::Image image = juce::ImageFileFormat::loadFrom(juce::File(imagePath)); 
        imageComponent.setImage(image);
        imageComponent.setImagePlacement(juce::RectanglePlacement(64));
 
        addAndMakeVisible(&imageComponent);

        juce::String playlistName = playlist.getPropertyAsValue(ID::PlaylistName, nullptr).toString();
        playlistNameLabel.setText(playlistName, juce::NotificationType::dontSendNotification);
        playlistNameLabel.setJustificationType(juce::Justification(36));
        addAndMakeVisible(&playlistNameLabel);
    }

    void paint (juce::Graphics& g) override
    {

    }

    void resized() override
    {
        auto left = getLocalBounds().removeFromLeft(150);
        imageComponent.setBounds(left.removeFromTop(150));
        playlistNameLabel.setBounds(left.removeFromTop(50));
        backButton.setBounds(left.removeFromBottom(50));

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewMenu)
};
