# Install script for directory: /home/fabi/Dev/AudioPlayer/Include/JUCE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/fabi/Dev/AudioPlayer/Builds/build/Include/JUCE/modules/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/fabi/Dev/AudioPlayer/Builds/build/Include/JUCE/extras/Build/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-9.0.2" TYPE FILE FILES
    "/home/fabi/Dev/AudioPlayer/Builds/build/Include/JUCE/JUCEConfigVersion.cmake"
    "/home/fabi/Dev/AudioPlayer/Builds/build/Include/JUCE/JUCEConfig.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/FindCppwinrt.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/FindWebView2.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/FindWindowsMIDIServices.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/JUCECheckAtomic.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/JUCEHelperTargets.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/JUCEModuleSupport.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/JUCEUtils.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/JuceLV2Defines.h.in"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/LaunchScreen.storyboard"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/PIPAudioProcessor.cpp.in"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/PIPAudioProcessorWithARA.cpp.in"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/PIPComponent.cpp.in"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/PIPConsole.cpp.in"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/RecentFilesMenuTemplate.nib"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/UnityPluginGUIScript.cs.in"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/bundleplaceholder.mm"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/checkBundleSigning.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/copyDir.cmake"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/juce_LinuxSubprocessHelper.cpp"
    "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/juce_runtime_arch_detection.cpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-9.0.2" TYPE DIRECTORY FILES "/home/fabi/Dev/AudioPlayer/Include/JUCE/extras/Build/CMake/juce_vst3_helper")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/fabi/Dev/AudioPlayer/Builds/build/Include/JUCE/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
