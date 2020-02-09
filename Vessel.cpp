#include"Vessel.h"
#include"SpriteObject.h"
#include"ModuleInterfaceGlobalList.h"
#include"InGameGraphicalObjectLink.h"
#include"MissileLauncher.h"
#include"TorpedoLauncher.h"
#include"RailgunLauncher.h"
#include"AntiProjectileMissileLauncher.h"
#include"Statistics.h"
#include"GameManager.h"

SDL_sem* vesselSem = SDL_CreateSemaphore(0);

Vessel::Vessel() :  UnitEntity()
{
	this->SetAbsVelocity(0);
	//this->velocityHeadingSetPointAbsValue = 0;
	this->Initialize();
}

Vessel::Vessel( Coord pos) : UnitEntity(pos)
{
	this->SetAbsVelocity(500);
	//this->velocityHeadingSetPointAbsValue = 0.3;
	this->Initialize();
};

void Vessel::Initialize()
{
	this->SetUnitState(enumUnitState::UNDEFINED);
	//this->SetThrust(0);
	//this->SetAngularThrust(0);
	this->pID = -100;
	this->pCurrentModuleID = 0;
	this->pLocomoteModules = false;
	this->pMoveModules = false;
	this->EnableWaypointTracking(true);
	//this->pDriver.SetVelocityMagnitude(100);
	//this->pDriver.SetAngularVelocityMagnitude(3); 	//velocityHeadingSetPointAbsValue = 0.3;
	//this->pDriver.SetAccelerationHeadingMagnitude(2);
	this->SetRotationDrive(EnumEntityDrive::None);

	
	//this->SetWalkerState(false);
}

void Vessel::updateVelocity(Uint32 ms)
{
	this->LoadImpulseForceFromEngines();
	DynamicEntityControlled::updateVelocity(ms);
	if (pLocomoteModules)
		LocomoteExternalModules(ms);

}

void Vessel::move(Uint32 millisec )
{
	DynamicEntityControlled::move(millisec);
	UpdateLaunchPoints();
	if (pMoveModules)
		ChangeHeadingForExternalModules(millisec);
}

void Vessel::handleEvent( SDL_Event & e, const Coord& cameraPos )
{
	DynamicEntityControlled::handleEvent(e,cameraPos);
}



//void Vessel::SetThrust(int value)
//{
//	this->pCurrentThrust = value;
//}
//
//void  Vessel::AddThrust(int value)
//{
//	SetThrust(this->pCurrentThrust + value);
//}
//
//void Vessel::SetAngularThrust(int value)
//{
//	this->currentAngularThrust = value;
//}
//void Vessel::AddAngularThrust(int value)
//{
//	SetAngularThrust(this->currentAngularThrust + value);
//}

 void Vessel::DestroyEntity()
 {
	UnitEntity::DestroyEntity();
	GameManager::RefreshHUDVesselList();
 }

 void Vessel::HandleExternalModuleDetach(std::shared_ptr<ExternalModule> toRemove, const primitives::vector& direction)
 {
	 UnitEntity::HandleExternalModuleDetach(toRemove, direction);
	 //this->AddThrust(-toRemove->GetProperties().Thrust);
	 //this->pDriver.SetMaxThrust(this->pDriver.GetMaxThrust() -toRemove->GetProperties().Thrust);
	 //this->pDriver.SetMaxAngularThrust(this->pDriver.GetMaxAngularThrust() - toRemove->GetProperties().AngularThrust);
 }
 void Vessel::HandleModuleStaticDetach(std::shared_ptr<ExternalModule> toRemove)
 {
	 UnitEntity::HandleModuleStaticDetach(toRemove);
	 //this->pDriver.SetMaxThrust(this->pDriver.GetMaxThrust() - toRemove->GetProperties().Thrust);
	 //this->pDriver.SetMaxAngularThrust(this->pDriver.GetMaxAngularThrust() - toRemove->GetProperties().AngularThrust);
	// this->AddThrust(-toRemove->GetProperties().Thrust);
 }

 void Vessel::TriggerBooster()
 {
	 setImpulseForce( geometry::rotateVector( primitives::vector(1000,0), this->getUnitHeading() )  );
 }

void Vessel::RefreshGridObjects()
{
	auto modules = GetAllModules();

	for(auto it = modules.begin(); it != modules.end(); ++it)
		(*it)->RefreshGridObjectColour();
}

