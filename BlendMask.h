#pragma once
#include"Mask.h"

class BlendMask : public Mask
{
public:

	BlendMask();
	BlendMask(SDL_BlendMode mode, const ColorRGB& col, int opacity);

private:

	SDL_BlendMode pBlendMode;
	ColorRGB pColor;

};