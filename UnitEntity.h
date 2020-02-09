#pragma once
#include"DynamicEntityControlled.h"
#include"UnitModuleContainer.h"

enum enumUnitState
{
	UNDEFINED = 0,
	NORMAL = 1,
	DOCKING,
	DOCKED
};

enum enReTargetStrategy
{
	NOCHANGE = 0,
	RANDOM = 1,
	CUSTOMAI = 2,
	CLOSEST = 3,
};

enum enVesselSize
{
	vNone = 0,
	v64 = 64,
	v128 = 128,
	v256 = 256,
	v512 = 512,
	v1024 = 1024,
	v2048 = 2048
};

class UnitEntity : public DynamicEntityControlled
{

public:

	UnitEntity();
	UnitEntity(const Coord pos);

	void Fire();
	void DeFire();

	std::vector<std::shared_ptr<Projectile>> RunLaunchers(int ms);
	void UpdateLauncherAmmo( std::shared_ptr<LauncherEntity> launcher);
	std::shared_ptr<Projectile> RunLaunchProjectile(std::shared_ptr<LauncherEntity> launcher, int ms);
	void RunDetached(int ms);

	void ApplyDamage(const Coord &, const game::Damage&) override;
	void AddDamageRecieved(const game::Damage& set);
	virtual void DestroyEntity() override;
	void SetHitPointZero();
	void setTexture(std::shared_ptr<GraphicalEntity>) override;

	enumUnitState GetUnitState() { return this->enUnitState; };
	virtual void HandleUnitState(Uint32 milliseconds);
	void SetUnitState(enumUnitState state) { this->enUnitState = state; }

	virtual void IncrementModuleLocmotionFrames();
	void SetLocomotionSpeed(int set) { this->pStepLocomotion.SetSpeed(set); }
	void AddLocomotionFrame(const Coord& Coord) { this->pStepLocomotion.GetFrames().push_back(Coord); }
	void ResetModuleLocomotion();
	void ResetLocomotionCounter();

	virtual void DetachAllModules(const primitives::vector& direction, double directionAngleSpread);
	virtual void DetachModule(std::shared_ptr<ExternalModule> module, const primitives::vector& direction);
	bool HasDetachModulesToProcess();
	void ProcessDetachedModules(int millisecond);
	void UpdateContainedListOfExternalModules();

	void ActivateModule(enModuleType type, int ID);
	bool IsModuleActive(enModuleType type, int ID);
	void DisableModule(enModuleType type, int ID);

	virtual void HandleExternalModuleDetach(std::shared_ptr<ExternalModule>, const primitives::vector& direction);
	virtual void HandleInternalModuleDetach(std::shared_ptr<ModuleEntity>, const primitives::vector& direction);
	virtual void HandleModuleStaticDetach(std::shared_ptr<ExternalModule> toRemove);

	Coord TransformPositionToLinkFrame(const Coord& pos, const ModuleEntity& module);
	Coord TransformPositionToLinkFrame(const Coord& pos, InGameGraphicalObjectLink& link);
	ModuleEntity* GetModuleFromCoord(const Coord&);
	std::shared_ptr<ModuleEntity> GetModulesWithGridObject(const std::shared_ptr<GridObject> object);
	std::shared_ptr<ModuleEntity> GetModulesFromID(enModuleType type, int ID);
	std::shared_ptr<ExternalModule> GetExternalModuleFromCoord(const Coord&);
	std::vector<std::shared_ptr<ModuleEntity>> GetInternalModulesList() const;
	std::vector<std::shared_ptr<ExternalModule>> GetExternalModulesList() const;
	std::vector<std::shared_ptr<ModuleEntity>> GetAllModules() const;
	std::vector<std::shared_ptr<LauncherEntity> > GetLauncherList() const;
	bool ContainsGridObject(std::shared_ptr<GridObject> object, bool checkInternalContainers);
	bool ContainsAmmo(std::shared_ptr<GridObject> object);
	bool ContainsInternalModule(std::shared_ptr<ModuleEntity> entity);

	void AddGraphicalObjectToBody(std::shared_ptr<InGameGraphicalObject> object, double relationalHeading, int ID);
	std::shared_ptr<FiguredGraphics> GetUnitGraphics() const;
	const std::shared_ptr<PixelFigure> GetFigure() const;
	void AddHitBox(int width, int height, Coord pos, enHitBoxCollision type);

