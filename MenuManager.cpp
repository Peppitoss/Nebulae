#include"MenuManager.h"


MenuManager::MenuManager(SDL_Renderer* gRenderer, int windowHeight , int windowWidth)
{
	SCREEN_HEIGHT = windowHeight;
	SCREEN_WIDTH = windowWidth;
	pRenderer = gRenderer;

	//pMainScreen = new MainMenu( this, SCREEN_HEIGHT, SCREEN_WIDTH); 
	pShipSelecScreen = new ShipSelectMenu( this, SCREEN_HEIGHT, SCREEN_WIDTH );

	//pScreens.push( pMainScreen );



	pExit = false;
	pMenuScreen = enMenuScreen::MAINMENU;

}


bool MenuManager::Run()
{

	SDL_Event e;

	//while (!pExit)
	//{
		Screen* topScreen = pScreens.top();
		ScreenMusic music = topScreen->GetMusic();

		if (music != NULL && !Mix_PlayingMusic())
		{
			Mix_PlayMusic(music.get(), -1);
		}
		else if( music == NULL )
		{
			Mix_HaltMusic();
		}

		while( SDL_PollEvent( &e ) != 0 )
		{

			topScreen->HandleEvent ( e );
			topScreen->render( *pRenderer);
			SDL_RenderPresent(pRenderer);
        }

		if( this->pMenuScreen == enMenuScreen::HALT )
			return false;

		return true;
	//}

	//if ( Mix_PlayingMusic() )
	//{
	//	Mix_HaltMusic();
	//}

}

void MenuManager::btnPlayClick()
{
	//pScreens.pop();
	pScreens.push( pShipSelecScreen );
	pMenuScreen = enMenuScreen::SHIPSELECT;
	
}

void MenuManager::btnStartGameManager()
{
	pMenuScreen = enMenuScreen::HALT;
}

MenuManager::~MenuManager()
{
	delete pShipSelecScreen;
	delete pMainScreen;
}

//void MainMenuClickEvent(void* object, enMainMenuClick click)
//{
//
//	switch( click )
//	{
//	case( enMainMenuClick::PLAY ) :
//		static_cast<MenuManager*>(object)->btnPlayClick();
//	break;
//		default :
//			break;
//
//
//	}
//	
//
//}