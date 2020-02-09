#include"NoiseMask.h"
#include"AnimationCollection.h"
#include"Debug.h"

NoiseMask::NoiseMask(SDL_Renderer* renderer)
	: AnimationMask(renderer)
{
	pOpacity = 0;
	SetupAnimations(renderer);
}

NoiseMask::NoiseMask(SDL_Renderer* renderer, double timer, int opacity )
	: AnimationMask(renderer, timer)
{
	pOpacity = opacity;
	SetupAnimations(renderer);
}

void NoiseMask::SetupAnimations(SDL_Renderer* renderer)
{
	auto frame = ImportFrame_mExplosion_3;
	auto pNoiseMask = new FrameStrip(frame->getFrames(), 200, &*ImportFrame_mNoise_1);
	FrameStripReader reader = FrameStripReader(std::shared_ptr<FrameStrip>(pNoiseMask));
	reader.SetDeleteAfterUse(true);
	reader.SetRepeat(true);
	SDL_SetTextureAlphaMod(pNoiseMask->getTexture(renderer), this->pOpacity);
	Debug::SDLErrorCheck();
	this->pAnimations.push_back(reader);

	//for (int i = 0; i < this->pAnimations.size(); i++)
	//{
	//	pAnimations[i].getTexture(&*pRenderer);
	//}
}

//void NoiseMask::RunAnimations(double milliseconds)
//{
//
//
//}