	virtual bool RemoveModuleEntity(std::shared_ptr<ModuleEntity> in);

	void DefineChassis(std::shared_ptr<FiguredGraphics> set, string chassisName);
	void DefineChassis(ModuleEntity* set, string chassisName);
	std::string GetChassisName() const { return this->pChassisName; };

	virtual bool DecrementAmmo(string type); //THIS will cause trouble in code, f.eks if amount is 20, but ammo left is 10, then player can still fire!!!, should not be like this
	bool AddAmmo(std::shared_ptr<ItemAmmo> ammo, Coord pos);	//only adds to first container in list!!
	bool RemoveAmmo(std::shared_ptr<ItemAmmo> ammo);	//returns false if ammo not found
	virtual bool HasAmmo(string type);
	std::vector<std::shared_ptr<AmmoContainer> > GetAmmoContainers() const { return this->pModuleContainer.GetAmmoContainerList(false); }

	virtual void TargetVessel(int vesselID);
	virtual void TargetVessel(std::shared_ptr<UnitEntity> target);
	virtual void AddTarget(std::shared_ptr<UnitEntity> target);
	virtual void RemoveTarget();
	void SetTargetSelectLinkState(bool ActiveState);

	bool AllModulesDetached();
	std::string GetUnitName() const { return this->pUnitName; };
	void SetUnitName(std::string set) { this->pUnitName = set; };

	virtual void RunModuleAnimations( int ms );

	virtual void RefreshGridObjects() = 0;

	void AddToTargetList(std::shared_ptr<UnitEntity> target);
	enReTargetStrategy GetReTargetStrategy() const { return this->pTargetStrategy; }
	void SetReTargetStrategy(enReTargetStrategy set) { this->pTargetStrategy = set; }

	void ChangeLauncherTargets();
	void RandomRetargetLaunchers();
	void ClosestReTargetLaunchers();

	int getTimer() { return pInternalTimerMs; };
	void resetTimer() { pInternalTimerMs = 0; };
	void incrementTimer(int set) { pInternalTimerMs = std::min(40000,pInternalTimerMs + set); };
	void setTimer(int set) { pInternalTimerMs = set; };

	bool IsSpawned()const { return pIsSpawned; }
	void SetIsNotSpawned() { pIsSpawned = false; }
	void SpawnVessel( bool refreshHUDList );

	bool IsInvulnerable() const { return this->pIsInvulnerable; }
	void SetInvulnerable(bool value) { this->pIsInvulnerable = value; }

protected:

	unsigned int pCurrentModuleID;
	int pChangeTargetTimer;

	UnitModuleContainer pModuleContainer;
	bool SetupNewModuleGraphics(std::shared_ptr<ModuleEntity> module, const Coord& pos, const Coord& anchorPoint);

	bool LocomotionCanExecute() { return this->pStepLocomotion.CanExecute(); }
	void ExecuteLocomotion(int milliseconds); //P.S Exact same code as in ModuleEntity :(
	void IncreaseLocomotionFrame(); //P.S Almost exact same code as in ModuleEntity :(
	virtual void UpdateLocomotionFrames();

	void LocomoteExternalModules( int milliseconds );
	void ChangeHeadingForExternalModules(int milliseconds);

	void HandleSingleHit(const Coord & Coord, const game::Damage& damageType);
	void setHitDamage(ModuleEntity* hitModule, const Coord & Coord, const game::Damage& damage);

	void UpdateLaunchPoints();
	void UpdateLaunchPoint(std::shared_ptr<LauncherEntity>);
	void ChangeLaunchersDirection(Coord ToPoint);
	virtual void ChangeLaunchersDirection(double angle);
	virtual void ChangeExternalModuleDirection(bool bridge, bool frame, bool wings, double angle);
	void RotateModuleGraphics(bool bridge, bool frame, bool wings, bool launcher, double angle);

	std::set<int> GetIndicesOfModules(bool bridge, bool frame, bool wings, bool launcher);

	std::vector<std::shared_ptr<UnitEntity>> pTargetList;

private:

	void Initialize();

	EntityStepLocomotion pStepLocomotion;
	string pChassisName;
	int pInternalTimerMs;
	std::string pUnitName;
	bool pIsSpawned;

	bool pIsInvulnerable;
	enumUnitState enUnitState;
	enReTargetStrategy pTargetStrategy;

};