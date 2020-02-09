#include"DecoModule.h"
#include"Statistics.h"
#include"ModuleInterfaceGlobalList.h"


DecoModule::DecoModule()
	:ModuleEntity()
{
	Initialize();
}

DecoModule::DecoModule(std::shared_ptr<GraphicalEntity> graphicalentity, string name) : ModuleEntity(graphicalentity, name)
{
	Initialize();
}

void DecoModule::SetFrame(int frame)
{
	if (pFrameAmount < frame)
		return;

	pCurrentFrame = frame;
	this->GetModuleGraphics()->SetSpriteFrame(frame, true);
}

int DecoModule::DecoFramesAmount() const
{
	return this->GetModuleGraphics()->GetFrames();
}

void DecoModule::IncrementDecoFrame()
{
	pCurrentFrame = (pCurrentFrame + 1) % (pFrameAmount+1) + 1;
	this->GetModuleGraphics()->SetSpriteFrame(pCurrentFrame, true);
}

void DecoModule::ShuffleFrame()
{
	pCurrentFrame = statistics::GetIntInRange(1, pFrameAmount);
	this->GetModuleGraphics()->SetSpriteFrame(pCurrentFrame, true);
}

void DecoModule::RunDecoAnimation(int ms)
{
	pAccumulatedTime += ms;
	if (pAccumulatedTime > pAnimationSpeed)
	{
		pAccumulatedTime = 0;
		if (this->pShuffleFrames)
			this->ShuffleFrame();
		else
			this->IncrementDecoFrame();
	}
}

void DecoModule::AddGlobalListModule(std::string name, Coord pos, enObjectDiscreteRotation rotation, enObjectFlipped flipped)
{
	ModuleEntity* entity = ModuleGlobalList::GetModule(name);
	enModuleType mType = ModuleGlobalList::GetType(name);
	entity->SetFlipped(flipped);
	entity->SetRotation(rotation);

	entity->getGraphics()->SetDrawAfterParent(true);

	Coord anchorPoint = this->getDimension().GetDimension() / 2 - entity->getDimension().GetDimension() / 2; //just for graphics
	this->IncrementSimpleMass(entity->GetSimpleMass());
	this->IncrementMomentOfInertia(entity->GetSimpleMass(), geometry::realDistance(pos));

	switch (mType)
	{
	case(enModuleType::mDecoration):
		{
			std::shared_ptr<DecoModule> ptr_deco = std::shared_ptr<DecoModule>(dynamic_cast<DecoModule*>(entity));
			AddDecorator(ptr_deco, pos, anchorPoint);
			return;
		}
	}
}

//some code here identical to that of unit entity!!
void DecoModule::AddDecorator(std::shared_ptr<DecoModule> in, Coord relative_pos, Coord anchorPoint)
{
	SetupNewModuleGraphics(in, relative_pos, anchorPoint);
	this->pDecorationModules.push_back(in);
}

//some code here identical to that of unit entity!!
bool DecoModule::SetupNewModuleGraphics(std::shared_ptr<ModuleEntity> module, const Coord& pos, const Coord& anchorPoint)
{
	if (module->getGraphics()->getConnectedObjectsAmount() > 1)
	{
		//not implemented for a sprite tree of a linked main node
		return false;
	}
	InGameGraphicalObjectLink& generatedLink = this->getGraphics()->addGraphicalObject(module->getGraphics(), pos, anchorPoint, 0);
	module->setRelativePosition(generatedLink.relationalZeroHeadingPosition);
	module->SetID(generatedLink.GetID());
	this->pCurrentModuleID = generatedLink.GetID()+1;
	return true;
}

void DecoModule::Initialize()
{
	pShuffleFrames = 0;
	pCurrentFrame = 0;
	pAccumulatedTime = 0;
	pFrameAmount = DecoFramesAmount();
	pAnimationSpeed = 0;
	pCurrentModuleID = 0;
	//this->pGraphics->getGraphicalObject()->getMainObject()->SetCollisionIsEnabled(this->reqCollisionDetect);
}