#pragma once
#include"Screen.h"
#include"ScreenButton.h"
#include"UnitEntity.h"
//#include"TextureObject.h"

using imagehandling::TextureObject_2;
//using std::string;
//static void SelectLauncher( Widget* );

enum enHUDSliderMeasurer
{
	MeasureHitPoint = 0,
	MeasureAvailableThrust = 1,
};

class HUDSlider : public Screen
{
public:

	HUDSlider(const Coord& pos, enHUDSliderMeasurer toMeasure, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void DefinePlayer(UnitEntity* player);

	void render(SDL_Renderer& renderer) override;
	void RenderWidget(std::shared_ptr<Widget> widget, SDL_Renderer&) override;

private:

	UnitEntity* pPlayer;
	enHUDSliderMeasurer pToMeasure;

	void InitializeValues();
	void CropHudSliders();
	void SetHUDCurrentValue();

	int HUDMaxValue;
	int lastRatio = -1;
	double HUDCurrentValue;

	int GetSliderSmallFrame();
	int GetSliderMediumFrame();
	int GetSliderLargeFrame();

	SDL_RendererFlip pFlip;

	std::shared_ptr<TextureObject_2> sliderSmall;
	std::shared_ptr<TextureObject_2> sliderMedium;
	std::shared_ptr<TextureObject_2> sliderLarge;
};
