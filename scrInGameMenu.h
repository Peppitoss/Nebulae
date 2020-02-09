#pragma once
#include "Screen.h"
//#include "GameManager.h"

//class GameManager;

class InGameMenu : public Screen
{
private:
	//GameManager* pParent;

	//Coord screenPosition;

	//int pScreenHeight;
	//int pScreenWidth;
public:

	InGameMenu() {};
	InGameMenu( int, int, Coord pos );
	

	//void render( SDL_Renderer& );
	void HandleActiveWidget( SDL_Event& e, Widget* );

	//GameManager* getParent() { return pParent; };
	virtual ~InGameMenu(void);
};

