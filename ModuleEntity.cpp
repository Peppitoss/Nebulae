#include"ModuleEntity.h"
#include"RandomNumberGenerator.h"
#include"GridObjectRoom.h"
#include"Globals.h"
#include"GameManager.h"
#include"ModuleInterfaceGlobalList.h"
#include"SurfaceGlobalList.h"
#include"UnitModuleContainer.h"
#include"StringBuilder.h"
#include <stdexcept>

ModuleDefinition::ModuleDefinition()
{
	this->ModuleType = enModuleType::mNone;
}

ModuleEntity::ModuleEntity() : DynamicEntity() , InventoryItem(enItemType::InternalModuleItem)
{
	pRelativePosition = NULL;
	Initialize();
}

ModuleEntity::ModuleEntity( std::shared_ptr<GraphicalEntity> ge, string name ) : DynamicEntity(ge), InventoryItem(enItemType::InternalModuleItem)
{
	pRelativePosition = NULL;
	Initialize();
	IDName = name;
}

//ModuleEntity::ModuleEntity(std::shared_ptr<FiguredGraphics> ge) : GenericEntity(ge) 
//{
//	pRelativePosition = NULL;
//	Initialize();
//}

void ModuleEntity::Initialize()
{
	IDName = "Noname";
	this->SetHullHitPoints(game::enHitPointType::HP_STRUCTURAL, 100, true);
	this->SetHullHitPoints(game::enHitPointType::HP_MATERIAL, 100, true);
	pEfficiency = 100;
	this->SetModuleType( enModuleType::mNone );
	this->pDisabled = false;
	this->pDetached = false;
	this->particleTriggerTime = 10;
	this->SetParticleEffect(EnumParticleEffect::noeffect);
	this->SetStepLocomotionSpeed(0);
	this->pStepLocomotion = EntityStepLocomotion(0, enStepExecution::aniTime);
	this->pIncludeHPInTooltip = true;

	this->pPrice = EntityPrice();
	//this->SetID(-1);	//don't change!

	//this->ApplyDestroyedModuleImage();	//for testing
}

void ModuleEntity::GenerateModuleGridBox(int gridsize = 64)
{
	if (this->GetFigure().get() && pGridObject.get() == false)
		this->generateGridObjectForModule(this->GetFigure()->GetBoxes(), SurfaceGlobalList::GetSurface(ModuleGlobalList::GetInterface(this->GetIDName()).GetSurfaceName() + "_").get(), gridsize);
}

void ModuleEntity::SetEfficiency( double set )
{
	this->pEfficiency = set;
}
//
//void ModuleEntity::IncludeSellValueInTooltip(bool value)
//{
//	if (this->pIncludeSellValueInTooltip == value)
//		return;
//
//	this->pIncludeSellValueInTooltip = value;
//	if(value)
//		this->pIncludePriceInTooltip = !value;
//
//	generateGridObjectDefaultToolTip();
//}
//void ModuleEntity::IncludePriceValueInTooltip(bool value)
//{
//	if (this->pIncludePriceInTooltip == value)
//		return;
//
//	this->pIncludePriceInTooltip = value;
//	if (value)
//		this->pIncludeSellValueInTooltip = !value;
//
//	generateGridObjectDefaultToolTip();
//}

void ModuleEntity::AddDamageRecieved(const game::Damage& set )
{
	GenericEntity::AddDamageRecieved( set );
	this->SetEfficiency( this->getPercentHullLeft()*100 );
	if (this->getPercentHullLeft() < 0.2)
	{
		this->ApplyDestroyedModuleImage();
	}
	else if(this->getPercentHullLeft() < 0.4)
	{
		this->ApplyDamagedModuleImage();
	}
};

//void ModuleEntity::SetDamageRecieved(const game::Damage& set )
//{
//	GenericEntity::SetHullHitPoints(set);
//	this->SetEfficiency( this->getPercentHullLeft()*100 );
//};

double ModuleEntity::GetEfficiency() 
{
	return pEfficiency/100;
};

//void ModuleEntity::generateGridObjectForModule( const BoxList& gridPos )
//{
//	pGridGraphics = std::shared_ptr<GridObject>(new GridObject( gridPos )); 
//}

