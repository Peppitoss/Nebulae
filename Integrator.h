#pragma once
#include"DynamicEntityControlled.h"
#include"ModuleEntity.h"
#include"LauncherEntity.h"
#include"InteriorEntity.h"
#include"Vessel.h"
#include"PlayerVessel.h"
//#include"PlayerWalker.h"
#include"CollisionDetect.h"
#include"GameCollisionHandler.h"
#include"ParticleEngine.h"
#include"VesselAI.h"
#include"Animation.h"
#include"Debri.h"
#include"GunPlatform.h"
#include"MaskEffector.h"
#include"Barrier.h"
#include"MissileProjectile.h"
#include"TorpedoProjectile.h"
#include"TimeUnit.h"
#include"Timer.h"

#include <SDL.h>
#include <SDL_image.h>
#include<list>

const int MaxProjectiles = 1000;

class ProjectileEngine
{
public:

	ProjectileEngine();

	//void AddNewProjectile(std::shared_ptr<Projectile>&)
	void MoveToDeleteProjectile(std::shared_ptr<Projectile>&);
	void MoveToNextActiveSetProjectile(std::shared_ptr<Projectile>&, bool isNewProjectile);

	std::array<std::shared_ptr<Projectile>, MaxProjectiles>* CurrentActiveSet() const { return pCurrentActiveSet; }
	std::array<std::shared_ptr<MissileProjectile>, MaxProjectiles>* CurrentAMissileSet() { return pAntiMissileSet; }
	std::array<std::shared_ptr<TorpedoProjectile>, MaxProjectiles>* CurrentTorpedoSet() { return pTorpedoMissileSet; }
	//std::vector<std::shared_ptr<Projectile>>* CurrentGenericSet() { return pGenericSet; }

	void ClearAll(bool clearVectors);
	void HandleProjectiles();
	int ProjectileCount() { return SDL_AtomicGet(&pProjectiles); }
	int NextProjectileCount() { return SDL_AtomicGet(&pNewProjectileCounter); }

private:

	SDL_atomic_t pProjectiles;
	SDL_atomic_t pNewProjectileCounter;
	SDL_atomic_t pDeleteProjectiles;
	bool Set1IsActive;

	std::array<std::shared_ptr<Projectile>, MaxProjectiles>* pCurrentActiveSet;
	std::array<std::shared_ptr<Projectile>, MaxProjectiles>* pNextActiveSet;

	std::array<std::shared_ptr<TorpedoProjectile>, MaxProjectiles>* pTorpedoMissileSet;
	std::array<std::shared_ptr<MissileProjectile>, MaxProjectiles>* pAntiMissileSet;
	std::array<std::shared_ptr<Projectile>, MaxProjectiles>* pGenericSet;

	std::array<std::shared_ptr<Projectile>, MaxProjectiles> pActiveSet1;
	std::array<std::shared_ptr<Projectile>, MaxProjectiles> pActiveSet2;
	std::array<std::shared_ptr<Projectile>, MaxProjectiles> pToDelete;

};


//class timer
//{
//
//public:
//
//	timer();
//	void StartTick();
//	void StopTick();
//
//	bool TickSumEqual(Uint32 sum);
//	Millisecond TickRequest();
//
//	double  GetAverageTick() const { return this->pCurrentAverageTick; }
//	Millisecond GetStepTime();
//
//	Uint32 GetTickSum() { return this->pTickSum; }
//
//private:
//
//	double pTicksUsedForAverageValue;
//	double pAverageTickSum;
//	double pCurrentAverageTick;
//	int pAverageCount;
//
//	Uint32 pFirstTick;
//	Uint32 pSecondTick;
//
//	Uint32 pTickLength;
//	Uint32 pTickSum;
//
//};

//enum enIntegratorMode
//{
//	enSpaceCombat = 1,
//	enInteriorCombat = 2,
//	enFail = 3
//};

