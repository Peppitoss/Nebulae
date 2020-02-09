#include"BlendMask.h"
#include"ShipImageDefinitions.h"

BlendMask::BlendMask() : Mask()
{

}

BlendMask::BlendMask(SDL_BlendMode mode, const ColorRGB& col, int opacity)
{
	pRemainingTime = 1;
	pBlendMode = mode;
	pColor = col;
	pIsTimed = false;

	pTexture = std::shared_ptr < imagehandling::TextureObject_2>(new imagehandling::TextureObject_2(ImportImage_whiteMask));
	pTexture->SetCustomModulation(col);
	pTexture->setModulation(imagehandling::enumTextureModulation::CUSTOMMOD);
	pTexture->setOpacity(opacity, true);
	pTexture->setBlendMode(pBlendMode);
	//SDL_Settex
}

