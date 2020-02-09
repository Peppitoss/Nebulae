#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "DrawClass.h"
#include "Graphics.h"
#include "ProceduralDesigner.h";
#include "Sprite.h";
#include "Integrator.h";
#include "Background.h";
#include "ParticleEngine.h"
#include "BehaviorTree.h"
#include "VesselAI.h"
#include "Animation.h"
#include "GameManager.h"
#include <string>     /* srand, rand */
#include <Windows.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Wrapper.h"
#include "Manager.h"

int MAP_WIDTH = 8000;
int MAP_HEIGHT = 6000;

//Loads media
bool loadMedia();

//Starts the SDLWrapper class, which initializes SDL in its constructor
Wrapper sdlWrapper = Wrapper();
//The window we'll be rendering to



//void close();
//bool SDLInit();

int main(int, char**) {

	srand (2);

	auto mMenuManager = new Manager();
	mMenuManager->Run();
	//GameManager* mGameManager = new GameManager();
	//mGameManager->Run();

	//Sprite importfrigate( ImportImage_frigate, spriteCollector, 100 );
	//Sprite importproject2( ImportImage_project, spriteCollector, 700 );
	//Sprite importhorny( ImportImage_horny, spriteCollector, 800 );



	return 0;
}




