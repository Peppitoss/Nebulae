#include"InstanceManager.h"
#include"SpriteObject.h"
#include"XMLFormatReader.h"
#include"ModuleInterfaceGlobalList.h"
#include"WalkerGlobalList.h"
#include"VesselGlobalList.h"
#include"ProjectileGlobalList.h"
#include"InteriorGlobalList.h"
#include"Behaviours.h"
#include"WorldManager.h"
#include"GameManager.h"
#include"VesselIntegrator.h"
#include"WalkerIntegrator.h"

//std::shared_ptr<PlayerVessel> InstanceManager::pPlayerVessel = nullptr;
//std::shared_ptr<PlayerWalker> InstanceManager::pPlayerWalker = nullptr;
std::shared_ptr<PlayerInterface> InstanceManager::pCurrentPlayer = nullptr;
std::shared_ptr<integrator> InstanceManager::pIntegrator = nullptr;

InstanceManager::InstanceManager()
{
	pCurrentPlayer = NULL;
	pCurrentBackground = NULL;

	pUseRealTimeSpeed = true;
	pInstanceEntities = std::list<std::shared_ptr<GenericEntity>>();
	pInstanceGraphicalEntities = std::list<std::shared_ptr<GraphicalEntity>>();

}

InstanceManager::InstanceManager(  SDL_Renderer* rend )
{
	this->pCurrentPlayer = nullptr;
	pCurrentBackground = NULL;

	pUseRealTimeSpeed = true;; //this one (true is realtime speed, false is for debugging)
	pInstanceEntities = std::list<std::shared_ptr<GenericEntity>>();
	pInstanceGraphicalEntities = std::list<std::shared_ptr<GraphicalEntity>>();

}

void InstanceManager::SetupManager(std::shared_ptr<Animator> animator, std::shared_ptr<particleEffects::ParticleEngine> pe, std::shared_ptr<MaskEffector> effector)
{
	pAnimator = animator;
	pParticleEffector = pe;
	pMaskEffector = effector;

	pIntegrator = std::shared_ptr<VesselIntegrator>(new VesselIntegrator(!pUseRealTimeSpeed));
	pIntegrator->setSpeed(GameManager::GetIntegratorSpeed());
	pIntegrator->setParticleEngine( pe );
	pIntegrator->setAnimator( animator );
	//pIntegrator->setMaskEffector(effector);
	//pIntegrator->InstantiateThreads();
	this->pCurrentPlayer = nullptr;
	//SetupPlayerVessel(VesselGlobalList::GetPlayerVesselName());
	//SetupPlayerWalker(WalkerGlobalList::GetPlayerWalkerName());
	//pIntegrator->setPlayerVessel(pPlayer);
	//pIntegrator->setPlayerWalker(pPlayerWalker);
	SetupBackground(Coord(0,0));
}

std::shared_ptr<StorageEntity> InstanceManager::AddStorageEntity( int width, int height, const GridObjectTexturePack& texturePack, spriteSize isize ) //delete warning here
{
	BoxList StorageRoom;
	StorageRoom.generateRectangle(width,height);

	Coord ObjectOrigo = Coord(std::floor((double)width/2), (height)/2 );
	StorageRoom.Translate(ObjectOrigo*-1); // see generateGridObjectForModule in ModuleEntity

	int size = isize.GetSize();
	//int origo = std::max(width,height)/2;

	if( width > size || height > size )	//Put a limit on room size of 32 currently
		return NULL;

	std::vector<CoordWithColor> colorList = std::vector<CoordWithColor>(); //no delete
	colorList.reserve( StorageRoom.GetSize().GetSize() );

	for( std::list<Coord>::const_iterator it = StorageRoom.begin(); it != StorageRoom.end(); ++it )
	{
		if( StorageRoom.isCorner(*it) )
			colorList.push_back( CoordWithColor( it->x + size/2, it->y + size/2, colorFromEnum( EnumColor::BLACK ) ) ); //use size/2 ??? 
		else
			colorList.push_back( CoordWithColor( it->x + size/2, it->y + size/2, colorFromEnum( EnumColor::WHITE ) ) );
	}
	imagehandling::IMGToSurface* roomSurface = new imagehandling::IMGToSurface(colorList, isize );
	SOPointer newSprite = SOPointer(new SpriteObject( std::shared_ptr<Sprite>( new Sprite(roomSurface)), false, false) ); 
	std::shared_ptr<FiguredGraphics> newGe = std::shared_ptr<FiguredGraphics>(new FiguredGraphics(newSprite, EntityClass::ROOM, true )); 
	std::shared_ptr<StorageEntity> result = std::shared_ptr<StorageEntity>(new StorageEntity(newGe, texturePack, StorageRoom ));
	return result;
}

