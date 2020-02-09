#pragma once
#include"TextureObject.h"

class Mask
{
public:

	Mask();

	virtual void RunMask( double milliseconds);
	virtual void UpdateMaskTexture(SDL_Renderer* renderer);
	bool IsFinished() { return pRemainingTime <= 0; }

	std::shared_ptr<imagehandling::TextureObject_2> GetTexture() { return pTexture;	}
	//virtual void InitializeTexture(SDL_Renderer& renderer);

	bool RemoveWhenFinished() const { return this->pRemoveWhenFinished; }
	void SetRemoveWhenFinished(bool set) { this->pRemoveWhenFinished = set; }

protected:

	std::shared_ptr<imagehandling::TextureObject_2> pTexture;
	double pRemainingTime;
	bool pIsTimed;

private:

	bool pRemoveWhenFinished;
	void Initialize();
	
};