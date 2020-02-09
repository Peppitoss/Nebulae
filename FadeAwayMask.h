#pragma once
#include"Mask.h"
#include"Graphics.h"

class FadeAwayMask : public Mask
{
public:

	FadeAwayMask();
	FadeAwayMask( int fadeTime, const ColorRGB& col);

	void RunMask(double milliseconds) override;

private:

	double pFadeTime;
	ColorRGB pColor;

};