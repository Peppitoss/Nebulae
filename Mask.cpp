#include"Mask.h"

Mask::Mask()
{
	Initialize();
}

void Mask::Initialize()
{
	pTexture = nullptr;
	pRemainingTime = 0;
	pIsTimed = true;
	pRemoveWhenFinished = true;
}

void Mask::RunMask(double milliseconds)
{
	if (pIsTimed && IsFinished() == false)
	{
		pRemainingTime -= milliseconds;
	}
}

void Mask::UpdateMaskTexture(SDL_Renderer* renderer)
{

}


//void Mask::InitializeTexture(SDL_Renderer& renderer)
//{
//	this->GetTexture()->getTexture(&renderer);
//}