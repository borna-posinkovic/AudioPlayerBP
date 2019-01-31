/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ThumbnailWaveform.h"
#include "ThumbnailPositionLine.h"

//==============================================================================
/**
*/
class AudioPlayerBpAudioProcessorEditor  : public AudioProcessorEditor,
										   public ChangeListener
{
public:

	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;


    AudioPlayerBpAudioProcessorEditor (AudioPlayerBpAudioProcessor&);
    ~AudioPlayerBpAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	void transportSourceChanged();

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
    
    AudioPlayerBpAudioProcessor& processor;

	TransportState state;

	AudioThumbnailCache thumbnailCache;
	ThumbnailWaveform thumbnailComponent;
	ThumbnailPositionLine positionLine;
	TextButton playButton;
	TextButton openButton;
	TextButton stopButton;

	Label gainLabel;
	Slider gainSlider;
	std::unique_ptr <SliderAttachment> gainValue;

	ToggleButton invertPhaseButton;
	std::unique_ptr <ButtonAttachment> invertPhaseValue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayerBpAudioProcessorEditor)
};
