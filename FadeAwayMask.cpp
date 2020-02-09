#include"FadeAwayMask.h"
#include"ShipImageDefinitions.h"

FadeAwayMask::FadeAwayMask() : Mask()
{

}

FadeAwayMask::FadeAwayMask(int fadeTime, const ColorRGB& col) : Mask()
{
	pFadeTime = fadeTime;
	pRemainingTime = pFadeTime;
	pIsTimed = pRemainingTime > 0;
	pColor = col;

	pTexture = std::shared_ptr < imagehandling::TextureObject_2>(new imagehandling::TextureObject_2(ImportImage_whiteMask));
	pTexture->SetCustomModulation(col);
	pTexture->setModulation(imagehandling::enumTextureModulation::CUSTOMMOD);

}

void FadeAwayMask::RunMask(double milliseconds)
{
	if (IsFinished())  
		return;
	
	if(pIsTimed)
		pRemainingTime -= milliseconds/1000;
	pTexture->setOpacity(255 * (pRemainingTime / pFadeTime), false);

}

