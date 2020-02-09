#pragma once
#include"ScreenHandler.h"

//static void CloseTablet(SDL_Event& e, Widget* button);

enum ScreenTabletType
{
	stSingleButton = 1,
	stNextBackButtons = 2,
	stShipSelectButton = 3,
	stError

};

class ScreenTablet : public ScreenHandler
{

public:

	ScreenTablet();
	ScreenTablet(int, int, Coord pos, bool stretch, const imagehandling::IMGToSurface*, ScreenTabletType type);

	bool HasErrorScreens();
	static string ErrorScreenName() { return "errorMessenger"; }
	
private:

	void Initialize();
	ScreenTabletType pTabletType;

};