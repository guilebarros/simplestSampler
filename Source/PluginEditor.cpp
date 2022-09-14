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
    
    //acessando a foto .png na pasta Resources (o nome do arquivo eh pic.png)
    
    auto myPicImage = juce::ImageCache::getFromMemory(BinaryData::pic_png, BinaryData::pic_pngSize);
    
    // checar se a imagem eh valida
    
    if (! myPicImage.isNull())
        mImageComponent.setImage(myPicImage, juce::RectanglePlacement::stretchToFit);
    else
        jassert(! myPicImage.isNull());
    
    addAndMakeVisible(mWaveThumbnail);
    addAndMakeVisible(mADSR);
    //foto pic.png
    addAndMakeVisible(mImageComponent);
    
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
    // foto pic.png
    mImageComponent.setBoundsRelative(0.33f, 0.05f, 0.35f, 0.25f);
    
    
}

void SimplestSamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}

