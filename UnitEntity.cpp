#include"UnitEntity.h"
#include"ModuleInterfaceGlobalList.h"
#include"Statistics.h"
#include"GameManager.h"

UnitEntity::UnitEntity()
	: DynamicEntityControlled(Coord(0))
{
	Initialize();
}
UnitEntity::UnitEntity(const Coord pos)
	: DynamicEntityControlled(pos)
{
	Initialize();
}

void UnitEntity::Initialize()
{
	if (this->getGraphics().get())
		this->pModuleContainer.SetUnitCenter(this->GetFigureCenter());
	this->enUnitState = enumUnitState::NORMAL;
	pTargetList = std::vector<std::shared_ptr<UnitEntity>>();
	this->pTargetStrategy = enReTargetStrategy::NOCHANGE;
	this->pInternalTimerMs = 0;
	this->pChangeTargetTimer = 6000;
	this->pIsSpawned = true;
	this->pIsInvulnerable = false;
}

void UnitEntity::SpawnVessel( bool refreshHudList ) 
{
	pIsSpawned = true;
	if(refreshHudList)
		GameManager::RefreshHUDVesselList();
}

Coord UnitEntity::TransformPositionToLinkFrame(const Coord& pos, const ModuleEntity& module)
{
	//Variant 1

	//Coord moduleObjectSize = module.getDimension().GetDimension();
	//return pos - this->getGraphics()->getCenter() - module.getRelativePosition() + moduleObjectSize / 2;

	//Variant 2 same as in graphical tree

	//Coord moduleObjectSize = module.getDimension().GetDimension();
	////Coord linkObjectSize = link.pGObject.get()->getSize().GetDimension();
	////Coord thisObjectSize = this->getSize().GetDimension();

	////Coord t1 = *link.relationalZeroHeadingPosition.get();
	////Coord t2 = this->getCenter();
	////Coord t3 = linkObjectSize / 2;

	//Coord result = Coord(pos) - this->getGraphics()->getCenter() - module.getRelativePosition() + moduleObjectSize / 2;
	return module.TransformPositionToModuleFrame(pos, this->getGraphics()->getCenter());
	//return result;
}

Coord UnitEntity::TransformPositionToLinkFrame(const Coord& pos, InGameGraphicalObjectLink& link)
{
	//Variant 1

	//Coord linkObjectSize = link.pGObject.get()->getSize().GetDimension();
	//return pos - this->getGraphics()->getCenter() - link.relationalPosition + linkObjectSize / 2;

	//Variant 2 same as in graphical tree

	Coord linkObjectSize = link.GObject().get()->getSize().GetDimension();
	Coord result = Coord(pos) - this->getGraphics()->getCenter() - link.relationalPosition  + linkObjectSize / 2;	//same form as in TransformPositionToModuleFrame(...)
	return result;

}

std::shared_ptr<ExternalModule> UnitEntity::GetExternalModuleFromCoord(const Coord& Coord)
{
	return this->pModuleContainer.GetExternalModuleFromCoord(Coord ,false);
}

ModuleEntity* UnitEntity::GetModuleFromCoord(const Coord& Coord)
{
	return this->pModuleContainer.GetModuleFromCoord(Coord, false);
}

std::vector<std::shared_ptr<ModuleEntity>> UnitEntity::GetAllModules() const
{
	auto externalModules = this->GetExternalModulesList();
	auto internalModules = this->GetInternalModulesList();
	std::vector<std::shared_ptr<ModuleEntity>> result = std::vector<std::shared_ptr<ModuleEntity>>();
	result.insert(result.end(), externalModules.begin(), externalModules.end());
	result.insert(result.end(), internalModules.begin(), internalModules.end());
	return result;
}

std::vector<std::shared_ptr<LauncherEntity> > UnitEntity::GetLauncherList() const
{
	std::vector<std::shared_ptr<LauncherEntity> > result = std::vector<std::shared_ptr<LauncherEntity> >();
	
	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
		result.push_back(*it);
	return result;
}

