#pragma once
#include"TextureObject.h"

class RectangleColor : public imagehandling::TextureObject_2
{

public:

	RectangleColor();
	RectangleColor( int width, int height );
	RectangleColor( int width, int height, ColorRGB color );

	void RectangleColor::generateTexture( SDL_Renderer* ) override;
	void SetColor( const ColorRGB& );

	int GetWidth() const;
	//void SetWidth( int value ) { this->pWidth = value; };

	int GetHeight() const;
	//void SetHeight( int value ) { this->pHeight = value; };

protected:

	int pWidth;
	int pHeight;

};