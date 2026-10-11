![](https://github.com/FabianOTL/AudioPlayer/blob/main/Resources/Preview.gif)


What is this?
------------- 
This is an audio player made in C++ using the JUCE library used to play local audio files and store them in playlists.

Features
-------- 
* Create/Delete playlist
* Open an audio file 
* Add/Remove audio file to/from a playlist
* Add tracks to queue

Building from source
-------------------- 
What you need: 
* A C++ compiler 
* CMake 
* JUCE Dependencies 

For JUCE Dependencies use this command (for apt package manager):

```
sudo apt-get update
        sudo apt-get install -y \
             libasound2-dev libjack-jackd2-dev \
             ladspa-sdk \
             libcurl4-openssl-dev  \
             libfreetype-dev libfontconfig1-dev \
             libx11-dev libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev libxi-dev \
             libwebkit2gtk-4.1-dev \
             libglu1-mesa-dev mesa-common-dev libegl-dev
```

After installing the dependencies you can use ```cmake -B Builds/yourBuild``` to generate the MakeFile, then ```cd Builds/yourBuild``` and ```make``` to generate the executable, which will appear in AudioPlayer_artefacts
