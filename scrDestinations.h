#pragma once
#include "Screen.h"
#include"TextBox.h"
#include"Location.h"

class scrDestination : public Screen
{
public:

	scrDestination() {};
	scrDestination(int, int, Coord pos);

	void HandleWidgetByState(SDL_Event& e) override;
	void HandleActiveWidget(SDL_Event& e, Widget*) override;

	void AddDestination(Location data);

	void LoadDestinationScreen(enCampaignPlanets planet);

	//void HandleInactiveWidget( SDL_Event& e, Widget* );
	virtual ~scrDestination(void);

private:
	void Initialize();

	void HandleMissionSelect(string buttontxt);
	void RemoveWidgetDestinations();
	bool HasDestination(Location data);

	void SelectDestination(const Location& data);
	Text GetLocationHeader(const Location& data);
	Text GetLocationValue(const Location& data);

	Location pSelectedDestination;
	
	Location pDefaultDestination;
	std::vector<Location> pDestinations;
	std::shared_ptr<Link> MapItemHeader;
	std::shared_ptr<Link> MapItemDescription;
};

