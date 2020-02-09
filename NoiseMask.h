#pragma once
#include"AnimationMask.h"

class NoiseMask : public AnimationMask
{

public:

	NoiseMask(SDL_Renderer* renderer);
	NoiseMask(SDL_Renderer* renderer, double timed, int opacity);

protected:

	//void RunAnimations(double milliseconds) override;
	void SetupAnimations(SDL_Renderer* renderer);

private:

	int pOpacity;

};