void ModuleEntity::TriggerParticleEffect( int milliseconds, const Coord offset)
{
	if (this->enParticleEffect == EnumParticleEffect::particledustspread)
	{
		this->particleTriggerTime += milliseconds;
		if ((this->particleTriggerTime - 1600) > 0)
		{
			this->particleTriggerTime = 0;
			//SDL_SemWait(particleEffects::sem);
			GameManager::GetParticleEngine()->AddNewEffect( GameManager::GetParticleEngine()->GetEffectDustSpread(this->getRelativePosition() + offset, 0.3, 2,0, 8000));	//32 is size of vessel/2
			//SDL_SemPost(particleEffects::sem);
		}
	}
}

//void ModuleEntity::generateGridObjectForModule( BoxList list, const GridObjectTexturePack& pack, int shipSize )
//{
//	pGridGraphics = std::shared_ptr<GridObject>(new GridObjectRoom( list, pack ));
//	pGridGraphics.get()->ObjectOrigo = Coord(std::ceil(((double)list.getWidth())/2), (list.getHeight())/2 ); //Coord(std::floor(((double)list.getWidth())/2), std::floor(((double)list.getHeight())/2) );
//	pGridGraphics.get()->GridOrigo = Coord(shipSize/2+list.getWidth()/2,shipSize/2+list.getHeight()/2);
//	pGridGraphics.get()->setPlacementOrigoDependent(true);
//}
//
//void ModuleEntity::generateGridObjectForModule(  const BoxList& list,const imagehandling::IMGToSurface* model, int shipSize )
//{
//	pGridGraphics = std::shared_ptr<GridObject>(new GridObject( list, spriteSize(STANDARDOBJECTSIZE),model ));
//	pGridGraphics.get()->ObjectOrigo = Coord(std::ceil(((double)list.getWidth())/2), (list.getHeight())/2 ); //why ceiling?? and not floor?
//	pGridGraphics.get()->GridOrigo = Coord(shipSize/2+list.getWidth()/2,shipSize/2+list.getHeight()/2);
//	pGridGraphics.get()->setPlacementOrigoDependent(true);
//}
//
//void ModuleEntity::generateGridObjectForModule(  const BoxList& list, imagehandling::IMGToSurface* model, int shipSize )
//{
//	pGridGraphics = std::shared_ptr<GridObject>(new GridObject( list, spriteSize(STANDARDOBJECTSIZE),model ));
//	pGridGraphics.get()->ObjectOrigo = Coord(std::ceil(((double)list.getWidth())/2), (list.getHeight())/2 ); //why ceiling?? and not floor?
//	pGridGraphics.get()->GridOrigo = Coord(shipSize/2+list.getWidth()/2,shipSize/2+list.getHeight()/2);
//	pGridGraphics.get()->setPlacementOrigoDependent(true);
//}

void ModuleEntity::generateGridObjectForModule( CoordMap<GriddedBox>& list, imagehandling::IMGToSurface* model, int shipSize )
{
	list.OptimizeIteratorConstraints();
	pGridObject = std::shared_ptr<GridObject>(new GridObject( list, spriteSize(STANDARDOBJECTSIZE),model ));
	pGridObject.get()->ObjectOrigo = Coord(std::ceil(((double)list.GetOptimiziedDimension().x)/2), (list.GetOptimiziedDimension().y)/2 ); //why ceiling?? and not floor?
	pGridObject.get()->SetNewGridOrigo( shipSize ); //Coord(shipSize/2+list.GetMaxCoordValues().x/2,shipSize/2+list.GetMaxCoordValues().y/2);
	pGridObject.get()->setPlacementOrigoDependent(true);
	pGridObject.get()->SetCanRotate(true);
	pGridObject->SetIDName(this->GetIDName());
}

void ModuleEntity::SetGridObjectRestriction( const BoxList& list ) //assumes list has smallest Coord of 0
{
	pGridObject.get()->SetBoxRed( list );
}

