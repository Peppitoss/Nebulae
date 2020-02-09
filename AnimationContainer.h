#pragma once
#include"Graphics.h"
#include<vector>
#include<string>
#include"Animation.h"

using std::string;

class AnimationContainer
{
public:

	AnimationContainer();

	bool DrawAnimations() const { return this->pDrawAnimations; };
	void SetAnimationDrawing(bool set) { this->pDrawAnimations = set; };
	bool HasAnimations() { return this->pModuleAnimations.size() > 0; };
	std::vector<FrameStripReader>& GetAnimationlist() { return this->pModuleAnimations; };
	void AddAnimation(std::shared_ptr<FrameStrip> strip) { this->pModuleAnimations.push_back(FrameStripReader(strip)); };
	void AddAnimation(FrameStripReader reader) { this->pModuleAnimations.push_back(reader); };
	std::vector<FrameStripReader>::const_iterator const AniFrame_begin() const { return pModuleAnimations.begin(); };
	std::vector<FrameStripReader>::const_iterator const AniFrame_end() const { return pModuleAnimations.end(); };

	FrameStripReader AnimationContainer::GetAnimation(int index) const;
	FrameStripReader AnimationContainer::GetAnimation(string ID) const;
	void AnimationContainer::RunAnimations(int ms);
	bool AnimationContainer::HasAnimationID(string ID) const;
	
	void ClearDeleteAfterUseFrames();

private:

	bool pDrawAnimations;
	std::vector<FrameStripReader> pModuleAnimations;

};