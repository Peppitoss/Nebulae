#pragma once
#include"Mask.h"
#include"Animation.h"

class AnimationMask : public Mask
{

public:

	AnimationMask(SDL_Renderer* renderer);
	AnimationMask(SDL_Renderer* renderer, double timer );

	void RunMask( double milliseconds) override;
	virtual void UpdateMaskTexture(SDL_Renderer* renderer);

protected:

	virtual void SetupAnimations(SDL_Renderer* renderer) = 0;
	virtual void RunAnimations(double milliseconds);
	virtual void DrawTexture(FrameStripReader&, SDL_Renderer* renderer);

	std::vector<FrameStripReader> pAnimations;

private:

	void Initialize(SDL_Renderer* renderer);
	bool pTimed;
	//SDL_Window* pWindow;
	
};