std::shared_ptr<ModuleEntity> UnitEntity::GetModulesWithGridObject(const std::shared_ptr<GridObject> object)
{
	return this->pModuleContainer.GetModulesWithGridObject(object);
}

std::shared_ptr<ModuleEntity> UnitEntity::GetModulesFromID(enModuleType type, int ID)
{
	return this->pModuleContainer.GetModulesFromID(type, ID);
}

bool UnitEntity::ContainsInternalModule(std::shared_ptr<ModuleEntity> entity)
{
	return this->pModuleContainer.ContainsInternalModule(entity);
}

bool UnitEntity::ContainsGridObject(std::shared_ptr<GridObject> object, bool checkInternalContainers)
{
	if (this->pModuleContainer.ContainsGridObject(object, false))
		return true;
	
	if (checkInternalContainers)
	{
		if (ContainsAmmo(object))
			return true;
	}
	return false;
}

bool UnitEntity::ContainsAmmo(std::shared_ptr<GridObject> object)
{
	for (auto it = this->pModuleContainer.AC_begin(); it != this->pModuleContainer.AC_end(); ++it)
		if ((*it)->HasAmmo(object))
			return true;
	return false;
}

std::vector<std::shared_ptr<ModuleEntity>> UnitEntity::GetInternalModulesList() const
{
	return this->pModuleContainer.GetInternalModulesList();
}

std::vector<std::shared_ptr<ExternalModule>> UnitEntity::GetExternalModulesList() const
{
	return this->pModuleContainer.GetExternalModulesList(false);
}

void UnitEntity::ApplyDamage(const Coord & Coord, const game::Damage& damage)
{
	if(!this->pIsInvulnerable)
		HandleSingleHit(Coord, damage);
}

bool UnitEntity::AllModulesDetached()
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		if ((*it)->IsDetached() == false)
			return false;
	}
	return true;
}

void UnitEntity::HandleSingleHit(const Coord & Coord, const game::Damage& damageType)
{
	if (damageType.value == 0)
		return;

	std::shared_ptr<ExternalModule> hitModule = GetExternalModuleFromCoord(Coord);
	if (hitModule != nullptr)
	{
		setHitDamage(hitModule.get(), TransformPositionToLinkFrame(Coord, *hitModule), damageType);
		if (hitModule->getHitPointLeft() <= 0)
		{
			DetachModule(hitModule, damageType.GetExplosiveForce());
			GameManager::GetParticleEngine()->AddNewEffect(GameManager::GetParticleEngine()->GetEffectDustSpread(this->getGraphics()->getGraphicalObject()->getMainObject()->reverseTransformPositionToObjectFrame(Coord) + this->getPosition().yflip(), 80, 100, 0, 40000));    //(Coord + this->getPosition().yflip()), 1, 100, 40000);	//OK to call gamemanager from here??
			if (AllModulesDetached())
				this->DestroyEntity();
		}
		else
		//{
			//if (hitModule->getPercentHullLeft() < 0.1)
			//{
			//	hitModule->ApplyDestroyedModuleImage();
			//}
			//else if (hitModule->getPercentHullLeft() < 0.6)
			//{
			//	hitModule->ApplyDamagedModuleImage();
			//}
		//}

		if (damageType.AOE_range >= this->getSize())
			this->AddDamageRecieved(damageType);
		return;
	}
	else
		this->AddDamageRecieved(damageType);
}

void UnitEntity::setHitDamage(ModuleEntity* hitModule, const Coord & Coord, const game::Damage& damage)
{
	hitModule->AddDamageRecieved(damage);
}

void UnitEntity::AddDamageRecieved(const game::Damage& set)
{
	GenericEntity::AddDamageRecieved(set);
	if (this->getPercentHullLeft() <= 0.3)
	{
		DetachAllModules(set.GetExplosiveForce(), 180);
		this->DestroyEntity();
	}
}

