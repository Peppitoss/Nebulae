#pragma once
#include"ScreenTablet.h"
#include"Graphics.h"
#include<string>



class SingleButtonTablet : public Screen
{

public:

	SingleButtonTablet( int w, int h, const Coord pos, const imagehandling::IMGToSurface*,  std::string buttonText );

	void HandleActiveWidget(SDL_Event& e, Widget* widget) override;

private:

	void Initialize(std::string text);

};