#include"CircleColor.h"
#include "Wrapper.h"

CircleColor::CircleColor() : TextureObject_2(new imagehandling::IMGToSurface(1, 1, ColorRGB(255, 255, 255, 255)))
{
}

CircleColor::CircleColor(int radius) : TextureObject_2(new imagehandling::IMGToSurface(radius*std::sqrt(2), radius*std::sqrt(2), ColorRGB(255, 255, 255, 255)))
{
	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
}
CircleColor::CircleColor(int radius, ColorRGB color) : TextureObject_2(new imagehandling::IMGToSurface(radius*std::sqrt(2), radius*std::sqrt(2), ColorRGB(255, 255, 255, 255)))
{
	this->SetCustomModulation(color);
	this->setOpacity(color.alpha(), false);
	this->modulationFlag = imagehandling::enumTextureModulation::CUSTOMMOD;
}

void CircleColor::generateTexture(SDL_Renderer* renderer)
{
	TextureObject_2::generateTexture(renderer);
}
void CircleColor::SetColor(const ColorRGB& col)
{
	this->SetCustomModulation(col);
	this->setOpacity(col.alpha(), false);
	this->ResetTexture();
}


int CircleColor::GetRadius() const
{
	return this->pRadius;
}