void UnitEntity::DestroyEntity()
{
	GenericEntity::DestroyEntity();
	this->getGraphics()->setTimer(50000);
	if (this->getSize() > 128)
		this->getGraphics()->TriggerDestroyAnimation(enHitAnimation::MISSILEEXPLOSION, 1000, true);
}

void UnitEntity::SetHitPointZero()
{
	this->pHull.setHitpoints(game::enHitPointType::HP_STRUCTURAL, 1, false);
}

bool UnitEntity::HasDetachModulesToProcess()
{
	return this->pModuleContainer.DetachedModulesCount() > 0;
}

void UnitEntity::DetachAllModules(const primitives::vector& direction, double directionAngleSpread)
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		if ((*it)->IsDetached() == false)
		{
			auto rotated = geometry::rotateVector(direction, geometry::DegreesToRad(statistics::GetIntInRange(-directionAngleSpread, directionAngleSpread)));
			DetachModule((*it), rotated);
		}
	}
}

void UnitEntity::DetachModule(std::shared_ptr<ExternalModule> toRemove, const primitives::vector& direction)
{
	if (toRemove->IsDetached())
		return;

	//some refactoring here for big duplicate code chunks

	//double angularimpulseforce = (direction.x + direction.y) / 32;
	if (toRemove->CanDetach())
		HandleExternalModuleDetach(toRemove, direction);
	else
		HandleModuleStaticDetach(toRemove);

	for (auto it = toRemove->contained_begin(); it != toRemove->contained_end(); ++it)
	{
		if ((*it)->IsDetached())	
			continue;
		HandleInternalModuleDetach((*it), direction);
	}

	if (toRemove->GetModuleType() == enModuleType::mBridge)
		this->SetHitPointZero();
}

void UnitEntity::HandleExternalModuleDetach(std::shared_ptr<ExternalModule> toRemove, const primitives::vector& direction)
{
	auto reducedDirection = direction.ScaleByFactor(direction, 1.0/this->GetSimpleMass());

	this->getGraphics()->getGraphicalObject()->DetachLinkWithObject(toRemove->getGraphics()->getGraphicalObject()->getMainObject());
	toRemove->SetDetached(true, 20000, toRemove->getRelativePosition() - this->getPosition().xflip());
	toRemove->setVelocity(this->getVelocity() + geometry::rotateVector(reducedDirection, 0.4*statistics::GetIntInRange(-3, 3)));
	toRemove->setAngularVelocity(this->getAngularVelocity() + statistics::GetIntInRange(1, 4, true)*reducedDirection.ABS());
	//toRemove->setImpulseForce();//(direction);
	//toRemove->setImpulseAngularForce);
	toRemove->SetParticleEffect(EnumParticleEffect::particledustspread);	//causing fps down??

	this->DecrementSimpleMass(toRemove->GetSimpleMass());
	this->DecrementMomentOfInertia(toRemove->GetSimpleMass(), geometry::realDistance(toRemove->getRelativePosition()));
	this->pModuleContainer.AddDetachedModuleToList(toRemove);
}