int Vessel::GetSummedHitPointsOfModules()
{
	int result = this->getMaxHullPoints();
	for (int i = 0; i < this->GetExternalModulesList().size(); i++)
	{
		result += this->GetExternalModulesList()[i]->getMaxHullPoints();
	}
	return result;
}

 void Vessel::AddExternalModule( std::shared_ptr<ExternalModule> in, Coord pos, Coord anchorPos )
 {
	 SetupNewModuleGraphics(in, pos, anchorPos);
	 this->pModuleContainer.AddExtModuleToList(in);
 }

void Vessel::AddWPlatform( std::shared_ptr<LauncherEntity> in, Coord pos, Coord anchorPos )
{ 
	if (SetupNewModuleGraphics(in, pos, anchorPos))
	{
		this->pModuleContainer.AddLauncherToList(in, true);
		in->setLaunchPoint(this->getPosition() + this->getSize() / 2 - in->getProjectile()->getHitPoint() + in->getRelativePosition() + in->getRelativeProjectileLaunchPoint());
		in->ChangeMothership(this);
	}
}

void Vessel::AddModuleEntity(std::shared_ptr<ModuleEntity> in, Coord relative_pos, Coord anchorPoint)
{
	this->IncrementSimpleMass(in->GetSimpleMass());
	this->IncrementMomentOfInertia(in->GetSimpleMass(), geometry::realDistance(relative_pos));
	switch (in->GetModuleType())
	{
	case(enModuleType::mController):
		this->AddController(std::static_pointer_cast<ControllerEntity>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mEngine):
		this->AddEngine(std::static_pointer_cast<EngineEntity>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mLauncher):
		this->AddWPlatform(std::static_pointer_cast<LauncherEntity>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mMissileLauncher):
		this->AddWPlatform(std::static_pointer_cast<MissileLauncherEntity>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mAntiMLauncher):
		this->AddWPlatform(std::static_pointer_cast<AntiProjectileMissileLauncher>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mTorpedoLauncher):
		this->AddWPlatform(std::static_pointer_cast<TorpedoLauncherEntity>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mRailgunLauncher):
		this->AddWPlatform(std::static_pointer_cast<RailgunLauncher>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mPowerSupply):
		this->AddPowerSupply(std::static_pointer_cast<PowerSupplyEntity>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mRadar):
		this->AddRadar(std::static_pointer_cast<RadarEntity>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mAmmoContainer):
		this->AddAmmoContainer(std::static_pointer_cast<AmmoContainer>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mBridge):
		AddExternalModule(std::static_pointer_cast<ExternalModule>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mWing):
		AddExternalModule(std::static_pointer_cast<ExternalModule>(in), relative_pos, anchorPoint);
		break;
	case(enModuleType::mEngineBody):
		AddExternalModule(std::static_pointer_cast<ExternalModule>(in), relative_pos, anchorPoint);
		//std::shared_ptr<FrameStrip> burner = in->GetAnimation("burner");
		//if(burner.get())
		//	burner->set
		break;
	case(enModuleType::mFrame):
		AddExternalModule(std::static_pointer_cast<ExternalModule>(in), relative_pos, anchorPoint);
		break;
		//case(enModuleType::mRoom):
		//	this->AddStorageRoom(std::static_pointer_cast<StorageEntity>(in), relative_pos, anchorPoint);
		//	break;
	}
}

int Vessel::GetThrust()
{
	return this->pDriver.GetMaxThrust();
}

int Vessel::MaxSpeed()
{
	return this->pDriver.GetVelocityMagnitude();
}
double Vessel::MaxAngularAcceleration()
{
	return this->pDriver.GetMaxAngularAcceleration();
}
double Vessel::MaxAcceleration()
{
	return (double)this->GetThrust() / (1 + this->GetSimpleMass());
}
double Vessel::MaxAngularSpeed()
{
	return this->pDriver.GetMaxAngularVelocity();
}

double Vessel::AngleDifferenceBasedOnVesselHeading(const Coord ToPosition)
{
	double angleToTarget = geometry::angleBetweenTwoPoints(this->getPosition(), ToPosition);
	double angleDifference = geometry::shortestAngleDifference(angleToTarget, this->getGlobalHeading());
	return angleDifference;
}

statistics::RandomNumber Vessel::ShipModuleValue() const
{
	statistics::RandomNumber result = statistics::RandomNumber(0, 0);

	auto internalList = this->pModuleContainer.GetInternalModulesList();
	auto externalList = this->pModuleContainer.GetExternalModulesList(false);

	

	for (int i = 0; i < internalList.size(); i++)
	{
		result += internalList[i]->GetPrice().SellValue();
	}

	for (int i = 0; i < externalList.size(); i++)
	{
		result += externalList[i]->GetPrice().SellValue();
	}

	return result;
}

