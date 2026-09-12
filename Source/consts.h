/*
  ==============================================================================

    enums.h
    Created: 8 Sep 2026 7:05:32pm
    Author:  fabi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <iostream>

enum MenuState
{
    List,
    Add,
    View
};

namespace ID 
{
    inline juce::Identifier Library("lib");
    inline juce::Identifier Playlist("playlist");
    inline juce::Identifier PlaylistName("playlistName");
    inline juce::Identifier PlaylistCover("cover");
    inline juce::Identifier Track("track");
    inline juce::Identifier TrackPath("trackPath");
};

inline juce::String appData = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getFullPathName();
inline juce::String projectDir = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentExecutableFile).getParentDirectory().getParentDirectory().getParentDirectory().getParentDirectory().getFullPathName();

