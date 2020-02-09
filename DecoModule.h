#pragma once
#include"ModuleEntity.h"

class DecoModule : public ModuleEntity
{

public:

	DecoModule::DecoModule();
	DecoModule::DecoModule(std::shared_ptr<GraphicalEntity> graphicalentity, string name);

	void SetFrame(int frame);
	int DecoFramesAmount() const;
	void RunDecoAnimation(int);

	void ShuffleFrame();
	void IncrementDecoFrame();
	int GetAnimationSpeed() { return this->pAnimationSpeed; }
	void SetAnimationSpeed(int set) { this->pAnimationSpeed = set; }
	void AddGlobalListModule(std::string name, Coord pos, enObjectDiscreteRotation rotation, enObjectFlipped flipped);

	std::vector<std::shared_ptr<DecoModule>>::const_iterator const Deco_begin() const { return pDecorationModules.begin(); };
	std::vector<std::shared_ptr<DecoModule>>::const_iterator const Deco_end() const { return pDecorationModules.end(); };

private:

	void AddDecorator(std::shared_ptr<DecoModule> in, Coord relative_pos, Coord anchorPoint);
	bool SetupNewModuleGraphics(std::shared_ptr<ModuleEntity> module, const Coord& pos, const Coord& anchorPoint);



	std::vector<std::shared_ptr<DecoModule>> pDecorationModules;

	int pCurrentModuleID;
	bool pShuffleFrames;
	int pAccumulatedTime;
	int pCurrentFrame;
	void Initialize();
	int pAnimationSpeed;
	int pFrameAmount;


};