EntityPrice Vessel::GetTotalPrice(bool includeShipValue) const
{
	EntityPrice result = EntityPrice(ShipModuleValue());

	if (includeShipValue)
		result.IncreaseSellValue(this->GetVesselPrice().SellValue());

	return result;
}

void Vessel::AddEngine( std::shared_ptr<EngineEntity> in, Coord pos, Coord anchorPoint )
{
	if (SetupNewModuleGraphics(in, pos, anchorPoint))
	{
		this->pModuleContainer.AddModuleToList(in, true);
		if (this->pModuleContainer.GetModulesAmount(enModuleType::mEngine) == 1)
		{
			this->pDriver.SetMaxThrust(in->GetThrust());
			this->pDriver.SetMaxAngularThrust(in->GetAngularThrust());
			this->SetDrive(EnumEntityDrive::Stable);
			this->SetRotationDrive(EnumEntityDrive::Stable);
		}
		else
		{
			this->pDriver.SetMaxThrust(this->pDriver.GetMaxThrust() + in->GetThrust());
			this->pDriver.SetMaxAngularThrust(this->pDriver.GetMaxAngularThrust() + in->GetAngularThrust());
		}
	}
}

void Vessel::RemoveEngineProperties(std::shared_ptr<EngineEntity> toRemove)
{
	//assumes not detached
	if (this->pModuleContainer.GetModulesAmount(enModuleType::mEngine) == 1)
	{
		this->currentAngularThrust = 0;
		this->currentThrust = 0;
		this->SetDrive(EnumEntityDrive::None);
		this->SetRotationDrive(EnumEntityDrive::None);
		this->pDriver.SetMaxThrust(0);
		this->pDriver.SetMaxAngularThrust(0);
	}
	else
	{
		this->pDriver.SetMaxThrust(this->pDriver.GetMaxThrust() - toRemove->GetThrust());
		this->pDriver.SetMaxAngularThrust(this->pDriver.GetMaxAngularThrust() - toRemove->GetAngularThrust());
	}

}

void Vessel::RemoveControllerProperties(std::shared_ptr<ControllerEntity> in)
{
	//assumes not detached
	if (this->pModuleContainer.GetModulesAmount(enModuleType::mController) == 1)
	{
		//doesn't work, is player gain changed afterwards??
		this->setControllerGain(0);
		this->setControllerDerivative(0);
		this->setControllerIntergral(0);
		this->setControllerGainHeading(0);
		this->setControllerDerivativeHeading(0);
		this->setControllerIntegralHeading(0);
		this->SetDrive(EnumEntityDrive::None);
		this->SetRotationDrive(EnumEntityDrive::None);
	}
}

void Vessel::AddRadar( std::shared_ptr<RadarEntity> in, Coord pos, Coord anchorPoint )
{
	SetupNewModuleGraphics(in, pos, anchorPoint);
	this->pModuleContainer.AddModuleToList(in, true);
}

void Vessel::AddPowerSupply( std::shared_ptr<PowerSupplyEntity> in, Coord relative_pos, Coord anchorPoint )
{
	SetupNewModuleGraphics(in, relative_pos, anchorPoint);
	this->pModuleContainer.AddModuleToList(in, true);
}

void Vessel::AddController( std::shared_ptr<ControllerEntity> in, Coord relative_pos, Coord anchorPoint )
{
	SetupNewModuleGraphics(in, relative_pos, anchorPoint);
	this->pModuleContainer.AddModuleToList(in, true);
}

void Vessel::AddAmmoContainer(std::shared_ptr<AmmoContainer> in, Coord relative_pos, Coord anchorPoint)
{
	SetupNewModuleGraphics(in, relative_pos, anchorPoint);
	this->pModuleContainer.AddModuleToList(in, true);
}