void InstanceManager::SetupSpaceInstance(Location& location, std::shared_ptr<PlayerVessel> player, bool resetIntegrator)
{
	auto data = WorldManager::GetLocation(location.GetName()).GetData(SDL_GetTicks());

	if (resetIntegrator)
	{
		pIntegrator->ResetObjects();
		pIntegrator = std::shared_ptr<VesselIntegrator>(new VesselIntegrator(!pUseRealTimeSpeed));

		pIntegrator->setParticleEngine(pParticleEffector);
		pIntegrator->setAnimator(pAnimator);


		player->setStateToNormal();

		//player->RefreshAmmoMap();	//run after shop
	}

	if (IsPreviousLocationPlayable() == false)
	{
		player->SetPosition(data.GetPlayerStartLocation());
		player->setVelocity(primitives::vector(0));
	}

	if (data.IncludePlayerInLocation())
	{
		pIntegrator->SetPlayer(player);
		pCurrentPlayer = player;
	}
	else
		pCurrentPlayer.reset();

	SetCurrentLocation(location);

	auto shipList = data.ShipList();
	for (auto it = shipList.begin(); it != shipList.end(); ++it)
		AddShipToIntegrator((*it));

	player->ResetRadarEntities();
	player->TargetVessel(nullptr);

	SetupBackground(location.GetDimension());
	pIntegrator->unpause();
}

void InstanceManager::SetupInteriorInstance(Location& location, std::shared_ptr<PlayerWalker> player, bool resetIntegrator)
{
	auto data = location.GetData();

	if (resetIntegrator)
	{
		pIntegrator->ResetObjects();
		pIntegrator = std::shared_ptr<WalkerIntegrator>(new WalkerIntegrator(!pUseRealTimeSpeed));
	}

	if (data.IncludePlayerInLocation())
	{
		pIntegrator->SetPlayer(player);
		pCurrentPlayer = player;
	}
	else
		pCurrentPlayer.reset();

	pIntegrator->setParticleEngine(pParticleEffector);
	pIntegrator->setAnimator(pAnimator);
	//pIntegrator->setMaskEffector(pMaskEffector);
	//player->ResetRadarEntities();

	if (data.GetInteriorModel().GetName() != "")
	{
		std::shared_ptr<InteriorEntity> interior = std::shared_ptr<InteriorEntity>(InteriorGlobalList::GetInterior(data.GetInteriorModel().GetName()));
		pIntegrator->addInteriorModel(interior);
	}

	for (auto it = data.ShipListBegin(); it != data.ShipListEnd(); ++it)
		AddWalkerToIntegrator((*it));

	player->setStateToNormal();
	if (IsPreviousLocationPlayable() == false)
	{
		player->SetPosition(data.GetPlayerStartLocation());
		player->setVelocity(primitives::vector(0));
	}
	player->TargetVessel(nullptr);
	player->RefreshAmmoMap();
	SetCurrentLocation(location);
	//pIntegrator->SetPlayer(pPlayerWalker);
	pIntegrator->unpause();
}

bool InstanceManager::IsPreviousLocationPlayable()
{
	return this->PreviousLocation().IsEmptyLocation() == false && this->PreviousLocation().Type != enLocationType::enBackgroundLocation;
}

void InstanceManager::SetCurrentLocation(Location& set)
{
	this->pCurrentLocation = set;
	if (IsPreviousLocationPlayable())
	{
		this->pCurrentLocation.SetPositionOffset(this->PreviousLocation().OuterDimension() / 2);
	}
	else
	{
		
	}
}

