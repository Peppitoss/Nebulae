#pragma once
#include"TextureObject.h"

class CircleColor : public imagehandling::TextureObject_2
{

public:

	CircleColor();
	CircleColor(int radius);
	CircleColor(int radius, ColorRGB color);

	void CircleColor::generateTexture(SDL_Renderer*) override;
	void SetColor(const ColorRGB&);

	int GetRadius() const;

protected:

	int pRadius;

};