static const std::string lMissile = "missile";
static const  std::string lRail = "rail";
static const  std::string lTorpedo = "torpedo";
static const  std::string lAntiMissile ="amissile";
static const std::string lcannon = "cannon";

int tProjectileThread(void* data);
int tParticleThread( void* data );
int tCollisionDetectionThread( void* data );
int tVesselComputeThread(void* data);
//int particleTestThread( void* data );

class integrator
{

public:

	integrator();
	integrator(bool fixedIndependentStepTime);

	~integrator();

	double getSpeed() const { return pSpeed; };
	std::shared_ptr<Animator> getAnimator() const { return pAnimator; };
	//std::shared_ptr<MaskEffector> getMaskEffects() const { return pMaskEffects; };

	bool run(int stepTimeMs);
	void runDynamic(double ml);
	//void runLauncher(double ml);
	//void runLauncher(std::shared_ptr<LauncherEntity> launcher, double ml);
	virtual void RunUnitEntity(double ml) = 0;
	virtual	void RunPlayer(double ml) = 0;
	virtual void RunOtherEntities(double ml) {};
	void runParticleEffect(double ml);
	void runAI(double ml);
	bool runAI(std::shared_ptr<VesselAI>, double milliseconds);
	void runGunPlatform(double ml);
	void runDebri(double ml);
	void runAnimator(double ml);
	//void runMaskEffects(double ml);

	void runProjectiles(double ml);
	void runProjectiles(int lowBound, int upperBound);
	void runProjectiles(int modulo);
	//
	double GetAverageTick() const { return this->pTimer->GetAverageTick(); }
	//

	bool collDetectEnemyVessel(std::shared_ptr<UnitEntity> object);
	bool collDetectAlliedVessel(std::shared_ptr<UnitEntity> object);

	bool collDetectPlayer();
	bool collDetectPlayer(int lowBound, int higherBound);
	void CollDetectAntiMissile(); //first only torpedo
	void CollDetectAntiMissile(std::shared_ptr<Projectile> projectile);
	virtual void CollDetect() = 0;
	//
	void collHandle(int ms);
	//
	//void addObject( std::shared_ptr<DynamicEntity> object ) {  dynamicObjects.push_back(object); };  // no in use
	bool IsDisabled() const { return this->pDisabled; }

	void addDynamicEntity(std::shared_ptr<DynamicEntity> object) { dynamicObjects.push_back(object); };
	void addLauncher(std::shared_ptr<LauncherEntity> object);
	void addAI(std::shared_ptr<VesselAI> object) { vesselAIObjects.push_back(object); };
	void addGunPlatform(std::shared_ptr<GunPlatform> object) { gunPlatforms.push_back(object); };
	void setAnimator(std::shared_ptr<Animator> object) { pAnimator = object; };
	//void setMaskEffector(std::shared_ptr<MaskEffector> object) { pMaskEffects = object; };
	void setSpeed(Uint32 set);
	void addInteriorModel(std::shared_ptr<InteriorEntity> model) { this->interiorObject = model; };
	//

	void RemoveLaunchersOfEntity(std::shared_ptr<LauncherProvider> object);
	void RemoveLauncher(std::shared_ptr<LauncherEntity> object);
	void RemoveLauncher(string key, std::shared_ptr<LauncherEntity> object);
	virtual void RemoveUnit(std::shared_ptr<UnitEntity> object) = 0;
	//
	void refreshEntity(DynamicEntity*);
	//
	virtual void HandleEvents(SDL_Event & e, const Coord& cameraPos);
	//void handlePlayerEvents(SDL_Event & e, const Coord& cameraPos);
	virtual void HandlePlayer(Uint32 ms) = 0;
	//
	bool ComputeUnitDynamic(std::shared_ptr<UnitEntity>, double ml, enumAllegiance allegiance);
		void ComputeWPlatformDynamic(GunPlatform*, double ml);
	//
	bool ReduceTimeOnProjectile(std::shared_ptr<Projectile>& projectile, double ml);
	//
	bool hasInterior() const { return this->interiorObject.get(); }

