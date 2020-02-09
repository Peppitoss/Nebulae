#pragma once
#include<SDL.h>
#include"Coord.h"

class Rect
{
public:

	Rect(int x, int y, int w, int h);

	void Add(int x, int y, int w, int h);

	static SDL_Rect IncreaseDimension(const SDL_Rect& rect, int x, int y, int w, int h);
	static SDL_Rect Translate(const SDL_Rect& rect, int x, int y);
	static SDL_Rect Translate(const SDL_Rect& rect, const Coord& amount);
	static SDL_Rect GetSDLRect(int x, int y, int w, int h) { return SDL_Rect{ x,y,w,h }; }

private:
	
	
	SDL_Rect pSdlRect;

};