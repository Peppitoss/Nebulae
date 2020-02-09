#pragma once
#include"ScreenTablet.h"
#include"Graphics.h"
#include<string>

class BackNextTablet : public Screen
{

public:

	BackNextTablet(int, int, const Coord pos, const imagehandling::IMGToSurface*, bool next, bool back);

	void HandleActiveWidget(SDL_Event& e, Widget* widget) override;

protected:

	void Initialize(bool next, bool back);

};