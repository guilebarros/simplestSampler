/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimplestSamplerAudioProcessor::SimplestSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // criando as vozes do sampler
    
    mFormatManager.registerBasicFormats();
    
    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice(new juce::SamplerVoice());
    }
}

SimplestSamplerAudioProcessor::~SimplestSamplerAudioProcessor()
{
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String SimplestSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimplestSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimplestSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimplestSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimplestSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimplestSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimplestSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimplestSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimplestSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimplestSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimplestSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    
}

void SimplestSamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimplestSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimplestSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool SimplestSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SimplestSamplerAudioProcessor::createEditor()
{
    return new SimplestSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void SimplestSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SimplestSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// implementacao da funcao de carregar aequivos

void SimplestSamplerAudioProcessor::loadFile()
{
    juce::FileChooser chooser {"Please choose a file to load"};
    
    if(chooser.browseForFileToOpen()) {
        
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
    }
    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1,0.1, 10.0));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimplestSamplerAudioProcessor();
}