void ModuleEntity::RefreshGridObjectColour()	//color only modulate!! 
{
	/*if(this->getGraphics()->getGraphicalObject()->getMainObject()->hasFigure() == false )
		return;*/
	
	//Coord refCoord = this->GetModuleGraphics()->GetFigure().GetBoxes().Begin().Coord(); //figurePixelBegin(0).Coord();

	//for( auto it =  this->GetModuleGraphics()->GetFigure().GetBoxes().Begin() ; it !=  this->GetModuleGraphics()->GetFigure().GetBoxes().End() ; ++it )
	//{
	//	double percentageHP = it.item().get()->GetPercentageOuterHitPointLeft();
	//	if( percentageHP < 1 && percentageHP > 0 )
	//		this->pGridGraphics.get()->SetBoxBlackLevel( refCoord*-1 + it.Coord(), percentageHP );
	//	else if( percentageHP <= 0 )
	//		this->pGridGraphics.get()->SetBoxBlackLevel( refCoord*-1 + it.Coord(), 0 );
	//}

}

void ModuleEntity::SetFlipped(enObjectFlipped flipped)
{
	if(this->getGridObject().get())
		this->getGridObject()->SetFlipped(flipped);
	if( flipped == enObjectFlipped::goIsFlipped )
		this->FlipGraphics();
}
void ModuleEntity::SetRotation(enObjectDiscreteRotation rotation)
{
	if(this->getGridObject().get())
		this->getGridObject()->SetRotation(rotation);
	this->Rotate(rotation);
}

void ModuleEntity::setUnitHeading(double newHeading)
{
	this->SetRelativeHeading(newHeading);
}

void ModuleEntity::updateVelocity(Uint32 milliseconds)
{
	DynamicEntity::updateVelocity(milliseconds);
}

void ModuleEntity::move(Uint32 milliseconds)
{
	DynamicEntity::move(milliseconds);
}

void ModuleEntity::ExecuteLocomotion(int milliseconds, const primitives::vector& changeinPos)
{
	if (this->pStepLocomotion.ExecuteLocomotion(milliseconds, this->GetLastChangeInPosition()))
	{
		this->IncreaseLocomotionFrame();
		this->pStepLocomotion.ResetCounter();
	}
}

void ModuleEntity::IncreaseLocomotionFrame()
{
	this->pStepLocomotion.IncreaseFrame();
	*this->pRelativePosition = this->getRelativePosition() + this->pStepLocomotion.GetLastChange();
}

bool ModuleEntity::IsLauncherType() const
{
	return UnitModuleContainer::IsLauncherType(this->GetModuleType());
}
bool ModuleEntity::IsExternalType() const
{
	return UnitModuleContainer::IsExternalType(this->GetModuleType());
}

primitives::vector ModuleEntity::ChangeInPosition(const primitives::vector& velocity, int milliseconds)
{
	primitives::vector oldPosition = primitives::vector(realX, realY);
	//velocity in m/s, position in pixels
	primitives::vector NEDFrameVelocity = geometry::rotateVector(velocity, this->getGlobalHeading());

	realX += velocity.x * (double)milliseconds / 1000; //NEDFrameVelocity
	realY += velocity.y * (double)milliseconds / 1000; //NEDFrameVelocity

	if ((int)realX*PIXELPRMETER != pRelativePosition->x)
	{
		pRelativePosition->x = (int)realX*PIXELPRMETER;
	}
	if (pRelativePosition->y != (int)realY*PIXELPRMETER)
	{
		pRelativePosition->y = (int)realY*PIXELPRMETER;
	}
	pLastChangeInPosition = primitives::vector(realX - oldPosition.x, realY - oldPosition.y);
	return pLastChangeInPosition;
}

void ModuleEntity::generateGridObjectToolTip( std::string tooltip )
{
	if(pGridObject != NULL )
	{
		if (pIncludeHPInTooltip)
			tooltip += "\nHP: " + this->GetHitPointForTooltip();

		InventoryItem::generateGridObjectToolTip(tooltip);
		
	}
}

void ModuleEntity::setRelativePosition( std::shared_ptr<Coord> pos )
{
	this->pRelativePosition = pos;
	if (pGridObject)
		pGridObject.get()->setProperPosition(*(pos.get()));	//setRelativePosition( *(pos.get()) );
}