void UnitEntity::HandleInternalModuleDetach(std::shared_ptr<ModuleEntity> toRemove, const primitives::vector& direction)
{
	auto reducedDirection = direction.ScaleByFactor(direction, 0.5 / this->GetSimpleMass());

	this->getGraphics()->getGraphicalObject()->DetachLinkWithObject(toRemove->getGraphics()->getGraphicalObject()->getMainObject());   //(*it)->getRelativePosition());
	toRemove->SetDetached(true, 20000, toRemove->getRelativePosition() - this->getPosition().xflip());
	toRemove->setVelocity(this->getVelocity() + geometry::rotateVector(reducedDirection, 0.4*statistics::GetIntInRange(-3, 3)));
	toRemove->setAngularVelocity(this->getAngularVelocity() + statistics::GetIntInRange(1, 4, true)*reducedDirection.ABS());
	//toRemove->setImpulseForce(geometry::rotateVector(primitives::vector::ScaleByFactor(direction, 0.3), 0.4*statistics::GetIntInRange(-3, 3)));
	//toRemove->setImpulseAngularForce(statistics::GetIntInRange(1, 6, true)*angularimpulseforce);
	toRemove->SetParticleEffect(EnumParticleEffect::particledustspread);	//causing fps down??

	this->DecrementSimpleMass(toRemove->GetSimpleMass());
	this->DecrementMomentOfInertia(toRemove->GetSimpleMass(), geometry::realDistance(toRemove->getRelativePosition()));
	this->pModuleContainer.AddDetachedModuleToList(toRemove);
}

void UnitEntity::HandleModuleStaticDetach(std::shared_ptr<ExternalModule> toRemove)
{
	toRemove->SetDetached(true);
	this->DecrementSimpleMass(toRemove->GetSimpleMass()/2);
	this->DecrementMomentOfInertia(toRemove->GetSimpleMass()/2, geometry::realDistance(toRemove->getRelativePosition()));
}

void UnitEntity::ProcessDetachedModules(int millisecond)
{
	std::vector<int> remainingIndices = std::vector<int>();
	int counter = 0;
	for (auto it = this->pModuleContainer.Detached_begin(); it != this->pModuleContainer.Detached_end(); ++it)
	{
		if (it->get() == false)
			continue;

		bool active = it->get()->getGraphics()->reduceTimer(millisecond);
		if (active == true)
			remainingIndices.push_back(counter);
		else
		{
			this->getGraphics()->getGraphicalObject()->GetObjectFromID((*it)->GetID())->SetDrawLink(false);
		}
		counter++;
	}
	if (remainingIndices.size() != counter)
		this->pModuleContainer.RemoveDetachedEntity(remainingIndices);//, true);
}

void UnitEntity::UpdateContainedListOfExternalModules()
{
	this->pModuleContainer.UpdateContainedListOfExternalModules();
	this->SetHullHitPoints(game::enHitPointType::HP_MATERIAL, this->pModuleContainer.GetMaterialHitPoints(), true);
	this->SetHullHitPoints(game::enHitPointType::HP_STRUCTURAL, this->pModuleContainer.GetStructuralHitPoints(), true);
}

void UnitEntity::UpdateLaunchPoints()
{
	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); it++)
		UpdateLaunchPoint((*it));
}

void UnitEntity::UpdateLaunchPoint(std::shared_ptr<LauncherEntity> entity)
{
	Coord newRelativePos = geometry::rotateCoord(entity->getRelativePosition().yflip(), getUnitHeading());
	Coord newProjLaunchPoint = geometry::rotateCoord(entity->getRelativeProjectileLaunchPoint().yflip(), getUnitHeading());
	Coord newLaunchPoint = this->getPosition() + Coord(this->getSize() / 2).yflip() + newRelativePos + newProjLaunchPoint;
	entity->setLaunchPoint(newLaunchPoint);
}

void UnitEntity::ChangeLaunchersDirection(Coord ToPoint)
{
	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
		(*it)->SetRelativeHeading(geometry::angleBetweenTwoPoints((*it)->getPosition(), ToPoint));
}

void UnitEntity::ChangeLaunchersDirection(double angle)
{
	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
	{
		(*it)->SetRelativeHeading(angle);
	}
}

void UnitEntity::ActivateModule(enModuleType type, int ID)
{
	std::shared_ptr<ModuleEntity> entity = this->GetModulesFromID(type, ID);
	if (entity != nullptr)
		entity->Enable();
}

void UnitEntity::DisableModule(enModuleType type, int ID)
{
	std::shared_ptr<ModuleEntity> entity = this->GetModulesFromID(type, ID);
	if (entity != nullptr)
		entity->Disable();
}

