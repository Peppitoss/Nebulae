#include"ShipSelectTablet.h"
#include"BackgroundCollection.h"
#include"GameManager.h"
#include"SurfaceButton.h"
#include"ScreenButton.h"

ShipSelectTablet::ShipSelectTablet(int w, int h, const Coord pos, const imagehandling::IMGToSurface* surf, string vesselName, bool next, bool back)
	: BackNextTablet(w,h, pos , surf, next, back )
{
	pVesselName = vesselName;
	Initialize(next, back);
}

void ShipSelectTablet::Initialize(bool next, bool back)
{
	BackNextTablet::Initialize(next, back);
	Coord pos = this->GetPosition();
	Coord bottomRight = pos + Coord(this->Width(), this->Height());

	Coord btnSize = Coord(128, STANDARD_BUTTON_HEIGHT);
	pos = Coord(bottomRight) - btnSize;
	
	Widget* mission = new ScreenButton(pos, 128, STANDARD_BUTTON_HEIGHT, BtnText("Start mission"));//new BoxedButton(Text("Start mission"), ImportImage_SmlButtonBack, bottomRight, nullptr);
	this->Control()->AddWidget(mission);
}

void ShipSelectTablet::HandleActiveWidget(SDL_Event& e, Widget* widget)
{
	BackNextTablet::HandleActiveWidget(e, widget);
	std::string txt = widget->ID();

	if (txt == "Start mission")
	{
		//GameManager::UpdatePlayerVesselFromProfile(pVesselName);
		//VesselGlobalList::SetPlayerVesselName(pVesselName);
		GameManager::Screens.closeOpenTablet();
		//GameManager::LoadLocation("Mission");
	}
}