void ModuleEntity::SetRelativeHeading(double value)//use pointer for heading, instead of this long line ??
{
	this->getGraphics()->getGraphicalObject()->getMainObject()->SetRelativeHeading(value);
	//if(updateUnitHeading)
	//	this->setUnitHeading(geometry::radLimit2PI(this->getUnitHeading() + this->GetRelativeHeading()));
};

double ModuleEntity::GetRelativeHeading()
{
	return this->getGraphics()->getGraphicalObject()->getMainObject()->GetRelativeHeading();
};

void ModuleEntity::refreshState()
{
	UpdateRelativePositionFromGrid();
}

void ModuleEntity::refreshHotKeys()
{
	UpdateHotKeysFromGrid();
}

void ModuleEntity::UpdateHotKeysFromGrid()
{
}

void ModuleEntity::UpdateRelativePositionFromGrid()
{
	if(this->hasGridObject())
		*this->pRelativePosition = pGridObject.get()->getRelativePosition();
}

std::shared_ptr<FiguredGraphics> ModuleEntity::GetModuleGraphics() const
{
	if( std::shared_ptr<FiguredGraphics> derived = std::dynamic_pointer_cast<FiguredGraphics>(this->getGraphics()))
		return derived;
	return nullptr;
}

const std::shared_ptr<PixelFigure> ModuleEntity::GetFigure() const
{
	if (ModuleGlobalList::HasInterface(this->GetIDName()))
		return ModuleGlobalList::GetInterface(this->GetIDName()).GetFigure();
	else
		return nullptr;
}

void ModuleEntity::SetDetached(bool set, int timed, const Coord& atPosition)
{
	this->SetDetached(set);
	this->pRelativePosition->x = atPosition.x;
	this->pRelativePosition->y = atPosition.y;
	this->realX = this->pRelativePosition->x;
	this->realY = this->pRelativePosition->y;
	this->getGraphics()->setTimer(timed);
}
void ModuleEntity::SetDetached(bool set)
{
	this->pDetached = set;
	this->reqCollisionDetect = !set;
	this->pGraphics->getGraphicalObject()->getMainObject()->SetCollisionIsEnabled(this->reqCollisionDetect);
}


int ModuleEntity::GetGridBoxesInFigure() 
{
	//if( this->getGridObject().get())
	//	return this->getGridObject()->GridBoxAmount();
	//else
	return this->GetModuleGraphics()->GetApproximateArea();
}

std::string ModuleEntity::ToWriteString() const
{
	//return "module",this->GetIDName() + ":" + this->getGridObject()->getRelativePosition().ToString()
	return this->GetIDName() + ":"
			+ this->pGridObject.get()->getRelativePosition().ToString() + ":"
			+ IntToString( (int)this->pGridObject.get()->GetDiscreteRotation() ) + "," + IntToString( (int)this->pGridObject.get()->GetFlipped() );
}

void ModuleEntity::ApplyNormalModuleImage()
{
	this->GetModuleGraphics()->SetSpriteFrame(1, false);
}
void ModuleEntity::ApplyDamagedModuleImage()
{
	this->GetModuleGraphics()->SetSpriteFrame(2, false);
}
void ModuleEntity::ApplyDestroyedModuleImage()
{
	this->GetModuleGraphics()->SetSpriteFrame(3, false);
}

void ModuleEntity::AddLocomotionFrame(const Coord Coord)
{
	this->pStepLocomotion.GetFrames().push_back(Coord);
}

Coord ModuleEntity::TransformPositionToModuleFrame(const Coord& pos, const Coord& unitCenter ) const
{
	Coord moduleObjectSize = this->getDimension().GetDimension();
	return pos - unitCenter.makepositive() - this->getRelativePosition() + moduleObjectSize / 2;
}

Coord ModuleEntity::ReverseTransformPositionToModuleFrame(const Coord& pos, const Coord& unitCenter) const
{
	Coord moduleObjectSize = this->getDimension().GetDimension();
	return pos + unitCenter.makepositive() + this->getRelativePosition() - moduleObjectSize / 2;
}

ModuleEntity::~ModuleEntity()
{
	this->FreeMemory();
}

void ModuleEntity::FreeMemory()
{
	this->pGridObject.reset();
	this->pRelativePosition.reset();

	GenericEntity::FreeMemory();
}
