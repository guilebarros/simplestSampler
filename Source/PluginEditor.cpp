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
    
    // Sliders
    
    // Attack Slider
    
    mAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::peru);
    mAttackSlider.setRange(0.0f, 5.0f, 0.01f);
    mAttackSlider.addListener(this);
    addAndMakeVisible(mAttackSlider);
    
    mAttackLabel.setFont(10.0f);
    mAttackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    mAttackLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    
    // Decay Slider
    
    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::peru);
    mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
    mDecaySlider.addListener(this);
    addAndMakeVisible(mDecaySlider);
    
    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    
    // Sustain
    
    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::peru);
    mSustainSlider.setRange(0.0f, 5.0f, 0.01f);
    mSustainSlider.addListener(this);
    addAndMakeVisible(mSustainSlider);
    
    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    
    // Release Slider
    
    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::peru);
    mReleaseSlider.setRange(0.0f, 1.0f, 0.01f);
    mReleaseSlider.addListener(this);
    addAndMakeVisible(mReleaseSlider);
    
    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    
    
    
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
    const auto startX = 0.6f;
    const auto startY = 0.6f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.4f;
    
    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + (dialWidth * 3), startY, dialWidth, dialHeight);
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
            
            mShouldBePainting = true; // desenhar a waveform
            
        }
    }
    
    repaint();
}

void SimplestSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &mAttackSlider)
    {
        audioProcessor.attack = mAttackSlider.getValue();
    }
    else if (slider == &mDecaySlider)
    {
        audioProcessor.decay = mDecaySlider.getValue();
    }
    else if (slider == &mSustainSlider)
    {
        audioProcessor.sustain = mSustainSlider.getValue();
    }
    else if (slider == &mReleaseSlider)
    {
        audioProcessor.release = mReleaseSlider.getValue();
    }
}
