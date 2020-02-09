#include"Rect.h"

Rect::Rect(int x, int y, int w, int h)
{
	pSdlRect = { x,y,w,h };
}

void Rect::Add(int x, int y, int w, int h)
{
	pSdlRect = IncreaseDimension(pSdlRect, x, y, w, h);
}

SDL_Rect Rect::IncreaseDimension(const SDL_Rect& rect, int x, int y, int w, int h)
{
	SDL_Rect result = { rect.x + x , rect.y + y, rect.w + w,  rect.h + h };
	return result;
}

SDL_Rect Rect::Translate(const SDL_Rect& rect, const Coord& amount)
{
	return Translate(rect, amount.x, amount.y);
}

SDL_Rect Rect::Translate(const SDL_Rect& rect, int x, int y)
{
	return IncreaseDimension(rect, x, y, 0, 0);
}