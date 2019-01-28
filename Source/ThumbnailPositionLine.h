/*
  ==============================================================================

    ThumbnailPositionLine.h
    Created: 29 Jan 2019 12:06:41am
    Author:  Borna

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ThumbnailPositionLine    : public Component,
								 public Timer
{
public:
    ThumbnailPositionLine(AudioTransportSource& transportSourceToUse) :
		transportSource(transportSourceToUse)
    {
		startTimer(40);
    }

    ~ThumbnailPositionLine()
    {
    }

    void paint (Graphics& g) override
    {
		auto duration = transportSource.getLengthInSeconds();

		if (duration > 0.0)
		{
			auto audioPosition = transportSource.getCurrentPosition();
			auto drawPosition = (audioPosition / duration) * getWidth();

			g.setColour(Colours::green);
			g.drawLine(drawPosition, 0.0f, drawPosition, (float)getHeight(), 2.0f);
		}
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

	void mouseDown(const MouseEvent& event) override
	{
		auto duration = transportSource.getLengthInSeconds();

		if (duration > 0.0)
		{
			auto clickPosition = event.position.x;
			auto audioPosition = (clickPosition / getWidth()) * duration;

			transportSource.setPosition(audioPosition);
		}
	}

private:

	AudioTransportSource& transportSource;

	void timerCallback() override
	{
		repaint();
	}


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailPositionLine)
};
