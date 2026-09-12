/*
  ==============================================================================

    Playlists.h
    Created: 7 Sep 2026 12:33:21pm
    Author:  fabi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>

#include "ListMenu.h"
#include "AddMenu.h"
#include "ViewMenu.h"
#include "consts.h"

class Playlists  : public juce::Component
{
public:
    juce::ValueTree library;
    ListMenu listMenu;
    AddMenu addMenu;
    ViewMenu viewMenu;
    MenuState menuState; 

    void changeMenuState(MenuState newState, juce::ValueTree vt=juce::ValueTree()); 
    Playlists();
    ~Playlists() override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Playlists)
};
