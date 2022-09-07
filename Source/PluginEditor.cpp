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
    setSize (600, 200);
}

SimplestSamplerAudioProcessorEditor::~SimplestSamplerAudioProcessorEditor()
{
}

//==============================================================================
void SimplestSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    
    
    // so vai desenhar a waveform se tivermos arrastado um arquivo
    
    if (mShouldBePainting)
    {
        // classe Path para desenhar a waveform
        juce::Path p;
        mAudioPoints.clear(); // limpa o path, caso carreguemos outro sample
        
        // Lógica (SUDO CODE)
        // O arquivo foi arrastado?
            // If yes
                // 1.obtenha a forma de onda do processor
                // 2.encontre ratio:  sample = 44100 (1 sec) ...eixo x(600 pixels) == sampleLength / getWidth()
                // 3. valores/amplitude da forma de onda (entre -1 e 1) ...eixo y(200 pixels) == (1 - 1 == 200 - 0)
        
        // 1.obtenha a forma de onda do processor
        auto waveform = audioProcessor.getWaveForm();
        
        // 2.encontre ratio:  sample = 44100 (1 sec) ...eixo x(600 pixels) == sampleLength / getWidth()
        
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);
        
        // escalar o arquivo de audio para o eixo x
        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(0, getHeight() / 2);
        
        // escalar para o eixo y
        
        for(int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>( mAudioPoints[sample], -1.0f, 1.0f, 200, 0);
            // adicionando linhas no path (valor do sample e o valor escalado no eixo y)
            p.lineTo(sample, point);
        }
        
        // finalmente desenhando o path
        g.strokePath(p, juce::PathStrokeType(2));
        
        mShouldBePainting = false; // nao precisamos desenhar de novo a waveform
        
    }
        
    
// COMMIT # 02 do PLUGIN (DRAG AND DROP)
    
//    g.setColour(juce::Colours::white);
//    g.setFont(15.0f);
//    // se for maior que 0, temos um som carregado
//    if(audioProcessor.getNumSamplerSounds() > 0)
//    {
//        g.fillAll(juce::Colours::indianred);
//        g.drawText("Sound Loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
//    }
//
//    else
//    {
//        g.drawText("Load a Sound", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
//    }
//
    

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
            mShouldBePainting = true; // desenhar a waveform
            // carregue o arquivo
            audioProcessor.loadFile(file);
        }
    }
    
    repaint();
}

