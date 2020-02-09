#pragma once
#include"TextureObject.h"



class PointColor : public imagehandling::TextureObject_2
{

public:

	PointColor();
	PointColor( ColorRGB color );
	PointColor( ColorRGB, spriteSize);
	PointColor( const imagehandling::IMGToSurface* );
	//PointColor( const imagehandling::IMGToSurface*, spriteSize );

	void PointColor::generateTexture( SDL_Renderer* ) override;
	void SetColor( const ColorRGB& );

	spriteSize GetScaling();

protected:

	static bool pFlagTextureFiltering;
	spriteSize pScale;

};