#pragma once
#include"Graphics.h"
#include"InGameGraphicalObject.h"
#include"ShipImageDefinitions.h"
#include"GridObjectCollection.h"
#include"MissileLauncher.h"
#include"Projectile.h"
#include"DockingEntity.h"
#include"Vessel.h"
#include"PlayerProfile.h"
#include"Animation.h"
#include"GunProjectile.h"
#include"MissileProjectile.h"
#include"EngineEntity.h"
#include"RadarEntity.h"
#include"PowerSupplyEntity.h"
#include"ControllerEntity.h"
#include"Integrator.h"
#include"BoxList.h"
#include"Background.h"
#include"InventoryItem.h"
#include"Location.h"

//static void PlayerMoveToLink(SDL_Event&, Widget* );

// ---------------------- REMINDER -------------------------//
//        to set color on edge boxes
//        aiShip->getGraphics()->getSprites()->getMainNode()->getSprite()->colorSetEdge();
// ---------------------- REMINDER -------------------------//
class InstanceManager
{

public:
	InstanceManager();
	InstanceManager( SDL_Renderer* );
	void SetupManager( std::shared_ptr<Animator> , std::shared_ptr<particleEffects::ParticleEngine> , std::shared_ptr<MaskEffector> );
	void SetupInstanceDebug( SDL_Renderer* );
	//void SetupInstance(std::shared_ptr<LocationData> data, std::shared_ptr<PlayerInterface> player);
	void SetupSpaceInstance(Location& data, std::shared_ptr<PlayerVessel> player, bool resetIntegrator);
	void SetupInteriorInstance(Location& data, std::shared_ptr<PlayerWalker> player, bool resetIntegrator);

	//void ReloadCurrentInstance(const PlayerProfile& profile);
	//void SpawnUnit(int UnitID);
	void SpawnUnit(const std::set<int>& UnitID);
	void ResetSpeed();

	static std::shared_ptr<integrator> GetIntegrator() { return pIntegrator; };
	//static std::shared_ptr<PlayerVessel> GetPlayerVessel() { return pPlayerVessel; };
	//static std::shared_ptr<PlayerVessel> SetupProfileVesselAndWalker(const PlayerProfile& profile);
	//static std::shared_ptr<PlayerVessel> SetupPlayerVessel(string vesselName);
	//static std::shared_ptr<PlayerWalker> GetPlayerWalker() { return pPlayerWalker; };
	static std::shared_ptr<PlayerInterface> GetCurrentPlayer() { return pCurrentPlayer; }
	Background* getBackground() { return pCurrentBackground; };

	bool IsPreviousLocationPlayable();
	//bool LocationIsNeutral(); 
	//~InstanceManager();

	void HandleLocationChange();

	void SetCurrentLocation(Location& set);
	Location CurrentLocation() { return this->pCurrentLocation; }
	Location PreviousLocation() { return this->pPreviousLocation; }

private:

	Location pCurrentLocation;
	Location pPreviousLocation;
	void SetupBackground(const Coord& locationSize);

	std::shared_ptr<GunProjectile> AddProjectile( std::string name, int size );
	std::shared_ptr<MissileProjectile> AddMissile( std::string name, int size );
	std::shared_ptr<LauncherEntity> AddLauncherEntity( std::string name, int size  );
	std::shared_ptr<MissileLauncherEntity> AddMLauncherEntity( std::string name, int size );
	std::shared_ptr<EngineEntity> AddEngine( std::string name, int size );
	std::shared_ptr<RadarEntity> AddRadar( std::string name, int size );
	std::shared_ptr<PowerSupplyEntity> AddPowerSupply( std::string name, int size );	
	std::shared_ptr<ControllerEntity> AddController( std::string name, int size );
	//std::shared_ptr<Vessel> AddVessel( SOTreePointer, const Coord& position );
	//std::shared_ptr<Vessel> AddVessel( SOTreePointer );
	std::shared_ptr<GunPlatform> AddRotEntity( GOTreePointer, const Coord& position );
	//static std::shared_ptr<PlayerVessel> SetupPlayerVessel( SOTreePointer );
	
	std::shared_ptr<Animator> pAnimator;
	std::shared_ptr<particleEffects::ParticleEngine> pParticleEffector;
	std::shared_ptr<MaskEffector> pMaskEffector;

	std::shared_ptr<StorageEntity> AddStorageEntity( int width, int height, const GridObjectTexturePack&, spriteSize size ); //delete warning here

	void SetupVesselAI(const ShipModel& model, std::shared_ptr<Vessel> );
	void SetupVesselStandardAI(const ShipModel& model, std::shared_ptr<Vessel>);
	//imagehandling::TextureCollection* txtCollection;

	static std::shared_ptr<integrator> pIntegrator;
	//static std::shared_ptr<PlayerVessel> pPlayerVessel;
	static std::shared_ptr<PlayerInterface> pCurrentPlayer;
	Background* pCurrentBackground;

	std::list<std::shared_ptr<GenericEntity>> pInstanceEntities;
	std::list<std::shared_ptr<GraphicalEntity>> pInstanceGraphicalEntities;

	void AddShipToIntegrator(ShipModel& model);
	void AddWalkerToIntegrator(ShipModel& model);
	
	bool pUseRealTimeSpeed; // false is for debugging (fixed timestep)

};