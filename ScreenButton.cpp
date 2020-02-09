#include"ScreenButton.h"
#include"StyleFactory.h"

ScreenButton::ScreenButton(Coord pos, int width, int height, Text text)
	: SurfaceButton(text.GetID(), pos, StyleFactory::StandardButtonStyle(width, height, text))
{
	this->Refresh();
}

ScreenButton::ScreenButton(Coord pos, ButtonStyle& style)
	: SurfaceButton(style.GetTextProperties(0).pText.GetID(), pos, style) 
{
	this->Refresh();
}

ScreenButton::ScreenButton(Coord pos, ButtonStyle& style, string ID)
	: SurfaceButton(ID, pos, style)
{
	this->Refresh();
}

ScreenButton::ScreenButton(Coord pos, Text text, int textSize, const imagehandling::IMGToSurface* image)
	: SurfaceButton(text.GetID(), pos, StyleFactory::StandardButtonStyle(image, textSize, text))
{
	this->Refresh();
}

ScreenButton::ScreenButton(Coord pos, const imagehandling::IMGToSurface* image, string ID)
	: SurfaceButton(ID, pos, StyleFactory::IconButtonStyle(image))
{
	this->Refresh();
}

ScreenButton::ScreenButton(Coord pos, const imagehandling::IMGToSurface* image, Text text)
	: SurfaceButton(text.GetID(), pos, StyleFactory::IconButtonStyle(image))
{
	this->Refresh();
}