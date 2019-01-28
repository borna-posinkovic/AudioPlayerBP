/*
  ==============================================================================

    ThumbnailWaveform.h
    Created: 28 Jan 2019 11:13:57pm
    Author:  Borna

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
/*
*/
class ThumbnailWaveform    : public Component,
							 public ChangeListener
{
public:
    ThumbnailWaveform(int sourceSamplesPerThumbnailSample, AudioFormatManager& formatManager, AudioThumbnailCache& cache)
		: thumbnail(sourceSamplesPerThumbnailSample, formatManager, cache)
    {

		thumbnail.addChangeListener(this);

    }

    ~ThumbnailWaveform()
    {
    }


	void setFile(const File& file) {

		thumbnail.setSource(new FileInputSource(file));
	}


    void paint (Graphics& g) override
    {
		if (thumbnail.getNumChannels() == 0)
			paintIfNoFileLoaded(g);
		else
			paintIfFileLoaded(g);
    }

	void paintIfNoFileLoaded(Graphics& g) {

		g.fillAll(Colours::darkgrey);
		g.setColour(Colours::ghostwhite);
		g.drawFittedText("No File Loaded", getLocalBounds(), Justification::centred, 1.0f);

	}

	void paintIfFileLoaded(Graphics& g) {

		g.fillAll(Colours::dimgrey);

		g.setColour(Colours::darkred);
		thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);

	}

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		if (source == &thumbnail)
			thumbnailChanged();

	}


    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }


private:
	
	AudioThumbnail thumbnail;

	void thumbnailChanged() {

		repaint();
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailWaveform)
};
