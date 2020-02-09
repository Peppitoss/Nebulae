#pragma once
#include"Globals.h"
#include"GameManager.h"
#include"MenuManager.h"

/////////////////////////////////////////////////
//											   //
//	     - Original Courtesy of S.G			   //
//											   //
/////////////////////////////////////////////////

class Manager
{
private:
	bool pExit;
	//SpriteDrawer* pixelDraw;
	std::shared_ptr<SDL_Renderer> mRenderer;
	std::shared_ptr<SDL_Window> mWindow;
	//std::stack<Screen> _screens;

	MenuManager* mMenuManager;
	GameManager* mGameManager;
public:
	void SDL_Init();
	void Run();
	Manager(void);
	~Manager(void);
};

