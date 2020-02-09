#include"ScrCampaign.h"
#include"MissionCaptionCollection.h"
#include"ToggleButton.h"
#include"GameManager.h"
#include"BackgroundCollection.h"
#include"ScreenButton.h"

ScrCampaign::ScrCampaign( int screen_width, int screen_height, Coord pos )
{
	pScreenWidth = screen_width;
	pScreenHeight = screen_height;

	pStretchBackgroundTextures = false;

	int spacing = screen_height/10;
	int x_coord = screen_width/3;
	int y_coord = screen_height/5+spacing;
	int rect_h = STANDARD_BUTTON_HEIGHT;
	int rect_w = x_coord;

	int y_shift = 0;
	int x_shift = 0;
	
	auto begin = std::shared_ptr<Widget>(new ScreenButton(Coord(749 + pos.y, 717 + pos.x), 128, STANDARD_BUTTON_HEIGHT, BtnText("Begin")));			//new BoxedButton( "Begin", ImportImage_SmlButtonBeing, Coord(749+pos.y, 717+pos.x), StartClick));
	this->Control()->AddWidget(begin);
	y_coord += STANDARD_BUTTON_HEIGHT*1.5;

	auto back = std::shared_ptr<Widget>(new ScreenButton(Coord(749 + 110 + pos.y, 717 + pos.x), 128, STANDARD_BUTTON_HEIGHT, BtnText("Back")));
	this->Control()->AddWidget(back);
	y_coord += STANDARD_BUTTON_HEIGHT*1.5;

	
	//auto mission_1 = std::shared_ptr<Widget>(new ScreenButton(Coord(552 - 13 + pos.y, 256 + pos.x), 128, STANDARD_BUTTON_HEIGHT, BtnText("Mission1"), StartClick));	//This should be "toggle button" its the marker on the campaign map
	//this->Control()->AddWidget(mission_1);
	//y_coord += STANDARD_BUTTON_HEIGHT*1.5;

	auto mercuryIcon = std::shared_ptr<Widget>(new ScreenButton(GetPositionOfPlanet(enCampaignPlanets::Mercury, 45), ImportImage_MapButtonMercury, "Mercury"));	//new ToggleButton( "M2", ImportImage_MapButtonMission, Coord(552-128+pos.y, 256+pos.x), StartClick));
	this->Control()->AddWidget(mercuryIcon);
	mercuryIcon->SetUseToggleClick(true);
	
	auto earthIcon = std::shared_ptr<Widget>(new ScreenButton(GetPositionOfPlanet(enCampaignPlanets::Earth, 110), ImportImage_MapButtonEarth, "Earth"));	//new ToggleButton( "M2", ImportImage_MapButtonMission, Coord(552-128+pos.y, 256+pos.x), StartClick));
	this->Control()->AddWidget(earthIcon);
	earthIcon->SetUseToggleClick(true);

	auto marsIcon = std::shared_ptr<Widget>(new ScreenButton(GetPositionOfPlanet(enCampaignPlanets::Mars, 90), ImportImage_MapButtonMars, "Mars"));	//new ToggleButton( "M2", ImportImage_MapButtonMission, Coord(552-128+pos.y, 256+pos.x), StartClick));
	this->Control()->AddWidget(marsIcon);
	marsIcon->SetUseToggleClick(true);

	auto venusIcon = std::shared_ptr<Widget>(new ScreenButton(GetPositionOfPlanet(enCampaignPlanets::Venus, 280), ImportImage_MapButtonVenus, "Venus"));	//new ToggleButton( "M2", ImportImage_MapButtonMission, Coord(552-128+pos.y, 256+pos.x), StartClick));
	this->Control()->AddWidget(venusIcon);
	venusIcon->SetUseToggleClick(true);

	MapItemHeader = std::shared_ptr<Link>(new Link(Text("Header"), standardLinkFont.c_str(), 12, Coord(970, 102), 64, 198, ColorRGB::White(), enTextPosition::centerTxt, true));
	MapItemDescription = std::shared_ptr<Link>(new Link(Text("Description"), standardLinkFont.c_str(), 12, Coord(970, 212), 320, 198, ColorRGB::White(), enTextPosition::upperLeftTxt, true));

	this->Control()->AddWidget(MapItemHeader);

	//auto MapItemDescription = std::shared_ptr<Widget>(new TextBox( "Description 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16", 12, Coord(970,212), 345, 198) );
	this->Control()->AddWidget(MapItemDescription);

	//auto exit = std::shared_ptr<Widget>(new BoxedButton( "Exit", ImportImage_MenuButtonExit, Coord(552-13+pos.y, 512+6+pos.x), MissionClick));
	//this->Control()->AddWidget(exit);
	//y_coord += SurfaceButton::SURFACE_BUTTON_HEIGHT*1.5;

	//pParent = parent;

	this->SetBackground(ImportImage_CampaignMenuBorder, 255 );
	this->AddBackgroundLayer(ImportImage_CampaignMenuScreen, 200 );
	this->CenterScreenPositionBasedOnBackgroundImage();
	this->Initialize();
}

