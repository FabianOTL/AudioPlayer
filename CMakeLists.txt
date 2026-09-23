cmake_minimum_required(VERSION 3.22)
project(AudioPlayer VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ignore warning 
if(MSVC)
    add_compile_options(/W0)
else()
    add_compile_options(-w)
endif()

add_subdirectory(Include/JUCE)

juce_add_gui_app(AudioPlayer
    PRODUCT_NAME "AudioPlayer"
    VERSION "1.0.0"
)

target_sources(AudioPlayer
    PRIVATE
        Source/Main.cpp
        Source/MainComponent.h
        Source/MainComponent.cpp
        Source/AddMenu.h 
        Source/AudioControl.h 
        Source/consts.h 
        Source/ListMenu.h 
        Source/Playlists.cpp 
        Source/Playlists.h 
        Source/RecentFiles.cpp 
        Source/RecentFiles.h 
        Source/ViewMenu.h
        Source/JuceHeader.h
)

target_link_libraries(AudioPlayer
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
