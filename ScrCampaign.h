#pragma once
#include "Screen.h"
#include"TextBox.h"

class ScrCampaign : public Screen
{
public:

	ScrCampaign() {};
	ScrCampaign( int, int, Coord pos );
	

	void HandleWidgetByState( SDL_Event& e ) override;
	void HandleActiveWidget( SDL_Event& e, Widget* ) override;
	//void HandleInactiveWidget( SDL_Event& e, Widget* );
	virtual ~ScrCampaign(void);

private:

	Coord PlanetariumCenterPosition();
	Coord GetPositionOfPlanet(enCampaignPlanets planet, double angle);
	double GetPlanetRadius(enCampaignPlanets planet);

	void Initialize();
	bool IsWidgetMissionButton( Widget* );
	void HandleMissionSelect( std::string buttontxt );

	void SelectEarth();
	void SelectMars();
	void SelectNoPlanet();

	int pMissionNumberSelected;

	enCampaignPlanets pSelectedPlanet;

	std::shared_ptr<Link> MapItemHeader;
	std::shared_ptr<Link> MapItemDescription;
};

