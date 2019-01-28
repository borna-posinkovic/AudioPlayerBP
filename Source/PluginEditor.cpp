/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPlayerBpAudioProcessorEditor::AudioPlayerBpAudioProcessorEditor (AudioPlayerBpAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), state(Stopped), thumbnailCache(5), 
	thumbnailComponent(512, processor.formatManager, thumbnailCache), positionLine(processor.transportSource)
{
 
    setSize (400, 300);

	addAndMakeVisible(&openButton);
	openButton.setButtonText("Open...");
	openButton.onClick = [this] { openButtonClicked(); };

	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(false);

	addAndMakeVisible(&stopButton);
	stopButton.setButtonText("Stop");
	stopButton.onClick = [this] { stopButtonClicked(); };
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	stopButton.setEnabled(false);

	addAndMakeVisible(&thumbnailComponent);
	addAndMakeVisible(&positionLine);
	
	processor.transportSource.addChangeListener(this);
}

AudioPlayerBpAudioProcessorEditor::~AudioPlayerBpAudioProcessorEditor()
{
}

//==============================================================================
void AudioPlayerBpAudioProcessorEditor::paint (Graphics& g)
{

 }

void AudioPlayerBpAudioProcessorEditor::resized()
{
	openButton.setBounds(10, 10, getWidth() - 10, 20);
	playButton.setBounds(10, 40, getWidth() - 10, 20);
	stopButton.setBounds(10, 70, getWidth() - 10, 20);

	Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);
	thumbnailComponent.setBounds(thumbnailBounds);
	positionLine.setBounds(thumbnailBounds);
}


void AudioPlayerBpAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &processor.transportSource)
		transportSourceChanged();
	
}


void AudioPlayerBpAudioProcessorEditor::changeState(TransportState newState) {

	if (state != newState)
	{
		state = newState;
		switch (state)
		{
		case Stopped:                           
			stopButton.setEnabled(false);
			playButton.setEnabled(true);
			processor.transportSource.setPosition(0.0);
			break;
		case Starting:                          
			playButton.setEnabled(false);
			processor.transportSource.start();
			break;
		case Playing:                           
			stopButton.setEnabled(true);
			break;
		case Stopping:                          
			processor.transportSource.stop();
			break;
		}
	}

}

void AudioPlayerBpAudioProcessorEditor::openButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...",
		{},
		"*.wav");                                      
	if (chooser.browseForFileToOpen())                                 
	{
		File file = chooser.getResult();                                  
		auto* reader = processor.formatManager.createReaderFor(file);              
		if (reader != nullptr)
		{
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true)); 
			processor.transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);                     
			playButton.setEnabled(true);   
			thumbnailComponent.setFile(file);
			processor.readerSource.reset(newSource.release());                                                      
		}
	}
}

void AudioPlayerBpAudioProcessorEditor::playButtonClicked()
{
	changeState(Starting);
}

void AudioPlayerBpAudioProcessorEditor::stopButtonClicked()
{
	changeState(Stopping);
}

void AudioPlayerBpAudioProcessorEditor::transportSourceChanged()
{
	if (processor.transportSource.isPlaying())
		changeState(Playing);
	else
		changeState(Stopped);
}

