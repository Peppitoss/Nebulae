#include"HUDProgressBar.h"
#include"GameManager.h"
#include"BackgroundCollection.h"

HUDProgressBar_1::HUDProgressBar_1(const Coord& pos, enHUDProgressBarMeasurer toMeasure) : Screen(pos, Coord(610, 100))	//1280
{
	pToMeasure = toMeasure;
	screenPosition = pos;
	pPlayer = nullptr;

	pScreenWidth = 1200;
	pScreenHeight = 1150;

	pBar = std::shared_ptr<TextureObject_2>(new TextureObject_2());
	pBar->SetCropBase(ImportImage_HUDProgressBar);

	InitializeValues();
}

void HUDProgressBar_1::DefinePlayer(PlayerVessel* player)
{
	pPlayer = player;
	InitializeValues();
}

void HUDProgressBar_1::InitializeValues()
{
	if (pPlayer == nullptr)
		return;

	switch (pToMeasure)
	{
	case(enHUDProgressBarMeasurer::MeasureBooster):
		HUDCurrentValue = 1;
		break;
	}
}

void HUDProgressBar_1::render(SDL_Renderer& renderer)
{
	Coord drawPosition = screenPosition + Coord(62, 14 + 6);
	this->CropImage();

	pBar->RenderTexture(&renderer, drawPosition);

	std::string hotKeyLetter = SDL_GetScancodeName(SDL_SCANCODE_B);
	if (hotKeyLetter.size() == 1)
		this->RenderLetter(hotKeyLetter[0], drawPosition + Coord(2, 99), renderer);

}

void HUDProgressBar_1::RenderWidget(std::shared_ptr<Widget> widget, SDL_Renderer& renderer)
{
	Screen::RenderWidget(widget, renderer);
}

void HUDProgressBar_1::SetHUDCurrentValue()
{
	switch (pToMeasure)
	{
	case(enHUDProgressBarMeasurer::MeasureBooster):
		HUDCurrentValue = pPlayer->GetBoosterProgressRatio();
		break;
	}
}

void HUDProgressBar_1::CropImage()
{
	if (pPlayer == nullptr)
	{
		pBar->ChangeFrame(1);
		return;
	}
	SetHUDCurrentValue();

	double steps = 10;
	double ratio = 1 + steps * HUDCurrentValue;

	if (lastRatio == ratio)
		return;

	//if (ratio >= steps)
	//{
	//	pBar->ChangeFrame(11);
	//}
	//else
		pBar->ChangeFrame(ratio);
/*
	if (ratio >= steps)
	{
		pBar->ChangeFrame(11);
	}
	else if (ratio >= steps - 1)
	{
		pBar->ChangeFrame(10);
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
	}*/

	lastRatio = ratio;
	pBar->ResetTexture();

}