#include"scrDestinations.h"
#include"BackgroundCollection.h"
#include"ScreenButton.h"
#include"GameManager.h"
#include"DestinationButton.h"
#include"LocTaurus.h"
#include"LocStarCruiser.h"
#include"LocLetevonShipyard.h"
#include"ShuttleAid_01.h"
#include"LostInSpace_02.h"
#include"AnotherRescue_03.h"

#include"MissionCaptionCollection.h"

scrDestination::scrDestination(int screen_width, int screen_height, Coord pos)
{
	pScreenWidth = screen_width;
	pScreenHeight = screen_height;

	pStretchBackgroundTextures = false;

	int spacing = screen_height / 10;
	int x_coord = screen_width / 3;
	int y_coord = screen_height / 5 + spacing;
	int rect_h = 30;
	int rect_w = x_coord;

	int y_shift = 0;
	int x_shift = 0;

	auto begin = std::shared_ptr<Widget>(new ScreenButton(Coord(300 + pos.y, 696 + pos.x), 80, rect_h, BtnText("Travel to destination")));			//new BoxedButton( "Begin", ImportImage_SmlButtonBeing, Coord(749+pos.y, 717+pos.x), StartClick));
	this->Control()->AddWidget(begin);

	auto back = std::shared_ptr<Widget>(new ScreenButton(Coord(300 + 96 + pos.y, 696 + pos.x), 64, rect_h, BtnText("Back")));
	this->Control()->AddWidget(back);



	this->SetBackground(ImportImage_DestinationMenuBorder, 255);
	this->AddBackgroundLayer(ImportImage_DestinationMenuScreen, 200);
	this->CenterScreenPositionBasedOnBackgroundImage();

	MapItemHeader = std::shared_ptr<Link>(new Link(Text("Header"), standardLinkFont.c_str(), 12, Coord(screenPosition.x + 42, screenPosition.y + 38), 24, 340, ColorRGB::White(), enTextPosition::centerTxt, true));
	MapItemDescription = std::shared_ptr<Link>(new Link(Text("Description"), standardLinkFont.c_str(), 12, Coord(screenPosition.x + 42, screenPosition.y + 62), 280, 340, ColorRGB::White(), enTextPosition::upperLeftTxt, true));

	this->Control()->AddWidget(MapItemHeader);
	this->Control()->AddWidget(MapItemDescription);



	this->Initialize();
}

void scrDestination::HandleWidgetByState(SDL_Event& e)
{
	for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); ++it)
	{
		if ((*it)->GetState() == WIDGET_STATE::WS_ACTIVE)
		{
			HandleActiveWidget(e, (*it).get());
			break;
		}
	}
}

void scrDestination::HandleActiveWidget(SDL_Event& e, Widget* widget)
{
	std::string txt = widget->ID();
	if (txt == "Back")
	{
		GameManager::Screens.closeDestination();
		return;
	}
	else if (txt == "Travel to destination")
	{
		GameManager::startMission();
		GameManager::TravelTo(pSelectedDestination);
		return;
	}

	HandleMissionSelect(txt);

}

void scrDestination::HandleMissionSelect(string buttontxt)
{
	for (int i = 0; i < this->pDestinations.size(); i++)
	{
		if (pDestinations[i].GetName() == buttontxt)
		{
			SelectDestination(pDestinations[i]);
			return;
		}
	}
}

void scrDestination::RemoveWidgetDestinations()
{
	bool erased;
	for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); )
	{
		erased = false;
		std::string txt = (*it)->ID();

		for (int i = 0; i < this->pDestinations.size(); i++)
		{
			if (pDestinations[i].GetName() == txt)
			{
				it = this->Control()->EraseWidget(it);
				erased = true;
				break;
			}
		}
		if (!erased)
			++it;


	}
}

void scrDestination::SelectDestination(const Location& location)
{
	MapItemHeader->RenewText(location.GetName());
	MapItemDescription->RenewText(location.GetDataCopy().GetDescription().GetCurrentValue());
	this->pSelectedDestination = location;
}

void scrDestination::AddDestination(Location data)
{
	if (HasDestination(data))
		return;

	int destinationNumber = pDestinations.size();
	int yStart = 42;
	int xStart = 420;
	int btnHeight = 52;

	auto begin = std::shared_ptr<Widget>(new DestinationButton(Coord(this->screenPosition.x + xStart, this->screenPosition.y + yStart + btnHeight* destinationNumber), GetLocationHeader(data), GetLocationValue(data)));
	begin->SetUseToggleClick(true);
	this->Control()->AddWidget(begin);
	pDestinations.push_back(data);

}

Text scrDestination::GetLocationHeader(const Location& data)
{
	BtnText prefix;
	//if(data.IsMission())
	//	return Text(BtnText("Story mission").GetCurrentValue() + " " + data.GetMissionName());
	////if (data.IsNeutralLocation())
	////	return Text(BtnText("Visit").GetCurrentValue() + " " + data.GetSectorName());
	//else
	return Text(BtnText("Auto mission").GetCurrentValue());// +" " + data.GetSectorName());

}

Text scrDestination::GetLocationValue(const Location& data)
{
	Text result = Text(data.GetName());
	result.SetID(data.GetName());
	return result;
}

void scrDestination::LoadDestinationScreen(enCampaignPlanets planet)
{
	RemoveWidgetDestinations();
	pDestinations = std::vector<Location>();
	SelectDestination(pDefaultDestination);
	switch (planet)
	{
	case Sun:
		break;
	case Mercury:
		break;
	case Venus:
		break;
	case Earth:
		//this->AddDestination(LocationTaurus());
		//this->AddDestination(*GameManager::GetStartLocationFromMission(MissionName::Mission_1).get());
		//this->AddDestination(Location_LostInSpace_02());
		//this->AddDestination(Location_AnotherRescue_03());
		//this->AddDestination(Location_Letevon_Shipyard());
		break;
	case Mars:
		break;
	case NoPlanet:
		break;
	default:
		break;
	}
}

bool scrDestination::HasDestination(Location data)
{
	for (int i = 0; i < this->pDestinations.size(); i++)
		if (this->pDestinations[i].GetName() == data.GetName())
			return true;
	return false;
}

void scrDestination::Initialize()
{
	pDestinations = std::vector<Location>();
	pDefaultDestination = Location();
	pDefaultDestination.SetName(Text("locationtxt", "Destination").GetCurrentValue());
	pDefaultDestination.GetData().SetDescription(MissionText("DefaultDescription"));
}


scrDestination::~scrDestination()
{
}
