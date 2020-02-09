#include"HUDSlider.h"
#include"GameManager.h"
#include"BackgroundCollection.h"
//#include"StyleFactory.h"



HUDSlider::HUDSlider(const Coord& pos, enHUDSliderMeasurer toMeasure, SDL_RendererFlip flip) : Screen(pos, Coord(610, 100))	//1280
{
	pToMeasure = toMeasure;
	screenPosition = pos;
	pPlayer = nullptr;

	pFlip = flip;

	pScreenWidth = 610;
	pScreenHeight = 48;

	sliderSmall = std::shared_ptr<TextureObject_2>(new TextureObject_2());
	sliderSmall->SetCropBase(ImportImage_HUDSliderSmall);
	sliderSmall->SetFlipOnDraw(flip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

	sliderMedium = std::shared_ptr<TextureObject_2>(new TextureObject_2());
	sliderMedium->SetCropBase(ImportImage_HUDSliderMedium);
	sliderMedium->SetFlipOnDraw(flip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

	sliderLarge = std::shared_ptr<TextureObject_2>(new TextureObject_2());
	sliderLarge->SetCropBase(ImportImage_HUDSliderLarge);
	sliderLarge->SetFlipOnDraw(flip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

	InitializeValues();
}

void HUDSlider::DefinePlayer(UnitEntity* player)
{
	pPlayer = player;
	InitializeValues();
}

void HUDSlider::InitializeValues()
{
	if (pPlayer == nullptr)
		return;

	switch (pToMeasure)
	{
	case(enHUDSliderMeasurer::MeasureAvailableThrust):
		HUDMaxValue = pPlayer->GetDriverThrust();
		HUDCurrentValue = HUDMaxValue;
		break;
	case(enHUDSliderMeasurer::MeasureHitPoint):
		HUDMaxValue = pPlayer->getHull().getHitPoints(game::enHitPointType::HP_MATERIAL).maxHitPoints();
		HUDCurrentValue = HUDMaxValue;
		break;
	}
}

void HUDSlider::render(SDL_Renderer& renderer)
{
	Screen(render);

	Coord posSmallSlider = Coord(62, 14 + 6);
	Coord posLargeSlider = Coord(200, 0);
	Coord posMediumSlider = Coord(130, 13);

	if (pFlip == SDL_FLIP_HORIZONTAL)
	{
		posLargeSlider = Coord(60, 0);
		posSmallSlider = Coord(62 + 72 + 101, 14 + 6);
		posMediumSlider = Coord(130 + 29, 13);
	}

	this->CropHudSliders();

	sliderSmall->RenderTexture(&renderer, screenPosition + posSmallSlider);
	sliderMedium->RenderTexture(&renderer, screenPosition + posMediumSlider);
	sliderLarge->RenderTexture(&renderer, screenPosition + posLargeSlider);
}

void HUDSlider::RenderWidget(std::shared_ptr<Widget> widget, SDL_Renderer& renderer)
{
	Screen::RenderWidget(widget, renderer);

	//Coord posSmallSlider = Coord(62, 14 + 6);
	//Coord posLargeSlider = Coord(200, 0);
	//Coord posMediumSlider = Coord(130, 13);

	//if (pFlip == SDL_FLIP_HORIZONTAL)
	//{
	//	posLargeSlider = Coord(60, 0);
	//	posSmallSlider = Coord(62 + 72 + 101, 14 + 6);
	//	posMediumSlider = Coord(130 + 29, 13);
	//}

	//this->CropHudSliders();

	//sliderSmall->RenderTexture(&renderer, screenPosition + posSmallSlider);
	//sliderMedium->RenderTexture(&renderer, screenPosition + posMediumSlider);
	//sliderLarge->RenderTexture(&renderer, screenPosition + posLargeSlider);
}

void HUDSlider::SetHUDCurrentValue()
{
	switch (pToMeasure)
	{
	case(enHUDSliderMeasurer::MeasureAvailableThrust):
		HUDCurrentValue = pPlayer->GetDriverThrust();
		break;
	case(enHUDSliderMeasurer::MeasureHitPoint):
		HUDCurrentValue = pPlayer->getHull().getHitPoints(game::enHitPointType::HP_MATERIAL).remainingHitPoints();
		break;
	}
}

void HUDSlider::CropHudSliders()
{
	if (pPlayer == nullptr)
	{
		sliderSmall->ChangeFrame(1);
		sliderMedium->ChangeFrame(1);
		sliderLarge->ChangeFrame(1);
		return;
	}
	SetHUDCurrentValue();

	double steps = 14;
	double ratio = steps * HUDCurrentValue / HUDMaxValue;

	if (lastRatio == ratio)
		return;

	if (ratio >= steps)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(5);
		sliderLarge->ChangeFrame(6);
	}
	else if (ratio >= steps-1)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(5);
		sliderLarge->ChangeFrame(5);
	}
	else if (ratio >= steps - 2)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(5);
		sliderLarge->ChangeFrame(4);
	}
	else if (ratio >= steps - 3)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(5);
		sliderLarge->ChangeFrame(3);
	}
	else if (ratio >= steps - 4)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(5);
		sliderLarge->ChangeFrame(2);
	}
	else if (ratio >= steps - 5)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(5);
		sliderLarge->ChangeFrame(1);
	}
	else if (ratio >= steps - 6)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(4);
		sliderLarge->ChangeFrame(1);
	}
	else if (ratio >= steps - 7)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(3);
		sliderLarge->ChangeFrame(1);
	}
	else if (ratio >= steps - 8)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(2);
		sliderLarge->ChangeFrame(1);
	}
	else if (ratio >= steps - 9)
	{
		sliderSmall->ChangeFrame(5);
		sliderMedium->ChangeFrame(1);
		sliderLarge->ChangeFrame(1);
	}
	else if (ratio >= steps - 10)
	{
		sliderSmall->ChangeFrame(4);
		sliderMedium->ChangeFrame(1);
		sliderLarge->ChangeFrame(1);
	}
	else if (ratio >= steps - 11)
	{
		sliderSmall->ChangeFrame(3);
		sliderMedium->ChangeFrame(1);
		sliderLarge->ChangeFrame(1);
	}
	else if (ratio >= steps - 12)
	{
		sliderSmall->ChangeFrame(2);
		sliderMedium->ChangeFrame(1);
		sliderLarge->ChangeFrame(1);
	}
	else
	{
		sliderSmall->ChangeFrame(1);
		sliderMedium->ChangeFrame(1);
		sliderLarge->ChangeFrame(1);
	}

	if (lastRatio != ratio)
	{
		lastRatio = ratio;
		sliderSmall->ResetTexture();
		sliderMedium->ResetTexture();
		sliderLarge->ResetTexture();
	}
}