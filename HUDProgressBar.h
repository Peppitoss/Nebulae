#pragma once
#include"Screen.h"
#include"ScreenButton.h"
#include"PlayerVessel.h"

using imagehandling::TextureObject_2;

enum enHUDProgressBarMeasurer
{
	MeasureBooster = 1
};

class HUDProgressBar_1 : public Screen
{
public:

	HUDProgressBar_1(const Coord& pos, enHUDProgressBarMeasurer toMeasure);

	void DefinePlayer(PlayerVessel* player);

	void render(SDL_Renderer& renderer) override;
	void RenderWidget(std::shared_ptr<Widget> widget, SDL_Renderer&) override;

private:

	PlayerVessel* pPlayer;
	enHUDProgressBarMeasurer pToMeasure;

	void InitializeValues();
	void CropImage();
	void SetHUDCurrentValue();

	//int HUDMaxValue;
	int lastRatio = -1;
	double HUDCurrentValue;

	std::shared_ptr<TextureObject_2> pBar;
};
