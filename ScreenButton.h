#pragma once
#include"SurfaceButton.h"

class ScreenButton : public SurfaceButton
{
public:
	
	ScreenButton(Coord pos, int width, int height, Text text);
	ScreenButton(Coord pos, ButtonStyle& style);
	ScreenButton(Coord pos, ButtonStyle& style, string ID);
	ScreenButton(Coord pos, const imagehandling::IMGToSurface* image, string ID);
	ScreenButton(Coord pos, const imagehandling::IMGToSurface* image, Text text);

	ScreenButton(Coord pos, Text text, int textSize, const imagehandling::IMGToSurface* image);

private:


};