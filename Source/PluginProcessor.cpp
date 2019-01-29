/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPlayerBpAudioProcessor::AudioPlayerBpAudioProcessor()
     : AudioProcessor (BusesProperties().withOutput("Output", AudioChannelSet::stereo()))
{

	formatManager.registerBasicFormats();
}

AudioPlayerBpAudioProcessor::~AudioPlayerBpAudioProcessor()
{
}

//==============================================================================
const String AudioPlayerBpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPlayerBpAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlayerBpAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPlayerBpAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPlayerBpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPlayerBpAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPlayerBpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPlayerBpAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioPlayerBpAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioPlayerBpAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AudioPlayerBpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	transportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void AudioPlayerBpAudioProcessor::releaseResources()
{
	transportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioPlayerBpAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void AudioPlayerBpAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	transportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
}

//==============================================================================
bool AudioPlayerBpAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioPlayerBpAudioProcessor::createEditor()
{
    return new AudioPlayerBpAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPlayerBpAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioPlayerBpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPlayerBpAudioProcessor();
}




