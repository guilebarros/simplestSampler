/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SimplestSamplerAudioProcessorEditor::SimplestSamplerAudioProcessorEditor (SimplestSamplerAudioProcessor& p)
: AudioProcessorEditor (&p), mWaveThumbnail(p), mADSR(p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    addAndMakeVisible(mWaveThumbnail);
    addAndMakeVisible(mADSR);
    
    startTimerHz(30);
    
    setSize (600, 400);
}

SimplestSamplerAudioProcessorEditor::~SimplestSamplerAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void SimplestSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

}

void SimplestSamplerAudioProcessorEditor::resized()
{
    
    mWaveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);
    mADSR.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);
    
}

void SimplestSamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}

