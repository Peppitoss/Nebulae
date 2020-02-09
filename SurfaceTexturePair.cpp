#include"SurfaceTexturePair.h"
#include"Debug.h"

SurfaceTexturePair::SurfaceTexturePair()
{
	pSurface = std::shared_ptr<SDL_Surface>();
	pTexture = std::shared_ptr<SDL_Texture>();
}

void SurfaceTexturePair::GenerateSurface(std::shared_ptr<SDL_Surface> surface)
{
	pSurface = surface;
	pTexture.reset();
}

void SurfaceTexturePair::GenerateTexture(SDL_Renderer* renderer)
{
	pTexture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, pSurface.get()), Debug::DestroyTexture);
	Debug::SDLErrorCheck();
}

void SurfaceTexturePair::DrawSurface(SDL_Renderer* renderer, const Coord& position)
{
	if (pTexture.get())
		DrawTexture(renderer, position);
	else if (pSurface.get())
	{
		GenerateTexture(renderer);
		DrawTexture(renderer, position);
	}
}

void SurfaceTexturePair::DrawSurface(SDL_Renderer* renderer, const Coord& position, double heading)
{
	if (pTexture.get())
		DrawTexture(renderer, position, heading);
	else if(pSurface.get())
	{
		GenerateTexture(renderer);
		DrawTexture(renderer, position, heading);
	}
}

void SurfaceTexturePair::DrawTexture(SDL_Renderer* renderer, const Coord& position, double heading)
{
	heading = geometry::radToDegrees(heading)*-1;

	int w, h;
	SDL_QueryTexture(this->pTexture.get(), NULL, NULL, &w, &h);
	Debug::SDLErrorCheck();

	SDL_Rect renderQuad = { position.x, position.y, w, h };
	SDL_Point rotPoint = { 0,0 };
	SDL_RenderCopyEx(renderer, this->pTexture.get(), NULL, &renderQuad, heading, &rotPoint, SDL_FLIP_NONE);
	Debug::SDLErrorCheck();
}

void SurfaceTexturePair::DrawTexture(SDL_Renderer* renderer, const Coord& position)
{
	int w, h;
	SDL_QueryTexture(this->pTexture.get(), NULL, NULL, &w, &h);
	Debug::SDLErrorCheck();

	SDL_Rect renderQuad = { position.x, position.y, w, h };
	SDL_RenderCopy(renderer, this->pTexture.get(), NULL, &renderQuad);
	Debug::SDLErrorCheck();
}

