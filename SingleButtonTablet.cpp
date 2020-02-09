#include"SingleButtonTablet.h"
#include"BackgroundCollection.h"
#include"GameManager.h"
#include"SurfaceButton.h"
#include"ScreenButton.h"



SingleButtonTablet::SingleButtonTablet(int w, int h, const Coord pos, const imagehandling::IMGToSurface* surf, std::string buttonText)
	: Screen(pos, Coord(w, h) , surf )
{
	Initialize(buttonText);
}

void SingleButtonTablet::Initialize(std::string text)
{
	this->CenterScreenPositionBasedOnBackgroundImage(Coord(this->pScreenWidth, this->pScreenHeight));
	Coord pos = this->GetPosition();
	Coord bottomRight = pos + Coord(this->Width(), this->Height());

	//int bHeight = 0;
	//int bWidth = 220;
	Coord btnSize = Coord(128, STANDARD_BUTTON_HEIGHT);
	Widget* close = new ScreenButton(Coord(bottomRight) - btnSize, btnSize.x, btnSize.y, BtnText(text.c_str()));
	
	//close->SetPosition();
	this->Control()->AddWidget(close);
	
}

void SingleButtonTablet::HandleActiveWidget(SDL_Event& e, Widget* widget)
{
	Screen::HandleActiveWidget(e, widget);
	std::string txt = widget->ID();

	if (txt == "Close" || txt == "close" )
	{
		GameManager::Screens.closeOpenTablet();
	}

}