bool UnitEntity::IsModuleActive(enModuleType type, int ID)
{
	std::shared_ptr<ModuleEntity> entity = this->GetModulesFromID(type, ID);
	if (entity != nullptr)
		return this->GetModulesFromID(type, ID)->IsEnabled();
	return false;
}

void UnitEntity::ChangeExternalModuleDirection(bool bridge, bool frame, bool wings, double angle)
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		if(bridge && (*it)->GetModuleType() == enModuleType::mBridge )
			(*it)->SetRelativeHeading(angle);
		if (frame && (*it)->GetModuleType() == enModuleType::mFrame)
			(*it)->SetRelativeHeading(angle);
		if (wings && (*it)->GetModuleType() == enModuleType::mWing)
			(*it)->SetRelativeHeading(angle);
	}
}

void UnitEntity::RotateModuleGraphics(bool bridge, bool frame, bool wings, bool launcher, double angle)
{
	std::set<int> rotateindices = GetIndicesOfModules(bridge, frame, wings, launcher);	//can be stored as a cache..
	std::vector<std::shared_ptr<InGameGraphicalObjectLink>> toRotate = std::vector<std::shared_ptr<InGameGraphicalObjectLink>>();
	this->getGraphics()->getGraphicalObject()->GetObjectFromIDs(rotateindices, toRotate);
	this->getGraphics()->getGraphicalObject()->UpdateLinkHeading(angle, toRotate);
}

std::set<int> UnitEntity::GetIndicesOfModules(bool bridge, bool frame, bool wings, bool launcher)
{
	std::set<int> rotateindices = std::set<int>();
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		if (bridge && (*it)->GetModuleType() == enModuleType::mBridge)
			rotateindices.insert((*it)->GetID());
		if (frame && (*it)->GetModuleType() == enModuleType::mFrame)
			rotateindices.insert((*it)->GetID());
		if (wings && (*it)->GetModuleType() == enModuleType::mWing)
			rotateindices.insert((*it)->GetID());
	}

	if(launcher)
		for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
			rotateindices.insert((*it)->GetID());

	return rotateindices;
}

void UnitEntity::IncrementModuleLocmotionFrames()
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		if ((*it)->LocomotionCanExecute())
			(*it)->IncreaseLocomotionFrame();
	}
}

void UnitEntity::ExecuteLocomotion(int milliseconds)	//P.S exact same code as in ModuleEntity .. :(
{
	if (this->pStepLocomotion.ExecuteLocomotion(milliseconds, this->GetLastChangeInPosition()))
		UpdateLocomotionFrames();
}

void UnitEntity::Fire()
{
	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
	{
		if (it->get()->IsDetached() == false && !(*it)->IsAutoFire())
			(*it)->setLaunch(true);
		//(*it)->FireBlaster(true);
	}
}

void UnitEntity::DeFire()
{
	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
	{
		if (it->get()->IsDetached() == false)
			(*it)->setLaunch(false);
		//(*it)->FireBlaster(false);
	}

}

void UnitEntity::AddGraphicalObjectToBody(std::shared_ptr<InGameGraphicalObject> object, double relationalHeading, int ID)
{
	this->getGraphics()->addGraphicalObject(object, Coord(0), Coord(0), relationalHeading, ID);
}

std::shared_ptr<FiguredGraphics> UnitEntity::GetUnitGraphics() const
{
	std::shared_ptr<FiguredGraphics> derived = std::dynamic_pointer_cast<FiguredGraphics>(this->getGraphics());
	return derived;
}

const std::shared_ptr<PixelFigure> UnitEntity::GetFigure() const
{
	return ModuleGlobalList::GetInterface(this->GetChassisName()).GetFigure();
}


