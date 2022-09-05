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
    g.fillAll(juce::Colours::black);
    
    // se for maior que 0, temos um som carregado
    if(audioProcessor.getNumSamplerSounds() > 0)
    {
        g.fillAll(juce::Colours::indianred);
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        g.drawText("Sound Loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }
    
    else
    {
        g.setColour(juce::Colours::white);
        g.drawText("Load a Sound", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }
     
    

}

void SimplestSamplerAudioProcessorEditor::resized()
{
    
    // mLoadButton.setBounds(getWidth() / 2 - 50, getWidth() / 2 - 50, 100, 100);
    
}

bool SimplestSamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            // o usuario esta arrastando um arquivo de audio com extensao valida
            return true;
        }
    }
    return false;
}

void SimplestSamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)

{
    for (auto file : files)
    {
        if(isInterestedInFileDrag(file))
        {
            // carregue o arquivo
            audioProcessor.loadFile(file);
        }
    }
    
    repaint();
}

