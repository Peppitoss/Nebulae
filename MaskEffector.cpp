#include"MaskEffector.h"
#include"Graphics.h"
#include"FadeAwayMask.h"
#include"BlendMask.h"
#include"NoiseMask.h"
#include"FadeToColorMask.h"

MaskEffector::MaskEffector(SDL_Renderer* renderer)
{
	this->pRenderer = renderer;
}

void MaskEffector::Run(double ms)
{
	for (int i = 0; i < this->pMasks.size(); i++)
	{
		if (this->pMasks[i]->IsFinished() == false)
			this->pMasks[i]->RunMask(ms);
		else if(this->pMasks[i]->RemoveWhenFinished())
			RemoveMask(this->pMasks[i]);
	}
}

void MaskEffector::RunUpdateMaskTextures()
{
	for (int i = 0; i < this->pMasks.size(); i++)
	{
		this->pMasks[i]->UpdateMaskTexture(this->pRenderer);
	}
}

void MaskEffector::RemoveMask(std::shared_ptr<Mask> mask)
{
	typename std::vector<std::shared_ptr<Mask>>::iterator it = std::find(pMasks.begin(), pMasks.end(), mask);
	if (it != pMasks.end())
	{
		using std::swap;
		swap(*it, pMasks.back());
		pMasks.pop_back();
	}
}

void MaskEffector::ResetEffects()
{
	for (int i = 0; i < this->pMasks.size(); i++)
	{
		this->pMasks[i].reset();
	}
	this->pMasks.clear();
}

std::shared_ptr<Mask> MaskEffector::GenerateNoiseMask(int opacity, int timed)
{
	if (this->pRenderer == nullptr)
		return nullptr;

	auto result = std::shared_ptr<Mask>(new NoiseMask(this->pRenderer, timed, opacity));
	pMasks.push_back(result);
	return result;
}

std::shared_ptr<Mask> MaskEffector::GenerateBlendMask(const ColorRGB& col, SDL_BlendMode mode, int opacity)
{
	if (this->pRenderer == nullptr)
		return nullptr;

	auto result = std::shared_ptr<Mask>(new BlendMask(mode, col, opacity));
	pMasks.push_back(result);
	return result;
}

std::shared_ptr<Mask> MaskEffector::GenerateFadeAwayMask(const ColorRGB& col, int fadeTime)
{
	if (this->pRenderer == nullptr)
		return nullptr;

	auto result = std::shared_ptr<Mask>(new FadeAwayMask(fadeTime, col));
	pMasks.push_back(result);
	return result;
}
std::shared_ptr<Mask> MaskEffector::GenerateFadeToColorMask(const ColorRGB& col, int fadeTime)
{
	if (this->pRenderer == nullptr)
		return nullptr;

	auto result = std::shared_ptr<Mask>(new FadeToColor(fadeTime, col));
	pMasks.push_back(result);
	return result;
}