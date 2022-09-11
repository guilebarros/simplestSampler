#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"

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
    
    juce::TextButton mLoadButton { "Load " };
    
    WaveThumbnail mWaveThumbnail;
    
    juce::Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    juce::Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mAttackAttachment,
                                                                          mDecayAttachment,
                                                                          mSustainAttachment,
                                                                          mReleaseAttachment;
    
    SimplestSamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimplestSamplerAudioProcessorEditor)
};
