#include"ShipSelectTablet.h"
#include"BackgroundCollection.h"
#include"GameManager.h"
#include"SurfaceButton.h"
#include"ScreenButton.h"

BackNextTablet::BackNextTablet(int w, int h, const Coord pos, const imagehandling::IMGToSurface* surf, bool next, bool back)
	: Screen(pos, Coord(w, h), surf)
{
	Initialize(next, back);
}

void BackNextTablet::Initialize(bool next, bool back)
{
	this->CenterScreenPositionBasedOnBackgroundImage(Coord(this->pScreenWidth, this->pScreenHeight));
	Coord pos = this->GetPosition();
	Coord bottomRight = pos + Coord(this->Width(), this->Height());

	if (next == false && back == false)
		return;

	Coord size = Coord(128, STANDARD_BUTTON_HEIGHT);

	if (next)
	{
		Widget* next = new ScreenButton(bottomRight - size * 2, size.x, size.y, BtnText("Next"));
		this->Control()->AddWidget(next);
		bottomRight = bottomRight.xadd(-2*next->GetSize().x);
	}
	if (back)
	{
		
		Widget* prev = new ScreenButton(bottomRight - size, size.x, size.y, BtnText("Previous"));	//new BoxedButton("", ImportImage_SmlButtonBack, bottomRight, nullptr);
		this->Control()->AddWidget(prev);
		bottomRight = bottomRight.xadd(-1 * prev->GetSize().x);
	}
	Widget* close = new ScreenButton(Coord(pos.x, pos.y + this->Height() - size.y), size.x, size.y, BtnText("Close"));
	//close->SetPosition(Coord(pos.x, pos.y + this->Height() - close->GetSize().y));
	this->Control()->AddWidget(close);
}

void BackNextTablet::HandleActiveWidget(SDL_Event& e, Widget* widget)
{
	Screen::HandleActiveWidget(e, widget);
	std::string txt = widget->ID();

	if (txt == "Previous")
	{
		//GameManager::closeOpenTablet();
		return;
	}
	if (txt == "Next")
	{
		//GameManager::closeOpenTablet();
		return;
	}
	if (txt == "Close")
	{
		GameManager::Screens.closeOpenTablet();
		return;
	}

}