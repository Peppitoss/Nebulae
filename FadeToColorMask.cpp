#include"FadeToColorMask.h"
#include"ShipImageDefinitions.h"


FadeToColor::FadeToColor() : Mask()
{

}

FadeToColor::FadeToColor(int fadeTime, const ColorRGB& col) : Mask()
{
	pFadeTime = fadeTime;
	pRemainingTime = pFadeTime;
	pIsTimed = pRemainingTime > 0;
	pColor = col;

	pTexture = std::shared_ptr < imagehandling::TextureObject_2>(new imagehandling::TextureObject_2(ImportImage_whiteMask));
	pTexture->SetCustomModulation(col);
	pTexture->setModulation(imagehandling::enumTextureModulation::CUSTOMMOD);

}

void FadeToColor::RunMask(double milliseconds)
{
	if (IsFinished())
		return;

	if (pIsTimed)
		pRemainingTime -= milliseconds;
	pTexture->setOpacity(255 * ( 1 - (pRemainingTime / pFadeTime) ), false);

	if (IsFinished())
		pTexture->setOpacity(255, false);

}

