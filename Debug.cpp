#include"Debug.h"
#include<SDL.h>

void Debug::SDLErrorCheck()
{
	auto err = SDL_GetError();
	if (err != nullptr && err[0] != '\0')
	{
		SDL_ClearError();
	}
}

void Debug::DestroyTexture(SDL_Texture* texture)
{
	SDL_DestroyTexture(texture);
	SDLErrorCheck();
}
