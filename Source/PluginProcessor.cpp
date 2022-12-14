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
                       ), mAPVTS(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
    // criando as vozes do sampler
    
    mFormatManager.registerBasicFormats();
    mAPVTS.state.addListener(this);
    
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
    
    updateADSR();
    
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
    
    if (mShouldUpdate)
    
    {
        updateADSR();
    }
    
    // obter e armazenar informacoes do buffer MIDI
    // ================DEPRECATED (ITERATOR)==========PART 9(VIDEO)
    juce::MidiMessage m;
    
    juce::MidiBuffer::Iterator it {midiMessages};
    int sample;
    
    while (it.getNextEvent(m, sample))
    {
        // verificar note on
        if(m.isNoteOn())
            mIsNotePlayed = true;
        //note on presente - iniciar o playhead
        else if(m.isNoteOff())
        // note of presente - pare o plahead
            mIsNotePlayed = false;
            
    }
    
    mSampleCount = mIsNotePlayed ? mSampleCount += buffer.getNumSamples() : 0;
    
    
    
    //=====================================================
    
    
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
    mSampler.clearSounds(); // limpa a memoria antes de carragar um sample
    
    juce::FileChooser chooser {"Please choose a file to load"};
    
    if(chooser.browseForFileToOpen()) {
        
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
    }
    
    // acertando o range para os note-ons MIDI
    juce::BigInteger range;
    range.setRange(0, 128, true);
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1,0.1, 10.0));
}

 void SimplestSamplerAudioProcessor::loadFile(const juce::String& path)
{
     
     mSampler.clearSounds(); // limpa a memoria antes de carragar um sample
     
     auto file = juce::File(path);
     mFormatReader = mFormatManager.createReaderFor(file);
     
     auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
     
     mWaveForm.setSize(1, sampleLength);
     
     // waveform (o argumento 1 esta levando em consideracao apenas o canal esquerdo)
     
     mFormatReader->read (&mWaveForm, 0, sampleLength, 0, true, false);
     
     // iterando no buffer (a variavel buffer eh nosso pointer para o buffer de audio)
     
     auto buffer = mWaveForm.getReadPointer(0); // 0 (so o canal esquerdo)
     
     for (int sample = 0; sample < mWaveForm.getNumSamples(); ++sample)
     {
         DBG(buffer[sample]); // linha para o console (verificar se esta iterando pelo vector do sample)
     }
     
     juce::BigInteger range;
     range.setRange(0, 128, true);
     mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1,0.1, 10.0));
     
     updateADSR();
 }

void SimplestSamplerAudioProcessor::updateADSR()
{
    
    mADSRParams.attack = mAPVTS.getRawParameterValue("ATTACK")->load();
    mADSRParams.decay = mAPVTS.getRawParameterValue("DECAY")->load();
    mADSRParams.sustain = mAPVTS.getRawParameterValue("SUSTAIN")->load();
    mADSRParams.release = mAPVTS.getRawParameterValue("RELEASE")->load();
    
    // DBG("attack: " << attack << " decay: " << decay << " sustain " << sustain << " release " << release);
    
    for (int i = 0; i < mSampler.getNumSounds(); i++)
    {
        if ( auto sound = dynamic_cast<juce::SamplerSound* >(mSampler.getSound(i).get()))
        {
            
            sound->setEnvelopeParameters(mADSRParams);
        }
        mSampler.getSound(i);
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout SimplestSamplerAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
    
    parameters.push_back (std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    parameters.push_back (std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 3.0f, 2.0f));
    parameters.push_back (std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    parameters.push_back (std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 2.0f));
    
    return { parameters.begin(), parameters.end()};
}

void SimplestSamplerAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    mShouldUpdate = true;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimplestSamplerAudioProcessor();
}
