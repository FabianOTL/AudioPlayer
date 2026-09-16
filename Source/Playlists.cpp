/*
  ==============================================================================

    Playlists.cpp
    Created: 7 Sep 2026 12:33:21pm
    Author:  fabi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Playlists.h"

Playlists::Playlists() : menuState(List), library(ID::Library), addMenu(library), listMenu(library)
{
    setSize(675, 300);

    // Import/Create the value tree 

    std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(juce::File(appData + "/library.xml"));

    if(xml == nullptr)
    {
        // Create the xml file
        if(auto xmlNew = library.createXml())
            xmlNew -> writeTo(juce::File(appData + "/library.xml"));
        xml = juce::XmlDocument::parse(juce::File(appData + "/library.xml"));
    }

    auto loadedTree = juce::ValueTree::fromXml(*xml);
    library.copyPropertiesAndChildrenFrom(loadedTree, nullptr);

    // Add the different menus 

    playlistsViewPort.setViewedComponent(&listMenu, false);
    playlistsViewPort.setScrollBarsShown(true, false); // Vertical enabled, Horizontal disabled
    playlistsViewPort.setSingleStepSizes(0, 20);      // Scroll step size in pixels
    addAndMakeVisible(playlistsViewPort);

    //addAndMakeVisible(&listMenu);
    addChildComponent(&addMenu);
    addChildComponent(&viewMenu);

    listMenu.createPlaylists();

    addMenu.backButton.onClick = [this] () {
        changeMenuState(MenuState::List);
    };

    addMenu.addPlaylistButton = [this] (juce::ValueTree playlist)
    {
        listMenu.addPlaylistButton(playlist);
        changeMenuState(MenuState::List);
        listMenu.resized();
    };

    listMenu.changeState = [this] (const MenuState newMenuState) {
        changeMenuState(newMenuState);
    };

    listMenu.changeStateView = [this] (const MenuState newMenuState, juce::ValueTree vt)
    {
        changeMenuState(newMenuState, vt);
    };

    viewMenu.backButton.onClick = [this] ()
    {
        changeMenuState(MenuState::List);
    };

    viewMenu.deletePlaylistButton.onClick = [this] ()
    {
        changeMenuState(MenuState::List);
        listMenu.deletePlaylist();
    };
}

Playlists::~Playlists()
{
}

void Playlists::paintOverChildren (juce::Graphics& g)
{ 
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void Playlists::resized()
{
    playlistsViewPort.setBounds(getLocalBounds());
}

void Playlists::changeMenuState(MenuState newMenuState, juce::ValueTree vt)
{
    if(newMenuState == menuState) return;

    switch(menuState)
    {
        case MenuState::List: 
            listMenu.setVisible(false);
            break;

        case MenuState::Add:
            addMenu.reset();
            addMenu.setVisible(false);
            break;

        case MenuState::View: 
            viewMenu.setVisible(false);
            break;

    }

    menuState = newMenuState;

    switch(menuState)
    {
        case MenuState::List:
            if(!listMenu.isLoaded()){
                listMenu.createPlaylists();
                listMenu.setLoaded(true);
            }
            listMenu.setVisible(true);
            break;

        case MenuState::Add: 
            addMenu.setVisible(true);
            break;

        case MenuState::View: 
            viewMenu.build(vt);
            viewMenu.setVisible(true);
            break;
    }

}
