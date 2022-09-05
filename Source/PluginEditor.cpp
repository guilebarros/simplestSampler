/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimplestSamplerAudioProcessorEditor::SimplestSamplerAudioProcessorEditor (SimplestSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
    addAndMakeVisible(mLoadButton);
    setSize (200, 200);
}

SimplestSamplerAudioProcessorEditor::~SimplestSamplerAudioProcessorEditor()
{
}

//==============================================================================
void SimplestSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SimplestSamplerAudioProcessorEditor::resized()
{
    mLoadButton.setBounds(getWidth() / 2 - 50, getWidth() / 2 - 50, 100, 100);
}