//identical one in deco
bool UnitEntity::SetupNewModuleGraphics(std::shared_ptr<ModuleEntity> module, const Coord& pos, const Coord& anchorPoint)
{
	//if (module->IDIsSet() )
	//	throw std::exception("error module is already defined in a unit");

	if (module->getGraphics()->getConnectedObjectsAmount() > 1)
	{
		auto baseGraphics = module->getGraphics();
		for (auto it = baseGraphics->getGraphicalObject()->begin(); it != baseGraphics->getGraphicalObject()->end(); ++it)
		{
			Coord LinkAnchorPoint = this->getDimension().GetDimension() / 2 - (*it)->GObject()->getSize().GetDimension() / 2;
			auto link = this->getGraphics()->addGraphicalObject(it->get()->GObject(), pos + *it->get()->relationalZeroHeadingPosition, LinkAnchorPoint, 0 );
			it->get()->SetID(link.GetID());
		}
	}
	InGameGraphicalObjectLink& generatedLink = this->getGraphics()->addGraphicalObject(module->getGraphics()->getGraphicalObject()->getMainObject(), pos, anchorPoint, 0);
	generatedLink.GObject()->SetDrawAfterParent(true); //only for testing!!, you can remove this
	module->SetID(generatedLink.GetID());
	module->setRelativePosition(generatedLink.relationalZeroHeadingPosition);
	this->pCurrentModuleID = module->GetID() + 1;
	return true;
}

bool UnitEntity::RemoveModuleEntity(std::shared_ptr<ModuleEntity> in)
{
	bool removed = false;

	if (in->IsExternalType())
		removed = this->pModuleContainer.RemoveExternalEntity(std::static_pointer_cast<ExternalModule>(in));
	else if (in->IsLauncherType())
		removed = this->pModuleContainer.RemoveLauncherEntity(std::static_pointer_cast<LauncherEntity>(in));
	else
		removed = this->pModuleContainer.RemoveModuleEntity(in);

	if (removed)
		this->getGraphics()->getGraphicalObject()->RemoveLink(in->getGraphics()->getGraphicalObject()->getMainObject());
	return removed;
}

void UnitEntity::AddHitBox(int width, int height, Coord pos, enHitBoxCollision type)
{
	std::shared_ptr<HitBoxEntity> newHB = std::shared_ptr<HitBoxEntity>(ModuleGlobalList::GetHitBox(width, height, ColorRGB(100, 255, 100, 255), type));
	this->pModuleContainer.AddHitBoxToList(newHB);
	

	Coord anchorpoint = this->getDimension().GetDimension() / 2 - Coord(width, height) / 2;
	InGameGraphicalObjectLink& generatedLink = this->getGraphics()->addGraphicalObject(newHB->getGraphics(), pos, anchorpoint, 0); //this->getSize() / 2
	generatedLink.SetIsHitBox(true);
	newHB->setRelativePosition(generatedLink.relationalZeroHeadingPosition);
	this->pCurrentModuleID = generatedLink.GetID() + 1;
	newHB->SetID(generatedLink.GetID());
	generatedLink.SetLinkTakeNoDamage(true);

}

void UnitEntity::ResetModuleLocomotion()
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		if ((*it)->LocomotionCanExecute())
			(*it)->ResetLocomotionCounter();
	}
}

void UnitEntity::UpdateLocomotionFrames()
{
	this->IncreaseLocomotionFrame();
	this->ResetLocomotionCounter();
	this->IncrementModuleLocmotionFrames();
	this->ResetModuleLocomotion();
}

void UnitEntity::ResetLocomotionCounter()
{
	this->pStepLocomotion.ResetCounter();
}

void UnitEntity::IncreaseLocomotionFrame() //P.S Almost exact same code as in ModuleEntity :(
{
	int testResult = this->pStepLocomotion.GetCurrentStep().y;
	this->SetAbsVelocity(testResult);
	this->pStepLocomotion.IncreaseFrame();
}

void UnitEntity::LocomoteExternalModules(int milliseconds)
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		if((*it)->LocomotionCanExecute())
			(*it)->ExecuteLocomotion(milliseconds, this->GetLastChangeInPosition());
	}
}

