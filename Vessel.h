#pragma once
#include"FiguredGraphics.h"
#include"HitPointMap.h"
#include"DynamicEntityControlled.h"
#include"LauncherEntity.h"
#include"StorageEntity.h"
#include"EngineEntity.h"
#include"RadarEntity.h"
#include"PowerSupplyEntity.h"
#include"DockingEntity.h"
#include"ControllerEntity.h"
#include"SpriteObject.h"
#include"HitBoxEntity.h"
#include"ExternalModule.h"
#include"AmmoContainer.h"
#include"UnitEntity.h"
#include"LauncherProvider.h"
#include"EntityPrice.h"
#include<SDL_thread.h>


extern SDL_sem *vesselSem;

// Missing destructor??

class Vessel : public UnitEntity, public LauncherProvider
{

public:

	Vessel();
	Vessel( Coord pos);

	virtual void handleEvent(SDL_Event & e, const Coord& cameraPos);

	void AddModuleEntity( std::shared_ptr<ModuleEntity> in, Coord pos, Coord anchorPoint );
	virtual void AddWPlatform( std::shared_ptr<LauncherEntity> in, Coord pos, Coord anchorPoint ); //anchor point is not used.. ? most likely double check?
	//void AddStorageRoom( std::shared_ptr<StorageEntity> in, Coord pos, Coord anchorPoint ); 
	void AddEngine( std::shared_ptr<EngineEntity> in, Coord pos, Coord anchorPoint ); 
	void AddRadar( std::shared_ptr<RadarEntity> in, Coord pos, Coord anchorPoint );
	void AddPowerSupply( std::shared_ptr<PowerSupplyEntity> in, Coord pos, Coord anchorPoint );
	void AddController( std::shared_ptr<ControllerEntity> in, Coord pos, Coord anchorPoint );
	void AddAmmoContainer(std::shared_ptr<AmmoContainer> in, Coord relative_pos, Coord anchorPoint);
	void AddExternalModule( std::shared_ptr<ExternalModule> in, Coord pos, Coord anchorPoint ); 
	//void AddRotatingEntity( std::shared_ptr<RotatingEntity> in, Coord pos, Coord anchorPoint );
	virtual void AddGlobalListModule( std::string name, Coord pos, enObjectDiscreteRotation rotation, enObjectFlipped flipped, bool isPlayer );

	void updateVelocity(Uint32) override;
	void move(Uint32 milliseconds);

	void HandleExternalModuleDetach(std::shared_ptr<ExternalModule>, const primitives::vector& direction) override;
	void HandleModuleStaticDetach(std::shared_ptr<ExternalModule> toRemove) override;
	void DestroyEntity() override;

	virtual void TriggerBooster();
	void HandleInternalModuleDetach(std::shared_ptr<ModuleEntity>, const primitives::vector& direction) override;
	bool RemoveModuleEntity(std::shared_ptr<ModuleEntity> in) override;
	//virtual void DockToTarget( DynamicEntity* target );

	std::vector<std::shared_ptr<ExternalModule>>::const_iterator const EXT_begin() const { return pModuleContainer.EModules_begin(); };
	std::vector<std::shared_ptr<ExternalModule>>::const_iterator const EXT_end() const { return pModuleContainer.EModules_end(); };

	std::vector<std::shared_ptr<LauncherEntity>>::const_iterator const LE_begin() const { return pModuleContainer.LE_begin(); };
	std::vector<std::shared_ptr<LauncherEntity>>::const_iterator const LE_end() const { return pModuleContainer.LE_end(); };

	std::vector<std::shared_ptr<RadarEntity>>::const_iterator const RE_begin() const { return pModuleContainer.RE_begin(); };
	std::vector<std::shared_ptr<RadarEntity>>::const_iterator const RE_end() const { return pModuleContainer.RE_end(); };

	void RefreshGridObjects();
	int GetSummedHitPointsOfModules();

	void RunModuleAnimations(int ms) override;
	int GetThrust() override;
	int MaxSpeed();
	double MaxAngularAcceleration();
	double MaxAcceleration();
	double MaxAngularSpeed();

	double AngleDifferenceBasedOnVesselHeading(const Coord ToPosition);

	EntityPrice GetVesselPrice() const { return this->pPrice; }
	void SetPrice(statistics::RandomNumber price) { this->pPrice.SetNewPrice(price); }
	virtual EntityPrice GetTotalPrice( bool includeShipValue ) const; //sum price of all modules

	void SetDriveMagnitudes(int MaxSpeed, double MaxAngularVelocity, double MaxAngularAcceleration);
	void SetMoveModules(bool set) { this->pMoveModules = set; }

protected:

	//int pCurrentThrust;

	void Initialize();
	void RemoveProperties(std::shared_ptr<ModuleEntity> in);
	void RemoveEngineProperties(std::shared_ptr<EngineEntity> in);
	void RemoveControllerProperties(std::shared_ptr<ControllerEntity> in);
	
	//void SetThrust(int value);
	//void AddThrust(int value);

	//void SetAngularThrust(int value);
	//void AddAngularThrust(int value);

	SpriteObject* wShipInnerBody; //pointer is stored in tree structure in shared_ptr,

	
	void SetDockDrive();
	void LoadImpulseForceFromEngines();
	statistics::RandomNumber ShipModuleValue() const;

	private:

	EntityPrice pPrice;
	bool pLocomoteModules;
	bool pMoveModules;

};