void Vessel::AddGlobalListModule( std::string name, Coord pos, enObjectDiscreteRotation rotation, enObjectFlipped flipped, bool isPlayer )
{
	ModuleEntity* entity = ModuleGlobalList::GetModule(name);

	if (isPlayer)
		entity->GenerateModuleGridBox(64);

	enModuleType mType = ModuleGlobalList::GetType( name );
	entity->SetFlipped( flipped );
	entity->SetRotation( rotation );

	entity->getGraphics()->SetDrawAfterParent(true);

	Coord anchorPoint = this->getDimension().GetDimension() / 2 - entity->getDimension().GetDimension() / 2; //just for graphics
	this->IncrementSimpleMass(entity->GetSimpleMass());
	this->IncrementMomentOfInertia(entity->GetSimpleMass(), geometry::realDistance(pos));

	switch( mType )
	{
	case( enModuleType::mController ) :
		{
		std::shared_ptr<ControllerEntity> ptr_ctrl = std::shared_ptr<ControllerEntity>( dynamic_cast<ControllerEntity*>(entity) );
		AddController( ptr_ctrl, pos, anchorPoint );
		return;
		}
	case( enModuleType::mEngine ) :
		{
		std::shared_ptr<EngineEntity> ptr_engine = std::shared_ptr<EngineEntity>( dynamic_cast<EngineEntity*>(entity) );
		AddEngine( ptr_engine, pos, anchorPoint );
		return;
		}
	case( enModuleType::mLauncher ) :
		{
		std::shared_ptr<LauncherEntity> ptr_launch = std::shared_ptr<LauncherEntity>( dynamic_cast<LauncherEntity*>(entity) );
		AddWPlatform( ptr_launch, pos, anchorPoint );
		return;
		}
	case( enModuleType::mMissileLauncher ) :
		{
		std::shared_ptr<MissileLauncherEntity> ptr_mlaunch = std::shared_ptr<MissileLauncherEntity>( dynamic_cast<MissileLauncherEntity*>(entity) );
		AddWPlatform( ptr_mlaunch, pos, anchorPoint );
		return;
		}
	case( enModuleType::mAntiMLauncher ) :
		{
		std::shared_ptr<AntiProjectileMissileLauncher> ptr_mlaunch = std::shared_ptr<AntiProjectileMissileLauncher>( dynamic_cast<AntiProjectileMissileLauncher*>(entity) );
		AddWPlatform( ptr_mlaunch, pos, anchorPoint );
		return;
		}
	case( enModuleType::mTorpedoLauncher ) :
		{
		std::shared_ptr<TorpedoLauncherEntity> ptr_mlaunch = std::shared_ptr<TorpedoLauncherEntity>( dynamic_cast<TorpedoLauncherEntity*>(entity) );
		AddWPlatform( ptr_mlaunch, pos, anchorPoint );
		return;
		}
	case( enModuleType::mRailgunLauncher ) :
		{
		std::shared_ptr<RailgunLauncher> ptr_mlaunch = std::shared_ptr<RailgunLauncher>( dynamic_cast<RailgunLauncher*>(entity) );
		AddWPlatform( ptr_mlaunch, pos, anchorPoint );
		return;
		}
	case( enModuleType::mPowerSupply ) :
		{
		std::shared_ptr<PowerSupplyEntity> ptr_psupply = std::shared_ptr<PowerSupplyEntity>( dynamic_cast<PowerSupplyEntity*>(entity) );
		AddPowerSupply( ptr_psupply, pos, anchorPoint );
		return;
		}
	case( enModuleType::mRadar ) :
		{
		std::shared_ptr<RadarEntity> ptr_radar = std::shared_ptr<RadarEntity>( dynamic_cast<RadarEntity*>(entity) );
		AddRadar( ptr_radar, pos, anchorPoint );
		return;
		}
	case(enModuleType::mAmmoContainer):
		{
		std::shared_ptr<AmmoContainer> ptr_ammo = std::shared_ptr<AmmoContainer>(dynamic_cast<AmmoContainer*>(entity));
		AddAmmoContainer(ptr_ammo, pos, anchorPoint);
		return;
		}
	case( enModuleType::mBridge ) :
		{
		std::shared_ptr<ExternalModule> ptr_exterbal = std::shared_ptr<ExternalModule>(dynamic_cast<ExternalModule*>(entity));
		AddExternalModule(ptr_exterbal, pos, anchorPoint);
		return;
		}
	case( enModuleType::mWing ) :
		{
		std::shared_ptr<ExternalModule> ptr_exterbal = std::shared_ptr<ExternalModule>(dynamic_cast<ExternalModule*>(entity));
		AddExternalModule(ptr_exterbal, pos, anchorPoint );
		return;
		}
	case( enModuleType::mEngineBody ) :
		{
		std::shared_ptr<ExternalModule> ptr_exterbal = std::shared_ptr<ExternalModule>(dynamic_cast<ExternalModule*>(entity));
		AddExternalModule(ptr_exterbal, pos, anchorPoint );
		return;
		}
	case( enModuleType::mFrame ) :
		{
		std::shared_ptr<ExternalModule> ptr_exterbal = std::shared_ptr<ExternalModule>(dynamic_cast<ExternalModule*>(entity));
		AddExternalModule(ptr_exterbal, pos, anchorPoint );
		return;
		}
	}
}