void UnitEntity::ChangeHeadingForExternalModules(int milliseconds)
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		(*it)->updateHeading(milliseconds);
	}
}

void UnitEntity::setTexture(std::shared_ptr<GraphicalEntity> set)
{
	pGraphics = set;
	this->UpdateCacheSize();
	if(set.get())
		this->pModuleContainer.SetUnitCenter(this->GetFigureCenter());
}

void UnitEntity::DefineChassis(std::shared_ptr<FiguredGraphics> set, string chassisName)
{
	this->setTexture(set);
	this->pChassisName = chassisName;
	this->SetSimpleMass(0);
	this->SetSimpleMomentOfInertia(20);
	this->reqCollisionDetect = true;
}

void UnitEntity::DefineChassis(ModuleEntity* set, string chassisName)
{
	this->setTexture(set->GetModuleGraphics());
	this->pChassisName = chassisName;

	this->SetSimpleMass(set->GetSimpleMass());
	this->SetSimpleMomentOfInertia(20);

	if (this->GetInternalModulesList().size() > 0)
	{
		for (auto it = this->GetInternalModulesList().begin(); it != this->GetInternalModulesList().end(); ++it)
		{
			this->IncrementSimpleMass((*it)->GetSimpleMass());
			this->IncrementMomentOfInertia((*it)->GetSimpleMass(), geometry::realDistance((*it)->getRelativePosition()));
		}
	}

	if (this->GetExternalModulesList().size() > 0)
	{
		for (auto itt = this->GetExternalModulesList().begin(); itt != this->GetExternalModulesList().end(); ++itt)
		{
			this->IncrementSimpleMass((*itt)->GetSimpleMass());
			this->IncrementMomentOfInertia((*itt)->GetSimpleMass(), geometry::realDistance((*itt)->getRelativePosition()));
		}
	}

	this->reqCollisionDetect = true;
}

bool UnitEntity::HasAmmo(string type)
{
	return true;
}
bool UnitEntity::DecrementAmmo(string type)
{
	return true;
}
bool UnitEntity::AddAmmo(std::shared_ptr<ItemAmmo> ammo, Coord pos)	//only adds to first container in list!!
{
	for (auto it = this->pModuleContainer.AC_begin(); it != this->pModuleContainer.AC_end(); ++it)
	{
		(*it)->AddAmmo(ammo, pos);
		return true;
	}
	return false;
}

bool UnitEntity::RemoveAmmo(std::shared_ptr<ItemAmmo> ammo) 	//only adds to first container in list!!
{
	for (auto it = this->pModuleContainer.AC_begin(); it != this->pModuleContainer.AC_end(); ++it)
	{
		return (*it)->RemoveAmmo(ammo);
	}
	return false;
}

void UnitEntity::TargetVessel(int vesselID)
{
	auto entity = GameManager::CurrentInstanceManager()->GetIntegrator()->GetUnitEntityFromID(vesselID);
	if (entity)
		TargetVessel(entity);
}

void UnitEntity::TargetVessel(std::shared_ptr<UnitEntity> target)
{
	if (target.get())
		AddTarget(target);
	else
		RemoveTarget();
}

void UnitEntity::AddTarget(std::shared_ptr<UnitEntity> target)
{
	for (auto lt = this->pModuleContainer.LE_begin(); lt != this->pModuleContainer.LE_end(); ++lt)
		(*lt)->SetAimTarget(target);
}

void UnitEntity::RemoveTarget()
{
	for (auto lt = this->pModuleContainer.LE_begin(); lt != this->pModuleContainer.LE_end(); ++lt)
		(*lt)->SetAimTarget(nullptr);
}

void UnitEntity::SetTargetSelectLinkState(bool ActiveState)
{
	ActiveState ? this->LinkTarget.link->SetWidgetClicked() : this->LinkTarget.link->SetWidgetOff();
}