	//void SetIntegrationMode(enIntegratorMode mode) { this->pMode = mode; };
	//enIntegratorMode GetIntegrationMode() const { return this->pMode; };

	std::list<std::shared_ptr<DynamicEntity>> dynamicObjects;
	std::list<std::shared_ptr<GunPlatform>> gunPlatforms;

	std::map<std::string, std::list<std::shared_ptr<LauncherEntity>>> launcherObjects;
	std::list<std::shared_ptr<VesselAI>> vesselAIObjects;
	std::list<std::shared_ptr<Debri>> vesselDebri;
	std::shared_ptr<InteriorEntity> interiorObject;

	void AddCollisionData(CollisionHandle_DE_PR* add);

	std::vector<CollisionHandle_DE_PR*> DEPR_CollisionHandle;
	std::vector<CollisionHandle_PR_PR*> PRPR_CollisionHandle;
	//std::list<CollisionHandle_DE_PR*> VEPR_CollisionHandle; //std::list<CollisionHandle_VE_PR*>
	std::list<CollisionHandle_DE_DE*> DEDE_CollisionHandle;

	void setParticleEngine(std::shared_ptr<particleEffects::ParticleEngine> engine) { pParticleEngine = engine; };

	//void setStepTimeFixed(bool set) { fixedIndependentStepTime = set; };
	void pause();
	void unpause();
	bool IsPaused() const { return this->isPaused; };
	//void unpause(bool checkLock);

	std::array<std::shared_ptr<Projectile>, 1000>* CurrentActiveProjectiles() const { return this->pDefaultProjectiles.CurrentActiveSet(); }
	int ActiveProjectiles() {	return this->pDefaultProjectiles.ProjectileCount();	}
	static GenericEntity* CurrentEventObject;

	virtual bool IsSpaceMode() = 0;
	virtual bool IsInteriorMode() = 0;

	virtual std::weak_ptr<DynamicEntity> CurrentPlayer() = 0;
	virtual bool SetPlayer(std::shared_ptr<PlayerInterface> player) = 0;

	Coord PlayerPosition();


	std::shared_ptr<particleEffects::ParticleEngine> pParticleEngine;//needs to be a shared pointer?? for optimization?  //temporarily public for testing


	void ResetObjects();
	virtual std::shared_ptr<UnitEntity> GetUnitEntityFromID(int ID) = 0;
	virtual std::vector<std::shared_ptr<UnitEntity>> GetUnitEntities(bool enemy) = 0;

	virtual void RemoveTargetSelectionOfUnit(Widget* ExcludeVesselOfLink) const = 0;
	virtual void RemoveTargetSelectionOfUnit(std::shared_ptr<UnitEntity> IncludeThisVessel) const = 0;

	virtual enumAllegiance GetUnitAllegiance(std::shared_ptr<UnitEntity> unit) = 0;

	//Threading


	void ClearUnitThreads() { SDL_AtomicSet(&this->pClearUnitThreads, 1); }
	void EnableUnitThreads() { SDL_AtomicSet(&this->pClearUnitThreads, 0); }
	bool RunUnitThreads() { return SDL_AtomicGet(&this->pClearUnitThreads) == 0; }

	bool RunOtherThreads() { return SDL_AtomicGet(&this->pClearParticleAndProjectileThreads) == 0; }
	void DisableOtherThreads();

	void DecrementThreadCollisionCounter();
	void IncrementThreadCollisionCounter();

	void DecrementThreadProjectileHandler();
	void IncrementThreadProjectileHandler();

	void DecrementThreadParticleHandler();
	void IncrementThreadParticleHandler();
	//void GenerateVesselThread(std::shared_ptr<Vessel>);
	//void GenerateCollisionThread(std::shared_ptr<Vessel> vessel);

