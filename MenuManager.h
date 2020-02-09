#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "ScrMainMenu.h"
#include "ScrShipSelect.h"
#include <stack>

/////////////////////////////////////////////////
//											   //
//	     - Original Courtesy of S.G			   //
//											   //
/////////////////////////////////////////////////

enum enMenuScreen
{
	MAINMENU = 0,
	SHIPSELECT,
	HALT,
};

class MainMenu;
class ShipSelectMenu;

class MenuManager
{

public:


	MenuManager( SDL_Renderer* gRenderer, int windowWidth, int windowHeight );

	SDL_Renderer* getRenderer() { return this->pRenderer; };

	bool Run();
	ScreenMusic GetMusic() { return pScreens.top()->GetMusic(); };

	void btnPlayClick();
	void btnStartGameManager();

	~MenuManager();

private:

	enMenuScreen pMenuScreen;
	MainMenu* pMainScreen;
	ShipSelectMenu* pShipSelecScreen;


	int SCREEN_HEIGHT; 
	int SCREEN_WIDTH;

	std::stack<Screen*> pScreens;
	//MainMenu pMainMenu;

	SDL_Renderer* pRenderer;
	bool pExit;

};


