#pragma once
#include"Mask.h"
#include"Graphics.h"

class FadeToColor : public Mask
{
public:

	FadeToColor();
	FadeToColor(int fadeTime, const ColorRGB& col);

	void RunMask(double milliseconds) override;

private:

	double pFadeTime;
	ColorRGB pColor;

};
