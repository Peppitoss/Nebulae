#include"AnimationContainer.h"

AnimationContainer::AnimationContainer()
{
	pDrawAnimations = true;
	pModuleAnimations = std::vector<FrameStripReader>();
}


FrameStripReader AnimationContainer::GetAnimation(int index) const
{
	if (this->pModuleAnimations.size() <= index)
		return nullptr;
	else
		return this->pModuleAnimations[index];
}

FrameStripReader AnimationContainer::GetAnimation(string ID) const
{
	for (auto it = this->pModuleAnimations.begin(); it != this->pModuleAnimations.end(); ++it)
		if ((*it).Strip->GetIDTag() == ID)
			return *it;
	return nullptr;
}

void AnimationContainer::RunAnimations(int ms)
{
	this->pDrawAnimations = true;
	int size = this->pModuleAnimations.size();

	for (int i = 0; i < size; i++)
		this->pModuleAnimations[i].RunFrame(ms);
}

bool AnimationContainer::HasAnimationID(string ID) const
{
	int size = this->pModuleAnimations.size();
	for (int i = 0; i < size; i++)
		if (this->pModuleAnimations[i].Strip->GetIDTag() == ID)
			return true;
}

//void AnimationContainer::ChangeAnimationPositions(const Coord& change)
//{
//	for (auto it = this->pModuleAnimations.begin(); it != this->pModuleAnimations.end(); ++it)
//	{
//		it->ChangePosition(change);
//
//	}
//}

void AnimationContainer::ClearDeleteAfterUseFrames()
{
	std::vector<FrameStripReader> temp = std::vector<FrameStripReader>();// this->pModuleAnimations;

	for (auto it = this->pModuleAnimations.begin(); it != this->pModuleAnimations.end(); ++it)
	{
		if ((*it).Strip->IsActive() == false && (*it).DeleteAfterUse())
		{
			(*it).ClearData();
		}
		else
		{
			temp.push_back((*it));
		}
	}
			
	this->pModuleAnimations = temp;
}