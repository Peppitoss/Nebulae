#include"PointColor.h"


bool PointColor::pFlagTextureFiltering = true;	//true or false??

PointColor::PointColor() : TextureObject_2(&imagehandling::whiteParticle)
{
	this->SetCustomModulation( ColorRGB(255,255,255) );
	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
	this->pScale = spriteSize(spriteSize(1));
	this->pAllowTextureFiltering = pFlagTextureFiltering;
}

PointColor::PointColor( ColorRGB color ) : TextureObject_2(&imagehandling::whiteParticle)
{
	this->SetCustomModulation( color );
	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
	this->pScale = spriteSize(spriteSize(1));
	this->pAllowTextureFiltering = pFlagTextureFiltering;
}

PointColor::PointColor( ColorRGB color, spriteSize eSize ) : TextureObject_2(imagehandling::IMGToSurface(eSize.width(), eSize.height(), color))
{
	this->SetCustomModulation( color );
	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
	this->pScale = spriteSize(eSize);
	this->pAllowTextureFiltering = pFlagTextureFiltering;
}

PointColor::PointColor( const imagehandling::IMGToSurface* surf ) : TextureObject_2(surf)
{
	this->SetCustomModulation( ColorRGB(255,255,255) );
	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
	this->pScale = spriteSize(spriteSize(1));
	this->pAllowTextureFiltering = pFlagTextureFiltering;
}

//PointColor::PointColor( const imagehandling::IMGToSurface*, spriteSize size)
//{
//	this->SetCustomModulation( ColorRGB(255,255,255) );
//	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
//	this->pScale = spriteSize(size);
//}

void PointColor::generateTexture( SDL_Renderer* renderer)
{
	TextureObject_2::generateTexture( renderer );
}

void PointColor::SetColor( const ColorRGB& col)
{
	this->SetCustomModulation( col );
	this->ResetTexture();
}

spriteSize PointColor::GetScaling()
{
	return pScale;
}