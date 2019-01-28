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
    : AudioProcessorEditor (&p), processor (p), state(Stopped), thumbnailCache(5), thumbnail(512, processor.formatManager, thumbnailCache)
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


	thumbnail.addChangeListener(this);
	processor.transportSource.addChangeListener(this);

	startTimer(40);
}

AudioPlayerBpAudioProcessorEditor::~AudioPlayerBpAudioProcessorEditor()
{
}

//==============================================================================
void AudioPlayerBpAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

	juce::Rectangle<int> thumbnailBounds(10, 100, getWidth() - 20, getHeight() - 120);
	if (thumbnail.getNumChannels() == 0)
		paintIfNoFileLoaded(g, thumbnailBounds);
	else
		paintIfFileLoaded(g, thumbnailBounds);


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
		transportSourceChanged();
	else if (source == &thumbnail)
		thumbnailChanged();
	
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
			thumbnail.setSource(new FileInputSource(file));
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

void AudioPlayerBpAudioProcessorEditor::thumbnailChanged() {

	repaint();

}

void AudioPlayerBpAudioProcessorEditor::paintIfNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds) {
	g.setColour(Colours::darkgrey);
	g.fillRect(thumbnailBounds);
	g.setColour(Colours::white);
	g.drawFittedText("No File Loaded", thumbnailBounds, Justification::centred, 1.0f);

}

void AudioPlayerBpAudioProcessorEditor::paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds) {

	g.setColour(Colours::white);
	g.fillRect(thumbnailBounds);
	g.setColour(Colours::red);                                     
	thumbnail.drawChannels(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1.0f);  

	auto audioLength(thumbnail.getTotalLength());                                      
	thumbnail.drawChannels(g, thumbnailBounds, 0.0, audioLength, 1.0f);

	g.setColour(Colours::green);
	auto audioPosition(processor.transportSource.getCurrentPosition());
	auto drawPosition((audioPosition / audioLength) * thumbnailBounds.getWidth() + thumbnailBounds.getX());    
	g.drawLine(drawPosition, thumbnailBounds.getY(), drawPosition, thumbnailBounds.getBottom(), 2.0f);

}