void InstanceManager::AddShipToIntegrator(ShipModel& model)
{
	if (VesselGlobalList::HasName(model.GetName()) == false)
		return;

	if (auto playerAsVessel = std::dynamic_pointer_cast<PlayerVessel>(this->GetCurrentPlayer()))
	{
		Coord spawnPosition = model.GetCurrentPosition() + this->pCurrentLocation.GlobalPosition().yflip();//primitives::RandomCoordinate(-spawnArea, -spawnArea, spawnArea, spawnArea, WorldManager::rEngine);

		std::shared_ptr<Vessel> vessel = std::shared_ptr<Vessel>(VesselGlobalList::GetVessel(model));
		vessel->SetPosition(spawnPosition);
		vessel->setUnitHeading(model.GetKinematics().CurrentHeading);
		vessel->SetID(model.GetID());
		vessel->SetUnitName(model.GetName());
		vessel->setAngularVelocity(model.GetKinematics().CurrentAngularSpeed);
		vessel->setVelocity(model.GetKinematics().CurrentVelocity);
		vessel->SetReTargetStrategy(enReTargetStrategy::RANDOM);
		if (model.IsInitiallyActive() == false)
			vessel->SetIsNotSpawned();
		SetupVesselAI(model, vessel);

		auto extenalKinematicsData = model.ExtenalEntityKinematics();
		for (auto external = vessel->EXT_begin(); external != vessel->EXT_end(); ++external)
		{
			auto value = extenalKinematicsData.find((*external)->GetIDName());
			if (value != extenalKinematicsData.end())
			{
				(*external)->SetKinematics(value->second);
				vessel->SetMoveModules(true);
			}
		}

		if (model.IsEnemy())
		{
			vessel->TargetVessel(playerAsVessel);
			vessel->AddToTargetList(playerAsVessel);
		}
		playerAsVessel->AddRadarTarget(vessel);
		std::dynamic_pointer_cast<VesselIntegrator>(pIntegrator)->AddVessel(vessel, true, model.IsEnemy());
	}
}

void InstanceManager::AddWalkerToIntegrator(ShipModel& model)
{
	if (WalkerGlobalList::HasName(model.GetName()) == false)
		return;

	Coord spawnPosition = model.GetCurrentPosition();//primitives::RandomCoordinate(-spawnArea, -spawnArea, spawnArea, spawnArea, WorldManager::rEngine);

	std::shared_ptr<WalkingEntity> walker = std::shared_ptr<WalkingEntity>(WalkerGlobalList::GetWalker(model.GetName()));

	//for (auto it = model.GetLinks().begin(); it != model.GetLinks().end(); ++it)
	//{
	//	LinkObject linkData = LinkObject::ReadLink((*it));
	//	walker->generateNewLink(linkData);
	//}
	
	walker->SetPosition(spawnPosition);
	walker->SetID(model.GetID());
	walker->SetUnitName(model.GetName());
	walker->SetReTargetStrategy(enReTargetStrategy::RANDOM);
	if (model.IsInitiallyActive() == false)
		walker->SetIsNotSpawned();

	std::dynamic_pointer_cast<WalkerIntegrator>(pIntegrator)->AddWalker(walker, model.IsEnemy());
}

void InstanceManager::SetupVesselAI(const ShipModel& model, std::shared_ptr<Vessel> vessel)
{
	auto behavior = model.GetBehaviour();

	if (behavior.IsEmpty())
		return;

	auto nodes = behavior.Nodes();

	for (int i = 0; i < nodes.size(); i++)
	{
		BehaviorNode& node = *nodes[i];
		int ID = node.GetActionArg().TargetVesselID;
		if (ID == PlayerModelID)
			node.GetActionArg().TargetObject = (Vessel*)this->GetCurrentPlayer()->AsUnitEntity();
		else if(this->GetIntegrator()->IsSpaceMode())
			node.GetActionArg().TargetObject = (Vessel*)this->GetIntegrator()->GetUnitEntityFromID(ID).get();
	}

	std::shared_ptr<VesselAI> AI = std::shared_ptr<VesselAI>(new VesselAI(vessel.get(), behavior));
	pIntegrator->addAI(AI);

	//std::shared_ptr<VesselAI> AI = nullptr;
	//switch (model.GetBehaviour())
	//{
	//case(enAIModel::NOACTION):
	//	SetupVesselStandardAI(model, vessel);
	//	break;
	//case(enAIModel::MOVETOTARGETCOORD):
	//	AI = std::shared_ptr<VesselAI>(new VesselAI(vessel.get(), AITree::GenerateTravelToCoordTree(model.GetTargetPosition())));
	//	pIntegrator->addAI(AI);
	//	break;
	////case(enActions::MOVETOTARGETENTITY):
	////	SetupVesselStandardAI(model, vessel);
	////	break;
	//case(enAIModel::AIPROBER):
	//	vessel->SetDrive(EnumEntityDrive::DynamicVelocityHeading);
	//	AI = std::shared_ptr<VesselAI>(new VesselAI(vessel.get(), AITree::GenerateInterceptTarget((Vessel*)this->GetCurrentPlayer()->AsUnitEntity(), true)));
	//	pIntegrator->addAI(AI);
	//	break;
	//case(enActions::HOLDPOSITION):
	//	SetupVesselStandardAI(model, vessel);
	//	break;
	//case(enActions::TEMPLATEFUNCTION):
	//	SetupVesselStandardAI(model, vessel);
	//	break;
	//case(enActions::TRACKTARGETENTITY):
	//	SetupVesselStandardAI(model, vessel);
	//	break;
}

