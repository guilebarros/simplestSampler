#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SimplestSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                             public juce::FileDragAndDropTarget
{
public:
    SimplestSamplerAudioProcessorEditor (SimplestSamplerAudioProcessor&);
    ~SimplestSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    // funcoes virtuais 
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    juce::TextButton mLoadButton { "Load " };
    
    SimplestSamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimplestSamplerAudioProcessorEditor)
};