std::vector<std::shared_ptr<Projectile>> UnitEntity::RunLaunchers(int milliseconds)
{
	std::vector<std::shared_ptr<Projectile>> result = std::vector<std::shared_ptr<Projectile>>();
	result.reserve(5);
	for (auto lt = this->pModuleContainer.LE_begin(); lt != this->pModuleContainer.LE_end(); ++lt)
	{
		if ((*lt)->IsAutoFire())
			(*lt)->UpdateAim(milliseconds);

		UpdateLauncherAmmo(*lt);
		auto launched = RunLaunchProjectile(*lt, milliseconds);
		if (launched.get())
			result.push_back(std::move(launched));
	}
	return result;
}

std::shared_ptr<Projectile> UnitEntity::RunLaunchProjectile(std::shared_ptr<LauncherEntity> launcher, int ms)
{
	std::shared_ptr<Projectile> result = nullptr;
	if (launcher->ReadyToLaunch() && launcher->GetRateOfFire() > 0)
	{
		if (launcher->getLastTimeFired() >= launcher->GetRateOfFire())
		{
			auto launched = launcher->launch();
			if (launched.get())
				result = std::move(launched);
			launcher->setLastFire(0);
		}
		else
			launcher->setLastFire(launcher->getLastTimeFired() + ms);
	}
	
	return result;
}

void UnitEntity::UpdateLauncherAmmo(std::shared_ptr<LauncherEntity> launcher)
{
	int launchedProjectiles = launcher->GetLaunchedProjectiles();

	if (launchedProjectiles > 1)
		throw std::exception("should not get here!");
	else if (launchedProjectiles == 1)
	{
		launcher->ResetLaunchedProjectiles();
		if (this->DecrementAmmo(launcher->GetAmmoType()) == false)
			launcher->Disable();
	}
}

void UnitEntity::RunDetached(int milliseconds)
{
	for (auto it = this->pModuleContainer.Detached_begin(); it != this->pModuleContainer.Detached_end(); ++it)
	{
		if (it->get())
		{
			(*it)->updateVelocity(milliseconds);
			(*it)->move(milliseconds);
			(*it)->TriggerParticleEffect(milliseconds, this->getDimension().Center());
		}
	}
}

void UnitEntity::RunModuleAnimations( int ms )
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		(*it)->GetAnimationContainer().RunAnimations(ms);
	}
}

void UnitEntity::AddToTargetList(std::shared_ptr<UnitEntity> target)
{
	this->pTargetList.push_back(target);
}


void UnitEntity::ChangeLauncherTargets()
{
	if (this->getTimer() < this->pChangeTargetTimer)
		return;

	this->setTimer(0);
	switch (this->GetReTargetStrategy())
	{
	case(enReTargetStrategy::NOCHANGE):
		return;
	case(enReTargetStrategy::RANDOM):
		RandomRetargetLaunchers();
		return;
	case(enReTargetStrategy::CLOSEST):
		ClosestReTargetLaunchers();
		return;
	case(enReTargetStrategy::CUSTOMAI):
		return;
	}
}


void UnitEntity::RandomRetargetLaunchers()
{
	if (this->pTargetList.size() == 0)
		return;

	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
	{
		int index = statistics::GetIntInRange(0, this->pTargetList.size() - 1);
		(*it)->SetAimTarget(pTargetList[index]);
	}
}

void UnitEntity::ClosestReTargetLaunchers()
{
	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
	{
		for (auto tt = this->pTargetList.begin(); tt != pTargetList.end(); ++tt)
			if ((*it)->TargetWithinRange(*tt))
				(*it)->SetAimTarget(*tt);
	}
}




//void UnitEntity::DockToTarget(DynamicEntity* target)
//{
//	this->SetVesselState(enumUnitState::DOCKING);
//}

void UnitEntity::HandleUnitState(Uint32 milliseconds)
{

}