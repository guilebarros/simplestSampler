/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 11 Sep 2022 12:49:24am
    Author:  Guilherme Barros

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(SimplestSamplerAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::cadetblue.darker());
    // so vai desenhar a waveform se tivermos arrastado um arquivo
    auto waveform = audioProcessor.getWaveForm();
    
    if(waveform.getNumSamples() > 0)

    {
        // classe Path para desenhar a waveform
        
        juce::Path p;
        mAudioPoints.clear(); // limpa o path, caso carreguemos outro sample
        
        
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);
        
        // escalar o arquivo de audio para o eixo x
        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        
        g.setColour(juce::Colours::yellow);
        p.startNewSubPath(0, getHeight() / 2);
        
        // escalar para o eixo y
        
        for(int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>( mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0);
            // adicionando linhas no path (valor do sample e o valor escalado no eixo y)
            p.lineTo(sample, point);
        }
        
        // finalmente desenhando o path
        g.strokePath(p, juce::PathStrokeType(2));
        
        
        // texto com o nome do arquivo carregado no sample
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        auto textBounds = getLocalBounds().reduced(10, 10);
        g.drawFittedText(mFileName, textBounds, juce::Justification::topRight, 1);
    }
    
    else
    {
        // se nao carregar o sample escrever o texto para carregar
        g.setColour(juce::Colours::white);
        g.setFont(20.0f);
        g.drawFittedText("Dude, drop an audio file to load", getLocalBounds(), juce::Justification::centred, 1);
    }
}

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool WaveThumbnail::isInterestedInFileDrag(const juce::StringArray& files)
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

void WaveThumbnail::filesDropped(const juce::StringArray& files, int x, int y)

{
    for (auto file : files)
    {
        if(isInterestedInFileDrag(file))
        {
           //obtendo o nome do arquivo
            auto myFile = std::make_unique<juce::File>(file);
            mFileName = myFile->getFileNameWithoutExtension();
            // carregue o arquivo
            audioProcessor.loadFile(file);
            
            
        }
    }
    
    repaint();
}
