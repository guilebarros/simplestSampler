#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"

class SimplestSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor
                                             
{
public:
    SimplestSamplerAudioProcessorEditor (SimplestSamplerAudioProcessor&);
    ~SimplestSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    // funcoes virtuais 
    
    
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    WaveThumbnail mWaveThumbnail;
    ADSRComponent mADSR;
    
    SimplestSamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimplestSamplerAudioProcessorEditor)
};