void InstanceManager::SetupInstanceDebug( SDL_Renderer* rend )
{

	pIntegrator->ResetObjects();

	//SOPointer importship				= SOPointer(new SpriteObject( std::shared_ptr<Sprite>(new Sprite( ImportImage_ship2 )), true, true));
	//SOPointer importplatform			= SOPointer(new SpriteObject( std::shared_ptr<Sprite>(new Sprite( ImportImage_nemean_fighter )), true, true));
	//SOPointer importplayership			= SOPointer(new SpriteObject( std::shared_ptr<Sprite>(new Sprite( ImportImage_ship )), true, true));
	//GOPointer importirwin				= GOPointer(new SpriteObject( std::shared_ptr<Sprite>(new Sprite( ImportImage_irwin )), true, false));

	//GOTreePointer fIrwin 			= GOTreePointer( new InGameGraphicalObjectTree(importirwin));
	//GOTreePointer pPlatform			= GOTreePointer( new InGameGraphicalObjectTree(importplatform));
	//SOTreePointer sTarget			= SOTreePointer( new SpriteGraphicalObject(importplayership));
	//SOTreePointer sTree				= SOTreePointer( new SpriteGraphicalObject(importship));


	//std::shared_ptr<GunProjectile> project = AddProjectile( pTree );
	//std::shared_ptr<Projectile* projectile = ProjectileGlobalList::GetProjectile("missile", 64  );
	//MissileProjectile* missile = (MissileProjectile*)projectile.get();
	//missile->simpleMass = 80;
	//missile->simpleAngularAcceleration = 0.1;
	//missile->SetThrust(300);
	//std::shared_ptr<Projectile> projectile2 = ProjectileGlobalList::GetProjectile("missile", 64  );
	//MissileProjectile* missile2 = (MissileProjectile*)projectile2.get();
	//missile2->simpleMass = 80;
	//missile2->simpleAngularAcceleration = 0.1;
	//missile2->SetThrust(300);

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::shared_ptr<MissileLauncherEntity> wgunnerAI1 = AddMLauncherEntity("Speare SII",64);
	std::shared_ptr<MissileLauncherEntity> wgunnerAI2 = AddMLauncherEntity("Speare SII",64);

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//  REFACTORING ENDED HERE TO ALLOW DEBUGGING OF NEW STRUCTURE
	//
	//std::shared_ptr<Vessel> aiShip = AddVessel( sTarget );
//	aiShip->setGlobalHeading( 1.785 );
	//aiShip->simpleThrust = 4000;
	//aiShip->simpleMass = 1000;
	//aiShip->simpleAngularAcceleration = 1;
	//aiShip->setControllerGain(400);
	//aiShip->setControllerDerivative( 400);
	//aiShip->setControllerIntergral(0.500);
	//aiShip->setControllerGainHeading( 5 );
	//aiShip->setControllerDerivativeHeading(5);
	//aiShip->setControllerIntegralHeading(0);

	//aiShip->setLinkCallback( PlayerMoveToLink );

	//aiShip->AddWPlatform( wgunnerAI1, Coord(22,20),  Coord(aiShip->getSize()/2 - wgunnerAI1->getSize()/2) ); //correct size??
	//aiShip->AddWPlatform( wgunnerAI2, Coord(-20,20), Coord(aiShip->getSize()/2 - wgunnerAI2->getSize()/2) );
	//wgunnerAI1->ChangeMothership(aiShip);
	//wgunnerAI2->ChangeMothership(aiShip);

	std::shared_ptr<MissileLauncherEntity> wgunner = AddMLauncherEntity("Speare SII",64);
	std::shared_ptr<MissileLauncherEntity> wgunner2 = AddMLauncherEntity("Speare SII",64);

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ADD Rotation Platform
	//

	//std::shared_ptr<LauncherEntity> wGunnerPlatform = AddLauncherEntity( gunnerPlatform, 20, project, primitives::vector(-0.2,-10), Coord(-2,-10) );
	//int pSize = spriteSize(wGunnerPlatform->getEnumSize()).getIntValue();

	//std::shared_ptr<GunPlatform> gunPlatform = AddRotEntity( pPlatform, Coord(100,100));
	//gunPlatform->simpleMass = 1000;
	//gunPlatform->velocitySetPointAbsValue = 1;
	//gunPlatform->velocityHeadingSetPointAbsValue = 1;
	//gunPlatform->AddLauncher(wGunnerPlatform, Coord(20,0),  Coord(gunPlatform->getSize()/2 - pSize/2) );
	//wGunnerPlatform->setRateOfFire(500);

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//int gunnerVelocity = 10; //integrate with launcher entity or projectile later


	//if( VesselGlobalList::GetPlayerVesselName() == "" )
	//	SetupPlayerVessel( sTree );
	//else
	//	SetupPlayerVessel( VesselGlobalList::GetPlayerVesselName() );

	//pPlayer->generateSingleLink( Coord(100,100), "Test Text" );

	if( true )
	{

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::shared_ptr<StorageEntity> playerStorage = AddStorageEntity( 5,5, GridObjectCollection::GO_std_wallpack, Coord(64));
	int storageSpriteSize = playerStorage->getGraphics()->getSize();

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::shared_ptr<EngineEntity> playerEngine = AddEngine( "Brownian", 64 );

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::shared_ptr<RadarEntity> playerRadar = AddRadar( "Radar", 64 );

	//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::shared_ptr<PowerSupplyEntity> playerPowerSupply = AddPowerSupply( "PSupply", 64 );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	std::shared_ptr<ControllerEntity> playerController = AddController( "PID", 64 );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	}
}

void InstanceManager::SpawnUnit(const std::set<int>& UnitID)
{
	for (auto it = UnitID.begin(); it != UnitID.end(); ++it)
	{
		auto unit = GetIntegrator()->GetUnitEntityFromID(*it);
		if (unit != nullptr)
		{
			unit->SpawnVessel(false);
		}
	}
	GameManager::RefreshHUDVesselList();
}

void InstanceManager::HandleLocationChange()
{
	if (this->GetCurrentPlayer())
	{
		Coord playerposition = this->GetCurrentPlayer()->AsUnitEntity()->getPosition();
		auto connector = pCurrentLocation.GetConnector(playerposition);

		if (connector.ConnectionOutOfBounds)
		{
			if (PreviousLocation().GetConnector(playerposition).ConnectionOutOfBounds)
			{
				//destroy player
			}
		}
		else if (connector.NextLocationCoord == pCurrentLocation.GetSolarSystemPosition())
		{

		}
		else //if(connector.NextLocationCoord != this->pPreviousLocation.GetSolarSystemPosition())
		{
			this->pPreviousLocation = pCurrentLocation.CopyLocationWithoutData();
			GameManager::pauseGame();
			GameManager::TravelTo(connector.NextLocationCoord);
		}
		//else if (connector.NextLocationCoord == this->pPreviousLocation.GetSolarSystemPosition())
		//{
		//	currentlocation
		//}
		int test = 2;
	}
}

void InstanceManager::ResetSpeed()
{
	pIntegrator->setSpeed(GameManager::GetIntegratorSpeed());
}

std::shared_ptr<GunPlatform> InstanceManager::AddRotEntity( GOTreePointer tree, const Coord& position )
{
	std::shared_ptr<GraphicalEntity> ge = std::shared_ptr<GraphicalEntity>(new GraphicalEntity( tree, EntityClass::PLATFORM, true )); 
	std::shared_ptr<GunPlatform> result = std::shared_ptr<GunPlatform>(new GunPlatform(position));
	result->setTexture(ge);
	this->pInstanceGraphicalEntities.push_back( ge );
	this->pInstanceEntities.push_back( result );
	return result;
}

//std::shared_ptr<Vessel> InstanceManager::AddVessel( SOTreePointer tree)
//{
//	return AddVessel(tree, Coord(0) );
//}

std::shared_ptr<MissileLauncherEntity> InstanceManager::AddMLauncherEntity( std::string name, int size )
{
	std::shared_ptr<MissileLauncherEntity> result = std::shared_ptr<MissileLauncherEntity>((MissileLauncherEntity*) ModuleGlobalList::GetModule(name));
	return result;
}

std::shared_ptr<LauncherEntity> InstanceManager::AddLauncherEntity( std::string name, int size )
{
	std::shared_ptr<LauncherEntity> result = std::shared_ptr<LauncherEntity>((LauncherEntity*)(ModuleGlobalList::GetModule(name)));
	return result;
}


std::shared_ptr<EngineEntity> InstanceManager::AddEngine( std::string name, int size)
{
	std::shared_ptr<EngineEntity> result = std::shared_ptr<EngineEntity>((EngineEntity*)(ModuleGlobalList::GetModule(name)));
	return result;
}

std::shared_ptr<RadarEntity> InstanceManager::AddRadar( std::string name, int size )
{
	std::shared_ptr<RadarEntity> result = std::shared_ptr<RadarEntity>((RadarEntity*)(ModuleGlobalList::GetModule(name)));
	return result;
}

std::shared_ptr<PowerSupplyEntity> InstanceManager::AddPowerSupply( std::string name, int size )
{
	std::shared_ptr<PowerSupplyEntity> result = std::shared_ptr<PowerSupplyEntity>((PowerSupplyEntity*)(ModuleGlobalList::GetModule(name)));
	return result;
}

std::shared_ptr<ControllerEntity> InstanceManager::AddController( std::string name, int size )
{
	std::shared_ptr<ControllerEntity> result = std::shared_ptr<ControllerEntity>((ControllerEntity*)(ModuleGlobalList::GetModule(name)));
	return result;
}


std::shared_ptr<GunProjectile> InstanceManager::AddProjectile( std::string name, int size )
{
	std::shared_ptr<GunProjectile> result = std::shared_ptr<GunProjectile>((GunProjectile*)(ModuleGlobalList::GetModule(name)));
	return result;
}

std::shared_ptr<MissileProjectile> InstanceManager::AddMissile( std::string name, int size )
{
	std::shared_ptr<MissileProjectile> result = std::shared_ptr<MissileProjectile>((MissileProjectile*)(ModuleGlobalList::GetModule(name)));
	return result;
}

void InstanceManager::SetupBackground(const Coord& locationSize)
{
	if (!pCurrentBackground)
		pCurrentBackground = new Background( ImportImage_background_space, 50 ); //distance originallly 50, but has to be 0 for walker maps

	/*pCurrentBackground->GenerateBackgroundLocationBorder(locationSize);*/
	//pCurrentBackground->addTexture( ImportImage_background_sat->getSurface(), 3, false );
}

//InstanceManager::~InstanceManager()
//{
//	delete pPlayer;
//	delete pCurrentBackground;
//	delete pIntegrator;F
//
//	for( std::list<GraphicalEntity*>::const_iterator it = pInstanceGraphicalEntities.begin(); it != pInstanceGraphicalEntities.end(); ++it )
//	{
//		delete *it;
//	}
//	pInstanceGraphicalEntities.clear();
//
//	for( std::list<GenericEntity*>::const_iterator it = pInstanceEntities.begin(); it != pInstanceEntities.end(); ++it )
//	{
//		delete *it;
//	}
//	pInstanceEntities.clear();
//
//}
//void PlayerMoveToLink(SDL_Event& e, Widget* button)
//{
//	auto state = button->GetState();
//	switch ( button->GetState() )
//	{
//		case WS_HOVER:
//			break;
//		case WS_ACTIVE:
//			break;
//		case WS_ON:	
//			InstanceManager::getPlayer()->DockToTarget( (DynamicEntityControlled*)integrator::CurrentEventObject, enumDockingType::DOCKSTORE );
//			break;
//	}
//}