void ScrCampaign::HandleWidgetByState( SDL_Event& e )
{
	pMissionNumberSelected = -1;
	for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); ++it)
	{
		//if( (*it)->GetState() == WIDGET_STATE::WS_OFF )
		//{
		//	HandleInactiveWidget( e, (*it).get() );
		//	break;
		//}
		if( (*it)->GetState() == WIDGET_STATE::WS_ACTIVE )
		{
			HandleActiveWidget( e, (*it).get() );
			return;
		}
	}
	if (pMissionNumberSelected == -1)
		this->SelectNoPlanet();
}

bool ScrCampaign::IsWidgetMissionButton( Widget* widget )
{
	std::string txt = widget->ID();
	if( txt[0] == 'M' && txt[1] >= 49 && txt[1] <= 57 )
		return true;
	return false;
}

void ScrCampaign::HandleActiveWidget( SDL_Event& e, Widget* widget )
{
	std::string txt = widget->ID();
	if(txt == "Back" )
	{
		GameManager::Screens.quitToMenu(false);	//quit to mainmenu
		return;
	}
	else if(txt == "Begin" )
	{
		GameManager::Screens.openDestination( this->pSelectedPlanet);
		return;
	}

	HandleMissionSelect( txt );

}

void ScrCampaign::HandleMissionSelect( string buttontxt )
{
	if(buttontxt == "Earth" )
	{
		SelectEarth();
	}
	else if(buttontxt == "Mars" )
	{
		SelectMars();
	}
}

void ScrCampaign::SelectEarth()
{
	pMissionNumberSelected = 1;
	MapItemHeader->RenewText(MissionText("Planet Earth").GetCurrentValue());
	MapItemDescription->RenewText(MissionText("PlanetEarthDescription").GetCurrentValue());
	pSelectedPlanet = enCampaignPlanets::Earth;
	//MapItemDescription->AutoResize();
}

void ScrCampaign::SelectMars()
{
	pMissionNumberSelected = 2;
	MapItemHeader->RenewText(MissionText("Planet Mars").GetCurrentValue());
	MapItemDescription->RenewText(MissionText("PlanetMarsDescription").GetCurrentValue());
	pSelectedPlanet = enCampaignPlanets::Mars;
	//MapItemDescription->AutoResize();
}

void ScrCampaign::SelectNoPlanet()
{
	pMissionNumberSelected = -1;
	MapItemHeader->RenewText(MissionText("The Solar System").GetCurrentValue());
	MapItemDescription->RenewText(MissionText("SolarSystemDescription").GetCurrentValue());
	pSelectedPlanet = enCampaignPlanets::NoPlanet;
}

void ScrCampaign::Initialize()
{
	this->pMissionNumberSelected = -1;
}

Coord ScrCampaign::PlanetariumCenterPosition()
{
	return Coord(this->pScreenWidth/2 + 240, this->pScreenHeight / 2 - 84);
}
Coord ScrCampaign::GetPositionOfPlanet(enCampaignPlanets planet, double angle)
{
	//Coord result;

	return geometry::GetPointFromCircle(PlanetariumCenterPosition(), GetPlanetRadius(planet), angle);

	//return result;
}

double ScrCampaign::GetPlanetRadius(enCampaignPlanets planet)
{
	switch (planet)
	{
	case Sun:
		return 0;
	case Mercury:
		return 34;
	case Venus:
		return 55;
	case Earth:
		return 80;
	case Mars:
		return 105;
	default:
		break;
	}
}

ScrCampaign::~ScrCampaign()
{
}
