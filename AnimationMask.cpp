
#include"AnimationMask.h"
#include"Globals.h"
#include"Debug.h"

AnimationMask::AnimationMask(SDL_Renderer* renderer) : Mask()
{
	pTimed = false;
	Initialize(renderer);

}

AnimationMask::AnimationMask(SDL_Renderer* renderer, double timer) : Mask()
{
	pTimed = timer > 0;
	if (!pTimed)
		pRemainingTime = 1;
	else
		pRemainingTime = timer;
	Initialize(renderer);
}

void AnimationMask::RunMask( double milliseconds)
{
	if (IsFinished())
		return;

	if (pTimed)
		pRemainingTime -= milliseconds / 1000;

	RunAnimations(milliseconds);
	
}

void AnimationMask::RunAnimations(double milliseconds)
{
	for (int i = 0; i < this->pAnimations.size(); i++)
	{
		pAnimations[i].RunFrame(milliseconds);
	}
}

void AnimationMask::UpdateMaskTexture(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, this->pTexture->getTexture());
	Debug::SDLErrorCheck();

	for (int i = 0; i < this->pAnimations.size(); i++)
	{
		DrawTexture(pAnimations[i], renderer);
	}

	SDL_SetRenderTarget(renderer, NULL);
	Debug::SDLErrorCheck();
}

void AnimationMask::DrawTexture(FrameStripReader& framestrip, SDL_Renderer* renderer)
{
	int size = framestrip.Strip->getSize();
	int frame = framestrip.GetCurrentFrame();
	SDL_Texture * texture = framestrip.Strip->getTexture(renderer);

	SDL_Rect* renderClip = new SDL_Rect();
	renderClip->x = size*frame;
	renderClip->y = 0;
	renderClip->w = size;
	renderClip->h = size;

	SDL_Rect renderQuad = { 0, 0, renderClip->w, renderClip->h };
	SDL_RenderCopy(renderer, texture, renderClip, NULL);
	Debug::SDLErrorCheck();
}

void AnimationMask::Initialize(SDL_Renderer* renderer)
{
	pTexture = std::shared_ptr<imagehandling::TextureObject_2>(new imagehandling::TextureObject_2(new imagehandling::IMGToSurface(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, ColorRGB(100, 100, 100))));
	SDL_SetTextureBlendMode(pTexture->getTexture(renderer), SDL_BlendMode::SDL_BLENDMODE_BLEND);
	Debug::SDLErrorCheck();
	SDL_SetTextureAlphaMod(pTexture->getTexture(renderer), 100);
	Debug::SDLErrorCheck();
}

