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
    addAndMakeVisible(&listMenu);
    addChildComponent(&addMenu);
    addChildComponent(&viewMenu);

    listMenu.createPlaylists();

    addMenu.backButton.onClick = [this] () {
        changeMenuState(MenuState::List);
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
}

Playlists::~Playlists()
{
}

void Playlists::paint (juce::Graphics& g)
{ 

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void Playlists::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

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
            listMenu.createPlaylists();
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
