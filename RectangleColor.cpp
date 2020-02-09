#include"RectangleColor.h"
#include "Wrapper.h"

RectangleColor::RectangleColor() : TextureObject_2( new imagehandling::IMGToSurface(1, 1, ColorRGB(255,255,255,255) ) )
{
}

RectangleColor::RectangleColor( int width, int height ) : TextureObject_2( new imagehandling::IMGToSurface(width, height, ColorRGB(255,255,255,255) ) )
{
	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
}
RectangleColor::RectangleColor( int width, int height, ColorRGB color ) : TextureObject_2( new imagehandling::IMGToSurface(width, height, ColorRGB(255,255,255,255) ))
{
	this->SetCustomModulation( color );
	this->setOpacity( color.alpha(), true ); //Don't set opacity before texture exists!
	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
}

void RectangleColor::generateTexture( SDL_Renderer* renderer )
{
	TextureObject_2::generateTexture( renderer );
}
void RectangleColor::SetColor( const ColorRGB& col)
{
	this->SetCustomModulation( col );
	this->setOpacity( col.alpha(), false);
	this->ResetTexture();
}


int RectangleColor::GetWidth()  const
{ 
	return this->pWidth; 
};

int RectangleColor::GetHeight()  const
{
	return this->pHeight;
}