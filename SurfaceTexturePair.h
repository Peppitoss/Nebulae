#pragma once
#include<SDL.h>
#include"Graphics.h"

class SurfaceTexturePair
{
public:
	SurfaceTexturePair();

	void GenerateSurface(std::shared_ptr<SDL_Surface> surface);
	void DrawSurface(SDL_Renderer* renderer, const Coord& position);
	void DrawSurface(SDL_Renderer* renderer, const Coord& position, double heading);

private:

	std::shared_ptr<SDL_Surface> pSurface;
	std::shared_ptr<SDL_Texture> pTexture;

	void GenerateTexture(SDL_Renderer* renderer);
	void DrawTexture(SDL_Renderer* renderer, const Coord& position);
	void DrawTexture(SDL_Renderer* renderer, const Coord& position, double heading);

};