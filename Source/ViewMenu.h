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
    juce::TextButton backButton, addTrackButton;
    juce::ValueTree playlistVT;
    std::unique_ptr<juce::FileChooser> chooser;

    std::function<void(const MenuState)> changeState;
    std::function<void(const juce::File&)> loadFile;

    ViewMenu()
    {
        setSize(625, 300);

        backButton.setButtonText("Back"); 
        addAndMakeVisible(&backButton);

        addTrackButton.setButtonText("Add Track");
        addTrackButton.onClick = [this] ()
        {
            addTrack();
        };
        addAndMakeVisible(&addTrackButton);
    }

    ~ViewMenu() override
    {

    }

    void build(juce::ValueTree playlist)
    {
        playlistVT = playlist;

        // Clean the tracks vector 
        
        while(tracks.size())
            tracks.pop_back();

        // The left side of the menu (cover, title and back button)
        juce::String imagePath = playlist.getPropertyAsValue(ID::PlaylistCover, nullptr).toString();
        juce::Image image = juce::ImageFileFormat::loadFrom(juce::File(imagePath)); 
        imageComponent.setImage(image);
        imageComponent.setImagePlacement(juce::RectanglePlacement(64));
 
        addAndMakeVisible(&imageComponent);

        juce::String playlistName = playlist.getPropertyAsValue(ID::PlaylistName, nullptr).toString();
        playlistNameLabel.setText(playlistName, juce::NotificationType::dontSendNotification);
        playlistNameLabel.setJustificationType(juce::Justification(36));
        addAndMakeVisible(&playlistNameLabel);

        // The right side 
        
        unsigned int noTracks = playlist.getNumChildren();
        for(int i = 0;i<noTracks;i++)
        {
            juce::String trackPath = playlist.getChild(i).getPropertyAsValue(ID::TrackPath, nullptr).toString();

            auto btn = std::make_unique<juce::TextButton>("Button");
            btn->setButtonText(juce::File(trackPath).getFileName());
            btn->onClick = [this, trackPath] ()
            {
                loadFile(juce::File(trackPath));
            };
            addAndMakeVisible(btn.get());

            auto del = std::make_unique<juce::TextButton>("Button");
            del -> setButtonText("Delete");
            auto toDelete =  playlistVT.getChild(i);
            del -> onClick = [this, toDelete] () {
                deleteTrack(toDelete);
            };
            addAndMakeVisible(del.get());

            tracks.push_back(std::move(btn));
            deletes.push_back(std::move(del));
        }

        resized();
    }

    void addTrack()
    {
        chooser = std::make_unique<juce::FileChooser> ("Select audio file...", juce::File {}, "*.wav;*.mp3");
        auto chooserFlag = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        auto open = [this] (const juce::FileChooser& fc) 
        {	
            auto file = fc.getResult();

            if(file != juce::File {})
            {
                juce::ValueTree trackVT(ID::Track);
                trackVT.setProperty(ID::TrackPath, file.getFullPathName(), nullptr);

                auto btn = std::make_unique<juce::TextButton>("Button");
                btn->setButtonText(file.getFileName());
                btn->onClick=[this, file] (){
                    loadFile(file);
                };
                addAndMakeVisible(btn.get());

                auto del = std::make_unique<juce::TextButton>("Button");
                del -> setButtonText("Delete");
                del -> onClick = [this, trackVT] ()
                {
                    deleteTrack(trackVT);
                };
                addAndMakeVisible(del.get());

                playlistVT.appendChild(trackVT, nullptr);  
                tracks.push_back(std::move(btn));
                deletes.push_back(std::move(del));

                if(auto xml = playlistVT.getParent().createXml())
                    xml -> writeTo(juce::File(appData + "/library.xml"));

                resized();
            }
        };

        chooser -> launchAsync(chooserFlag, open);  
    }

    void paint (juce::Graphics& g) override
    {

    }

    void resized() override
    {
        auto right = getLocalBounds();
        auto left = right.removeFromLeft(150);
        
        //left side
        imageComponent.setBounds(left.removeFromTop(150));
        playlistNameLabel.setBounds(left.removeFromTop(50));
        addTrackButton.setBounds(left.removeFromTop(50));
        backButton.setBounds(left.removeFromBottom(50));

        //right side
        for(int i=0;i<tracks.size();i++){
            auto row = right.removeFromTop(50);
            tracks[i]->setBounds(row.removeFromLeft(300));
            deletes[i] -> setBounds(row.removeFromLeft(100));
        }
    }

    void deleteTrack(juce::ValueTree toDelete)
    {
        int index = playlistVT.indexOf(toDelete);

        playlistVT.removeChild(index, nullptr);
        tracks.erase(tracks.begin() + index);
        deletes.erase(deletes.begin() + index);

        if(auto xml = playlistVT.getParent().createXml())
            xml -> writeTo(juce::File(appData + "/library.xml"));

        resized();
    }
private:
    std::vector<std::unique_ptr<juce::TextButton>> tracks, deletes;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewMenu)
};
