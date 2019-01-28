/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AudioPlayerBpAudioProcessorEditor  : public AudioProcessorEditor,
										   public ChangeListener,
										   private Timer
{
public:
    AudioPlayerBpAudioProcessorEditor (AudioPlayerBpAudioProcessor&);
    ~AudioPlayerBpAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	void timerCallback() override
	{
		repaint();
	}

	void transportSourceChanged();
	void thumbnailChanged();

	void paintIfNoFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);
	void paintIfFileLoaded(Graphics& g, const Rectangle<int>& thumbnailBounds);

	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();

	enum TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};


	void changeState(TransportState newState);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPlayerBpAudioProcessor& processor;

	TransportState state;

	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;

	TextButton playButton;
	TextButton openButton;
	TextButton stopButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerBpAudioProcessorEditor)
};