void Vessel::LoadImpulseForceFromEngines()
{
	for (auto it = this->pModuleContainer.EE_begin(); it != this->pModuleContainer.EE_end(); ++it)
	{
		primitives::vector disturbance = (*it)->GetDisturbanceForce();
		if (disturbance.x*disturbance.y != 0)
		{
			this->setImpulseForce((*it)->GetDisturbanceForce());
			this->setImpulseAngularForce(statistics::GetIntInRange(-3, 3));
			(*it)->ResetDisturbanceForce();
		}
	}
}

void Vessel::HandleInternalModuleDetach(std::shared_ptr<ModuleEntity> module, const primitives::vector& direction)
{
	RemoveProperties(module);
	UnitEntity::HandleInternalModuleDetach(module, direction);
}

void Vessel::RemoveProperties(std::shared_ptr<ModuleEntity> in)
{
	//assumes not detached
	enModuleType mType = in->GetModuleType();
	switch (mType)
	{
	case(enModuleType::mController):
	{
		std::shared_ptr<ControllerEntity> ptr_ctrl = std::static_pointer_cast<ControllerEntity>(in);
		RemoveControllerProperties(ptr_ctrl);
		return;
	}
	case(enModuleType::mEngine):
	{
		std::shared_ptr<EngineEntity> ptr_engine = std::static_pointer_cast<EngineEntity>(in);
		RemoveEngineProperties(ptr_engine);
		return;
	}
	case(enModuleType::mLauncher):
	{

		return;
	}
	case(enModuleType::mMissileLauncher):
	{

		return;
	}
	case(enModuleType::mAntiMLauncher):
	{

		return;
	}
	case(enModuleType::mTorpedoLauncher):
	{

		return;
	}
	case(enModuleType::mRailgunLauncher):
	{

		return;
	}
	case(enModuleType::mPowerSupply):
	{

		return;
	}
	case(enModuleType::mRadar):
	{

		return;
	}
	case(enModuleType::mAmmoContainer):
	{
		//replace code here with RefreshAmmoMap() TODO
		return;
	}
	case(enModuleType::mBridge):
	{

		return;
	}
	case(enModuleType::mWing):
	{

		return;
	}
	case(enModuleType::mEngineBody):
	{

		return;
	}
	case(enModuleType::mFrame):
	{

		return;
	}
	}
}

bool Vessel::RemoveModuleEntity(std::shared_ptr<ModuleEntity> in)
{
	if (UnitEntity::RemoveModuleEntity(in))
	{
		if (in->IsDetached() == false)
			this->RemoveProperties(in);
		return true;
	}
	return false;
}



//int Vessel::GetThrust() 
//{
//	//if( this->wEngines.size() > 0 ) //currently apply from one engine only
//	//	return this->wEngines.begin()->get()->GetThrust();
//	return this->pCurrentThrust;
//}

void Vessel::RunModuleAnimations(int ms)
{
	//if(this->pDrawAnimations = true;) HINT use this later

	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		AnimationContainer& animations = (*it)->GetAnimationContainer();
		//if (it->get()->GetModuleType() == enModuleType::mEngineBody && GetAppliedThrust().DotProduct() > 100)
			animations.RunAnimations(ms);
		//else
		//	animations.SetAnimationDrawing(false);
	}
	AnimationContainer& animations = this->pGraphics->getGraphicalObject()->getMainObject()->GetAnimationContainer();
	animations.RunAnimations(ms);
}

void Vessel::SetDriveMagnitudes(int MaxSpeed, double MaxAngularVelocity, double MaxAngularAcceleration)
{
	this->pDriver.SetVelocityMagnitude(MaxSpeed);
	this->pDriver.SetAngularVelocityMagnitude(MaxAngularVelocity);
	this->pDriver.SetAngularAccelerationMagnitude(MaxAngularAcceleration);
}


void Vessel::SetDockDrive()
{
	if(this->pDriver.GetGuidance().waypointCount() > 0 )
		this->SetDrive( EnumEntityDrive::DynamicPosition);
	else
		this->SetDrive( EnumEntityDrive::DynamicTargetTracking );
}

