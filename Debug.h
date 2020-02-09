#pragma once
#include<SDL.h>

class Debug
{
public:
	static void SDLErrorCheck();
	static void DestroyTexture(SDL_Texture*);

};