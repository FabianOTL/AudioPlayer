/*
  ==============================================================================

    AddMenu.h
    Created: 8 Sep 2026 6:03:02pm
    Author:  fabi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AddMenu  : public juce::Component
{
public:

    juce::TextButton submit, pictureButton, backButton;
    juce::Label menuTitle, nameLabel, pictureBox; 
    juce::TextEditor nameBox;

    std::unique_ptr<juce::FileChooser> chooser;
    AddMenu(juce::ValueTree library)
    {
        setSize(675, 300);

        // title
        menuTitle.setText("Add Playlist", juce::NotificationType::dontSendNotification);
        addAndMakeVisible(&menuTitle);

        // Playlist name textbox and label 

        nameBox.setBorder(juce::BorderSize(5));
        nameBox.setReadOnly(false);
        nameBox.setMouseClickGrabsKeyboardFocus(true);
        addAndMakeVisible(&nameBox);

        nameLabel.setText("Playlist name: ",  juce::NotificationType::dontSendNotification);
        addAndMakeVisible(&nameLabel);

        // Picture 
        
        pictureBox.setText(projectDir + "/Resources/default.jpeg",  juce::NotificationType::dontSendNotification);
        addAndMakeVisible(&pictureBox);
        pictureButton.setButtonText("Select cover");
        pictureButton.setWantsKeyboardFocus(false);
        addAndMakeVisible(&pictureButton);

        //Back button 
        backButton.setButtonText("Back");
        addAndMakeVisible(&backButton);

        pictureButton.onClick = [this]  
        {
            chooser = std::make_unique<juce::FileChooser> ("Select a picture", juce::File {}, "*.jpg,*.png");
            auto chooserFlag = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
            

            auto open = [this] (const juce::FileChooser& fc)
            {
                auto file = fc.getResult();

                if(file != juce::File {})
                {
                    pictureBox.setText(file.getFullPathName(), juce::NotificationType::dontSendNotification);
                }
            };
            chooser -> launchAsync(chooserFlag, open);
        };

        // Submit 

        submit.onClick = [this, library] () mutable 
        {
            juce::ValueTree playlist(ID::Playlist);
            playlist.setProperty(ID::PlaylistName, nameBox.getText(), nullptr);
            playlist.setProperty(ID::PlaylistCover, pictureBox.getText(), nullptr);
            library.appendChild(playlist, nullptr);
            
            if(auto xml = library.createXml()){
                xml -> writeTo(juce::File(appData + "/library.xml"));

                juce::AlertWindow::showMessageBoxAsync(
                    juce::AlertWindow::InfoIcon,
                    "Success",                        // Title
                    "The playlist has been created!", // Message
                    "OK"                              // Button text
                );
            }
            else 
            {
                juce::AlertWindow::showMessageBoxAsync(
                    juce::AlertWindow::InfoIcon,
                    "Error",                      // Title
                    "There was an error!",        // Message
                    "OK"                          // Button text
                );
            }
        };
        submit.setWantsKeyboardFocus(false);
        submit.setButtonText("Submit");
        addAndMakeVisible(&submit);
    }

    ~AddMenu() override
    {
    }

    void paint (juce::Graphics& g) override
    { 
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component   
    }

    void resized() override
    {
        auto left = getLocalBounds();
        auto right = left;

        right.removeFromLeft(150);
        right.removeFromTop(50);

        menuTitle.setBounds(left.removeFromTop(50));
        nameLabel.setBounds(left.removeFromTop(40));
        nameBox.setBounds(right.removeFromTop(40));
        
        pictureBox.setBounds(left.removeFromTop(40));
        pictureButton.setBounds(right.removeFromTop(40).removeFromRight(300).removeFromLeft(100));

        submit.setBounds(left.removeFromTop(40).removeFromLeft(100));
        backButton.setBounds(left.removeFromLeft(100).removeFromBottom(40));
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddMenu)
};