	Millisecond GetCurrentTimeStep(bool includeSpeed);

	int GetParticleThreadsAmount() const { return tParticles.value; }
	int GetProjectileThreadsAmount() const { return tProjectiles.value; }
	//int GetVesselThreadsAmount() const { return tVessels; }
	int GetMaxProjectileThreadsAmount() const { return this->pProjectileThreads; }
	int GetMaxParticleThreadsAmount() const { return this->pParticleEngineThreads; }
	//static SDL_sem* intloopsem;
	barrier::ConditionWaitBarrier MParticleBarrier;
	barrier::ConditionWaitBarrier MProjectileBarrier;
	barrier::ConditionWaitBarrier MCollisionBarrier;
	barrier::ConditionWaitBarrier MCollisionPlayerBarrier;
	barrier::ConditionWaitBarrier MUnitBarrier;

	barrier::ConditionWaitBarrier MVesselThreadCreationBarrier;
	//static SDL_sem* projectilelistsem;
	//static int projectilecounter;
	static SDL_mutex* pProjectileThreadCreationLock;
	static SDL_mutex* pParticleThreadCreationLock;
	static SDL_mutex* pVesselThreadCreationLock;
	static SDL_mutex* pCollisionThreadCreationLock;
	static SDL_mutex* pAddCollisionData;
	static SDL_mutex* pRemoveLaunchersLock;

	ProjectileEngine pDefaultProjectiles;
	//ProjectileEngine<MissileProjectile> pAntiMissileProjectiles;

	void IncrementThreadUnitCounter();
	void DecrementThreadUnitCounter();

	protected:

	void CompuiteUnitEntity(std::shared_ptr<UnitEntity> entity, int ms, enumAllegiance allegiance);
	bool ThreadsInitialized() { return this->pThreadsInitialized; }

	Millisecond currentStepTime;


private:

	void Initialize();
	void ResetVesselThreads();
	void InstantiateThreads();
	//enIntegratorMode pMode;

	timer* pTimer;
	GameCollisionHandler cHandler;
	
	std::shared_ptr<Animator> pAnimator; //needs to be a shared pointer?? for optimization?
	//std::shared_ptr<MaskEffector> pMaskEffects;

	void MoveIterateUnitEntity(std::shared_ptr<UnitEntity> entity, int ms);
	bool HandleUnitEntityTimer(std::shared_ptr<UnitEntity> entity, int ms);


	//Millisecond pLastStepTime;

	int pPauseLock;
	static const int MAXSPEED = 100;
	static const int MINSPEED = 1;
	double pSpeed; //percentage, 1000 = 1 iteration per second, 2000 = 0.5 iterations pr seconds (twice as slow)
	//bool fixedIndependentStepTime;
	bool isPaused;
	bool pDisabled;
	static const int minimalStepTime = 1;
 
	void ClearUnitEntities();
	virtual void ClearVessels();
	virtual void ClearWalkers();
	void ClearLauncherObjects();
	void ClearCollisionDetect();

	virtual void LoadPlayerLaunchers() = 0;

	integrator( const integrator& );
	integrator( integrator&& );
	integrator& operator=( const integrator& );
	integrator& operator=( integrator&& );

	static bool pThreadsInitialized;
	int pParticleEngineThreads;
	int pProjectileThreads;

	SDL_atomic_t pClearUnitThreads;
	SDL_atomic_t pClearParticleAndProjectileThreads;

	SDL_atomic_t tParticles;
	SDL_atomic_t tProjectiles;
	SDL_atomic_t tUnits;
	SDL_atomic_t tCollision;

	void TriggerParticleBarrier(int color);
	void TriggerUnitBarrier(int color);
	void TriggerCollisionBarrier(int color);
	void TriggeProjectileBarrier(int color);
	void TriggerPlayerCollisionBarrier(int color);
};

