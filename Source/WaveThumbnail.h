/*
  ==============================================================================

    WaveThumbnail.h
    Created: 11 Sep 2022 12:49:24am
    Author:  Guilherme Barros

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component,
                       public juce::FileDragAndDropTarget
{
public:
    WaveThumbnail(SimplestSamplerAudioProcessor& p);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

private:
    
    std::vector<float> mAudioPoints;
    bool mShouldBePainting { false }; // so desenha a waveform se carregar um arquivo
    
    juce::String mFileName { "" };
    
    SimplestSamplerAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
