#include "Manager.h"
#include "Globals.h";
#include"Debug.h"

void Manager::SDL_Init()
{
	pExit = false;
	mWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow("PixelGenerator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SDL_WINDOW_SHOWN),SDL_DestroyWindow);
	
	if ( mWindow == NULL )
	{
		throw std::runtime_error("Unable to create SDL_Window!");
	}
	mRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer( mWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE),SDL_DestroyRenderer);

	if ( mRenderer == NULL )
	{
		throw std::runtime_error("Unable to create SDL_Renderer!");
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	Debug::SDLErrorCheck();
}
Manager::Manager(void)
{
	SDL_Init();
	//MainMenu _main (DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH);

	this->mMenuManager = new MenuManager( mRenderer.get(), DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH );
	this->mGameManager = new GameManager( mRenderer.get(), DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH );

	//_screens.push(_main);
}

void Manager::Run()
{
	//bool quit = false;
	//while( !mGameManager->QuitGame() )
	//{
		//while(mMenuManager->Run())
		//{
		//}

		while(mGameManager->Run())
		{
		}
	//}

}
Manager::~Manager(void)
{

}
