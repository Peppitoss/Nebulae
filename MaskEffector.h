#pragma once
#include"Mask.h"

class MaskEffector
{


public:

	MaskEffector(SDL_Renderer* renderer);
	void Run(double ms);
	void RunUpdateMaskTextures();

	std::shared_ptr<Mask> GenerateNoiseMask(int opacity, int timed );
	std::shared_ptr<Mask> GenerateBlendMask(const ColorRGB& col, SDL_BlendMode mode, int opacity);
	std::shared_ptr<Mask> GenerateFadeAwayMask(const ColorRGB& col, int fadeTime);
	std::shared_ptr<Mask> GenerateFadeToColorMask(const ColorRGB& col, int fadeTime);

	std::vector<std::shared_ptr<Mask>>::const_iterator const Mask_begin() const { return pMasks.begin(); };
	std::vector<std::shared_ptr<Mask>>::const_iterator const Mask_end() const { return pMasks.end(); };

	int MaskCount() const { return this->pMasks.size(); };

	void ResetEffects();

private:

	std::vector<std::shared_ptr<Mask>> pMasks;
	SDL_Renderer* pRenderer;
	void RemoveMask(std::shared_ptr<Mask> mask);

};