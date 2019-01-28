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
    : AudioProcessorEditor (&p), processor (p), state(Stopped)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
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

	processor.transportSource.addChangeListener(this);
}

AudioPlayerBpAudioProcessorEditor::~AudioPlayerBpAudioProcessorEditor()
{
}

//==============================================================================
void AudioPlayerBpAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));


}

void AudioPlayerBpAudioProcessorEditor::resized()
{
	openButton.setBounds(10, 10, getWidth() - 10, 20);
	playButton.setBounds(10, 40, getWidth() - 10, 20);
	stopButton.setBounds(10, 70, getWidth() - 10, 20);
}


void AudioPlayerBpAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &processor.transportSource)
	{
		if (processor.transportSource.isPlaying())
			changeState(Playing);
		else
			changeState(Stopped);
	}
}


void AudioPlayerBpAudioProcessorEditor::changeState(TransportState newState) {

	if (state != newState)
	{
		state = newState;
		switch (state)
		{
		case Stopped:                           // [3]
			stopButton.setEnabled(false);
			playButton.setEnabled(true);
			processor.transportSource.setPosition(0.0);
			break;
		case Starting:                          // [4]
			playButton.setEnabled(false);
			processor.transportSource.start();
			break;
		case Playing:                           // [5]
			stopButton.setEnabled(true);
			break;
		case Stopping:                          // [6]
			processor.transportSource.stop();
			break;
		}
	}

}

void AudioPlayerBpAudioProcessorEditor::openButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...",
		{},
		"*.wav");                                        // [7]
	if (chooser.browseForFileToOpen())                                    // [8]
	{
		auto file = chooser.getResult();                                  // [9]
		auto* reader = processor.formatManager.createReaderFor(file);              // [10]
		if (reader != nullptr)
		{
			std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true)); // [11]
			processor.transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);                     // [12]
			playButton.setEnabled(true);                                                                    // [13]
			processor.readerSource.reset(newSource.release());                                                        // [14]
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

