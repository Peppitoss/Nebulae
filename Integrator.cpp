#include "CollisionDetect.h"
#include "Integrator.h"
#include "MissileLauncher.h"
#include "TorpedoLauncher.h"
#include "RailgunLauncher.h"
#include "AntiProjectileMissileLauncher.h"
#include"StringBuilder.h"
#include <SDL_thread.h>


GenericEntity* integrator::CurrentEventObject = NULL;
//double integrator::currentStepTime = 0;
//SDL_sem* integrator::intloopsem = SDL_CreateSemaphore(0);
//barrier::MutexBarrier integrator::pMBarrier;
SDL_mutex* integrator::pProjectileThreadCreationLock = SDL_CreateMutex();
SDL_mutex* integrator::pParticleThreadCreationLock = SDL_CreateMutex();
SDL_mutex* integrator::pVesselThreadCreationLock = SDL_CreateMutex();
SDL_mutex* integrator::pCollisionThreadCreationLock = SDL_CreateMutex();
SDL_mutex* integrator::pAddCollisionData = SDL_CreateMutex();
SDL_mutex* integrator::pRemoveLaunchersLock = SDL_CreateMutex();

//SDL_atomic_t integrator::waitForMainLoop = SDL_atomic_t();
//PlayerVessel* integrator::currentPlayer = NULL;


int tCollisionDetectionThread(void* data)
{
	SDL_LockMutex(integrator::pVesselThreadCreationLock);//integrator::pCollisionThreadCreationLock);
	
	std::tuple<integrator*, std::shared_ptr<Vessel> >* recievedData = (std::tuple<integrator*, std::shared_ptr<Vessel> >*)data;
	integrator* parent = std::get<0>(*recievedData);
	std::shared_ptr<Vessel> vessel = std::get<1>(*recievedData);
	enumAllegiance allegiance = parent->GetUnitAllegiance(vessel);

	SDL_UnlockMutex(integrator::pVesselThreadCreationLock);//integrator::pCollisionThreadCreationLock);
	barrier::SetSDLCondWaitBarrier(&parent->MVesselThreadCreationBarrier, 1);
	barrier::SetSDLCondWaitBarrier(&parent->MVesselThreadCreationBarrier, 0);

	if (allegiance == enumAllegiance::aENEMY)
	{
		while (parent->RunUnitThreads())
		{
			barrier::SetSDLCondWaitBarrier(&parent->MCollisionBarrier, 1);
			if (vessel->IsSpawned())
			{
				if (vessel->isDestroyed() || !vessel->getGraphics()->isActive())
				{
					barrier::SetSDLCondWaitBarrier(&parent->MCollisionBarrier, 0);
					break;
				}

				parent->collDetectEnemyVessel(vessel);
			}
			//if (parent->RunVesselThreads() == false) break;
			barrier::SetSDLCondWaitBarrier(&parent->MCollisionBarrier, 0);
		}
	}
	else
	{
		while (parent->RunUnitThreads())
		{
			barrier::SetSDLCondWaitBarrier(&parent->MCollisionBarrier, 1);
			if (vessel->IsSpawned())
			{
				if (vessel->isDestroyed() || !vessel->getGraphics()->isActive())
				{
					barrier::SetSDLCondWaitBarrier(&parent->MCollisionBarrier, 0);
					break;
				}

				parent->collDetectAlliedVessel(vessel);
			}
			//if (parent->RunVesselThreads() == false) break;
			barrier::SetSDLCondWaitBarrier(&parent->MCollisionBarrier, 0);
		}
	}
	parent->DecrementThreadCollisionCounter();
	return 0;
}

int tProjectileThread(void* data)
{
	SDL_LockMutex(integrator::pProjectileThreadCreationLock);
	//particleEffects::ParticleEngine* pengine = (particleEffects::ParticleEngine*)data;
	integrator* parent = (integrator*)data;
	int particleSegments = MaxProjectiles / parent->GetMaxProjectileThreadsAmount();
	if (particleSegments < 0)
		particleSegments = 1;
	int threadNumber = parent->GetProjectileThreadsAmount() - 1;
	int lowerBound = threadNumber*particleSegments;
	int upperBound = lowerBound + particleSegments;
	SDL_UnlockMutex(integrator::pProjectileThreadCreationLock);

	if (lowerBound >= MaxProjectiles)
	{
		upperBound = 0;
		lowerBound = 0;
		//thread becomes useless
		//too many threads
	}

	while (parent->RunOtherThreads())
	{
		barrier::SetSDLCondWaitBarrier(&parent->MProjectileBarrier, 1);
		//parent->runProjectiles(lowerBound, upperBound);
		parent->runProjectiles(threadNumber);
		barrier::SetSDLCondWaitBarrier(&parent->MProjectileBarrier, 0);
		//barrier::SetSDLCondWaitBarrier(&parent->MCollisionPlayerBarrier, 1);
		//parent->collDetectPlayer(lowerBound, upperBound);
		//barrier::SetSDLCondWaitBarrier(&parent->MCollisionPlayerBarrier, 0);
	}

	parent->DecrementThreadProjectileHandler();
	return 0;
}

int tParticleThread( void* data )
{
	SDL_LockMutex(integrator::pParticleThreadCreationLock);
	particleEffects::ParticleEngine* pengine = (particleEffects::ParticleEngine*)data;
	integrator* parent = (integrator*)data;
	int particleSegments = particleEffects::MaxParticles / parent->GetMaxParticleThreadsAmount();
	if (particleSegments < 0)
		particleSegments = 1;
	int threadNumber = parent->GetParticleThreadsAmount() - 1;
	int lowerBound = threadNumber*particleSegments;
	int upperBound = lowerBound + particleSegments;
	SDL_UnlockMutex(integrator::pParticleThreadCreationLock);

	if (lowerBound >= particleEffects::MaxParticles)
	{
		upperBound = 0;
		lowerBound = 0;
		//thread becomes useless
		//too many threads
	}

	while(parent->RunOtherThreads())
	{
		barrier::SetSDLCondWaitBarrier(&parent->MParticleBarrier, 1);
		parent->pParticleEngine->Move(parent->GetCurrentTimeStep(true).Value,				   lowerBound,	upperBound);
		parent->pParticleEngine->ReduceTimeOnParticle(parent->GetCurrentTimeStep(true).Value, lowerBound,	upperBound);
		barrier::SetSDLCondWaitBarrier(&parent->MParticleBarrier, 0);
	}
	
	parent->DecrementThreadParticleHandler();
	return 0;
}

bool integrator::pThreadsInitialized = false;

integrator::integrator()
{
	pTimer = new timer();
	pSpeed = 1;
	//fixedIndependentStepTime = false;
	pAnimator = NULL;
	isPaused = false;
	pPauseLock = 0;

	Initialize();

}

integrator::integrator(bool stepTimeFixed)
{
	pTimer = new timer();
	pSpeed = 1;
	//fixedIndependentStepTime = stepTimeFixed;
	pAnimator = NULL;
	pPauseLock = 0;

	dynamicObjects = std::list<std::shared_ptr<DynamicEntity>>();

	//launcherObjects = std::list<std::shared_ptr<LauncherEntity>>();
	vesselAIObjects = std::list<std::shared_ptr<VesselAI>>() ; 
	vesselDebri = std::list<std::shared_ptr<Debri>>();

	DEPR_CollisionHandle = std::vector<CollisionHandle_DE_PR*>();
	DEDE_CollisionHandle = std::list<CollisionHandle_DE_DE*>();

	Initialize();
	
}

integrator::~integrator()
{
	//this->ResetObjects();	//cause error...
}

void integrator::Initialize()
{
	pParticleEngineThreads = 5;
	pProjectileThreads = 5;

	//pThreadsInitialized = false;
	tParticles = SDL_atomic_t{ 0 };
	tProjectiles = SDL_atomic_t{ 0 };
	MParticleBarrier = std::move(barrier::ConditionWaitBarrier(pParticleEngineThreads + 1, SDL_FALSE));
	MCollisionBarrier = std::move(barrier::ConditionWaitBarrier(1, SDL_FALSE));
	MUnitBarrier = std::move(barrier::ConditionWaitBarrier(1, SDL_FALSE));
	MProjectileBarrier = std::move(barrier::ConditionWaitBarrier(pProjectileThreads + 1, SDL_FALSE));
	MCollisionPlayerBarrier = std::move(barrier::ConditionWaitBarrier(pProjectileThreads + 1, SDL_FALSE));
	MVesselThreadCreationBarrier = std::move(barrier::ConditionWaitBarrier(3, SDL_FALSE)),
	pClearUnitThreads = SDL_atomic_t{ 1 };
	pClearParticleAndProjectileThreads = SDL_atomic_t{ 1 };
	tUnits = SDL_atomic_t{ 0 };
	tCollision = SDL_atomic_t{ 0 };
	pDisabled = false;
	currentStepTime = Millisecond(0);

	pDefaultProjectiles = ProjectileEngine();
	//pAntiMissileProjectiles = ProjectileEngine();
	//pThreadCreationLock = SDL_CreateMutex();
	this->InstantiateThreads();
}

void integrator::InstantiateThreads()
{
	if(pThreadsInitialized==false)
	{
		pThreadsInitialized = true;
		SDL_AtomicSet(&this->pClearUnitThreads, 0);
		SDL_AtomicSet(&this->pClearParticleAndProjectileThreads, 0);

		for (int i = 0; i < pParticleEngineThreads; i++)
		{
			string name = "tP_" + IntToString(i);
			SDL_LockMutex(pParticleThreadCreationLock);
			IncrementThreadParticleHandler();
			SDL_CreateThread(tParticleThread, name.c_str(), (void*)this);
			SDL_UnlockMutex(pParticleThreadCreationLock);
		}
		for (int i = 0; i < pProjectileThreads; i++)
		{
			string name = "tPrj_" + IntToString(i);
			SDL_LockMutex(pProjectileThreadCreationLock);
			IncrementThreadProjectileHandler();
			SDL_CreateThread(tProjectileThread, name.c_str(), (void*)this);
			SDL_UnlockMutex(pProjectileThreadCreationLock);
			
		}


	}
}

Coord integrator::PlayerPosition()
{
	if (this->CurrentPlayer().expired() == true)
		return Coord(0, 0);
	return this->CurrentPlayer().lock()->getPosition();
}

void integrator::pause()
{
	pPauseLock++;
	isPaused = true;
	currentStepTime = Millisecond(0);
}

void integrator::unpause()
{
	if( isPaused )
	{
		pPauseLock--;
		if(pPauseLock <= 0)
		{
			//pTimer->StartTick();
			isPaused = false;
		}
	}

}

//void integrator::unpause()
//{
//	if (isPaused)
//	{
//		pPauseLock--;
//		if (pPauseLock <= 0)
//		{
//			pTimer->refreshTimer();
//			isPaused = false;
//		}
//	}
//
//}

bool integrator::run(int stepTime)
{
	if( this->isPaused ) 
		return false;

	pTimer->StartTick();

	//double milliseconds = pLastStepTime.GetValue();
	//if (fixedIndependentStepTime || (milliseconds < minimalStepTime))
	//	milliseconds = minimalStepTime;

	double modifiedStepTime = stepTime* pSpeed;
	this->currentStepTime = Millisecond(modifiedStepTime);
		//t_particles = SDL_CreateThread( testthreadfunction, "tParticle", (void*)pParticleEngine.get() );

	pParticleEngine->RunEffects(modifiedStepTime, 0, 1000);

		
	TriggerParticleBarrier(1);
	TriggeProjectileBarrier(1);

	runDynamic(modifiedStepTime);
	runAI(modifiedStepTime);
	TriggerUnitBarrier(1);
	RunUnitEntity(modifiedStepTime);
	if(this->CurrentPlayer().expired() == false)
		RunPlayer(modifiedStepTime);
	RunOtherEntities(modifiedStepTime);
	runGunPlatform(modifiedStepTime);
	if (!pThreadsInitialized)
		runProjectiles(modifiedStepTime);
			//must be done before collision detect
		//sync
		//SDL_AtomicSet(&waitForMainLoop, 1);

	//barrier::SetSDLCondWaitBarrier(&parent->MProjectileBarrier, 1);
	//parent->runProjectiles(lowerBound, upperBound);
	//barrier::SetSDLCondWaitBarrier(&parent->MProjectileBarrier, 0);
	//barrier::SetSDLCondWaitBarrier(&parent->MCollisionBarrier, 1);
	//parent->collDetectPlayer(lowerBound, upperBound);
	//barrier::SetSDLCondWaitBarrier(&parent->MCollisionBarrier, 0);

	runAnimator(modifiedStepTime); //own thread
	//runMaskEffects(modifiedStepTime);
	runDebri(modifiedStepTime);	 //own thread

	if(!pThreadsInitialized)
		runParticleEffect(modifiedStepTime);

	ClearCollisionDetect();
	TriggeProjectileBarrier(0);
	TriggerUnitBarrier(0);
	this->pDefaultProjectiles.HandleProjectiles();
	TriggerCollisionBarrier(1);
	//TriggerPlayerCollisionBarrier(1);
	CollDetect();
	//TriggerPlayerCollisionBarrier(0);
	TriggerCollisionBarrier(0);


	collHandle(modifiedStepTime);
	if (this->CurrentPlayer().expired() == false)
		HandlePlayer(modifiedStepTime);

	TriggerParticleBarrier(0);
			
	pTimer->StopTick();
	//pLastStepTime = pTimer->GetStepTime();
	return true;

}
//
//std::weak_ptr<DynamicEntity> integrator::CurrentPlayer()
//{
//	if (this->pMode == enIntegratorMode::enSpaceCombat )
//	{
//		return this->currentPlayerVessel;
//	}
//	else if(this->pMode == enIntegratorMode::enInteriorCombat)
//	{
//		return this->currentPlayerWalker;
//	}
//	return std::weak_ptr<DynamicEntity>();
//}



//void integrator::setPlayerWalker(std::weak_ptr<PlayerWalker> set)
//{
//	currentPlayerWalker = set;
//}

//void integrator::LoadPlayerLaunchers()
//{
//	if (this->pMode == enIntegratorMode::enSpaceCombat && !currentPlayerVessel.expired())
//	{
//		for (auto it = this->currentPlayerVessel.lock()->LE_begin(); it != currentPlayerVessel.lock()->LE_end(); ++it)
//			this->addLauncher((*it));
//	}
//	else if (!currentPlayerWalker.expired())
//	{
//		for (auto it = this->currentPlayerWalker.lock()->LE_begin(); it != currentPlayerWalker.lock()->LE_end(); ++it)
//			this->addLauncher((*it));
//	}
//
//}

//void integrator::handlePlayer( Uint32 ms )
//{
//	if( IsSpaceMode() )
//		integrator::currentPlayerVessel.lock()->handleVesselState(ms);
//	else
//		integrator::currentPlayerWalker.lock()->handleWalkerState(ms);
//}

//void integrator::RemoveTargetSelectionOfVessels(std::shared_ptr<Vessel> IncludeThisVessel) const
//{
//	for (auto it = this->enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
//	{
//		if ((*it)->LinkAmount() == 0)
//			continue;
//
//		if ((*it) == IncludeThisVessel)
//		{
//			(*it)->getLink(0).get()->SetSelected(true);
//			this->getPlayerVessel().lock()->TargetVessel((*it));	//Hackish here
//		}
//		else
//			(*it)->getLink(0).get()->SetSelected(false);
//	}
//}
//
//void integrator::RemoveTargetSelectionOfVessels(Widget* ExcludeVesselOfLink) const
//{
//	for (auto it = this->enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
//	{
//		if ((*it)->LinkAmount() == 0)
//			continue;
//
//		if ((*it)->getLink(0).get() == ExcludeVesselOfLink)
//		{
//			(*it)->getLink(0).get()->SetSelected(true);
//			this->getPlayerVessel().lock()->TargetVessel((*it));	//Hackish here
//		}
//		else
//			(*it)->getLink(0).get()->SetSelected(false);
//	}
//}

//enumAllegiance integrator::GetUnitAllegiance(std::shared_ptr<UnitEntity> vessel)
//{
//	for (auto it = this->alliedVesselObjects.begin(); it != alliedVesselObjects.end(); ++it)
//		if ((*it) == vessel)
//			return enumAllegiance::aFRIENDLY;
//	return enumAllegiance::aENEMY;
//}

//void integrator::handlePlayerEvents(SDL_Event & e, const Coord& cameraPos)
//{
//	
//}

void integrator::HandleEvents( SDL_Event & e, const Coord& cameraPos)
{
	this->CurrentPlayer().lock().get()->handleEvent(e, cameraPos);
	for( auto it =  vesselDebri.begin(); it != vesselDebri.end(); ++it )
	{
		CurrentEventObject = it->get();
		(*it)->handleEvent(e,cameraPos);
	}
}
//
//void integrator::runVessel( double milliseconds )
//{
//	if (!pThreadsInitialized)
//	{
//		for (auto it = enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
//		{
//			if (computeUnitDynamic(*it, milliseconds, enumAllegiance::aENEMY) == false) //false == removed from vessel list
//				break;
//
//			//removed for testing
//			(*it)->ChangeLauncherTargets();
//			(*it)->incrementTimer(milliseconds);
//
//		}
//
//		for (auto it = alliedVesselObjects.begin(); it != alliedVesselObjects.end(); ++it)
//		{
//			if (computeUnitDynamic(*it, milliseconds, enumAllegiance::aFRIENDLY) == false) //false == removed from vessel list
//			{
//				//if (vesselObjects.size() == 0)
//				break;
//			}
//			(*it)->ChangeLauncherTargets();
//			//else
//			//	++it;
//		}
//	}
//	if (this->IsSpaceMode() == false)
//	{
//		for (auto it = walkingObjects.begin(); it != walkingObjects.end(); ++it)
//		{
//			if (computeWalkerDynamic(*it, milliseconds) == false) //false == removed from vessel list
//			{
//				//if (vesselObjects.size() == 0)
//				break;
//			}
//		}
//	}
//
//	runPlayer( milliseconds );
//}

//void integrator::runPlayer( double milliseconds )
//{
//	if (this->IsSpaceMode())
//	{
//		computeUnitDynamic(this->getPlayerVessel().lock(), milliseconds, enumAllegiance::aFRIENDLY);
//		this->getPlayerVessel().lock()->RunRadarTargetTracking(milliseconds);
//	}
//	else
//	{
//		computeUnitDynamic(this->getPlayerWalker().lock(), milliseconds, enumAllegiance::aFRIENDLY);
//	}
//	this->getPlayerVessel().lock()->incrementTimer(milliseconds);
//}

void integrator::runGunPlatform( double milliseconds )
{
	for( auto it =  gunPlatforms.begin(); it != gunPlatforms.end(); it++ )
	{
		ComputeWPlatformDynamic( it->get(), milliseconds );
	}
}

//void integrator::runInteriorModel(double milliseconds)
//{
//	if (this->IsInteriorMode())
//	{
//		this->interiorObject->AnimateDecoModules(milliseconds);
//		this->interiorObject->HandleDoorModules( this->getPlayerWalker().lock()->getPosition(), milliseconds);	//fix position relation here
//	}
//}

void integrator::ComputeWPlatformDynamic( GunPlatform* platform, double milliseconds )
{
	//platform->UpdateAim();
	platform->updateVelocity( milliseconds );
	platform->move(milliseconds);
	platform->UpdateBehavior();
	if( platform->getGraphics()->isTimed() )
		platform->getGraphics()->reduceTimer(milliseconds);

	//for( auto lt = platform->LE_begin(); lt != platform->LE_end(); ++lt )
	//{
	//	for( auto pt = (*lt)->pbegin(); pt != (*lt)->pend(); pt++ )
	//	{
 //   		(*pt)->updateVelocity( milliseconds );
	//		(*pt)->move( milliseconds ); //run this in threads?
	//	}
	//}

	refreshEntity( platform );
}

bool integrator::ComputeUnitDynamic(std::shared_ptr<UnitEntity> unit, double milliseconds, enumAllegiance allegiance)
{
	MoveIterateUnitEntity(unit, milliseconds);
	if(HandleUnitEntityTimer(unit,milliseconds))
	{
		this->RemoveUnit(unit);
		return false;
	}

	CompuiteUnitEntity(unit, milliseconds, allegiance);
	return true;
}

void integrator::CompuiteUnitEntity(std::shared_ptr<UnitEntity> entity, int milliseconds, enumAllegiance allegiance)
{
	std::vector<std::shared_ptr<Projectile>> newProjectiles = entity->RunLaunchers(milliseconds);
	for (int i = 0; i < newProjectiles.size(); i++)
	{
		newProjectiles[i]->SetAllegiance(allegiance);
		this->pDefaultProjectiles.MoveToNextActiveSetProjectile(newProjectiles[i], true);
	}
	entity->RunDetached(milliseconds);
	entity->ProcessDetachedModules(milliseconds);
	entity->RunModuleAnimations(milliseconds);

	refreshEntity(entity.get());
}

bool integrator::HandleUnitEntityTimer(std::shared_ptr<UnitEntity> entity, int milliseconds)
{
	if (entity->getGraphics()->isTimed())
	{
		if (!entity->getGraphics()->reduceTimer(milliseconds) && !entity->HasDetachModulesToProcess())
			return true;

		entity->getGraphics()->HandleDestroyAnimation(entity->CenterPosition());
	}
	else if (entity->isDestroyed())
		return true;
	return false;
}

void integrator::MoveIterateUnitEntity(std::shared_ptr<UnitEntity> entity, int ms)
{

	entity->move(ms);
	entity->updateVelocity(ms);
}

void integrator::runDynamic( double milliseconds )
{
	for( auto it =  dynamicObjects.begin(); it != dynamicObjects.end(); it++ )
	{
		(*it)->updateVelocity( milliseconds );
		(*it)->move(milliseconds);

		if( (*it)->getGraphics()->isTimed() )
			(*it)->getGraphics()->reduceTimer(milliseconds);

		refreshEntity( it->get() );
	}
}

//void integrator::runLauncher( std::shared_ptr<LauncherEntity> launcher, double milliseconds )
//{
//	if (launcher->IsAutoFire())
//	{
//		launcher->UpdateAim(milliseconds);
//	}
//
//
//}

void integrator::runProjectiles(int lowBound, int upperBound)
{
	if (upperBound > this->pDefaultProjectiles.ProjectileCount())
		upperBound = this->pDefaultProjectiles.ProjectileCount();
	
	for (int i = lowBound; i < upperBound; i++)
	{
		std::shared_ptr<Projectile> pt = std::move((*pDefaultProjectiles.CurrentActiveSet())[i]);
		bool destroy = ReduceTimeOnProjectile(pt, this->currentStepTime.Value);
		if (destroy == false)
		{
			pt->updateVelocity(this->currentStepTime.Value);
			pt->move(this->currentStepTime.Value); //run this in threads?
			if (pt->HasParticleEffect())
				pt->TriggerParticleEffect();
			pDefaultProjectiles.MoveToNextActiveSetProjectile(pt, false);
		}
		else
			pDefaultProjectiles.MoveToDeleteProjectile(pt);
	}
}

void integrator::runProjectiles(int modulo)
{
	int upperBound = this->pDefaultProjectiles.ProjectileCount();

	for (int i = modulo; i < upperBound; i += this->pProjectileThreads)
	{
		std::shared_ptr<Projectile> pt = std::move((*pDefaultProjectiles.CurrentActiveSet())[i]);
		bool destroy = ReduceTimeOnProjectile(pt, this->currentStepTime.Value);
		if (destroy == false)
		{
			pt->updateVelocity(this->currentStepTime.Value);
			pt->move(this->currentStepTime.Value); //run this in threads?
			if (pt->HasParticleEffect())
				pt->TriggerParticleEffect();
			pDefaultProjectiles.MoveToNextActiveSetProjectile(pt, false);
		}
		else
			pDefaultProjectiles.MoveToDeleteProjectile(pt);
	}
}

void integrator::runProjectiles(double ml)
{
	runProjectiles(0, this->pDefaultProjectiles.ProjectileCount());
}

bool integrator::ReduceTimeOnProjectile(std::shared_ptr<Projectile>& projectile, double millisec)
{
	bool active = projectile->getGraphics()->reduceTimer( millisec );
	if( active == false || projectile->isDestroyed() )
		return true;
	return false;
}




//void integrator::runLauncher( double milliseconds )
//{
//	for( auto it =  launcherObjects.begin(); it != launcherObjects.end(); it++ )
//	{
//		for( auto itt = it->second.begin(); itt != it->second.end(); ++itt )
//		{
//			runLauncher(*itt,milliseconds);
//		}
//	}
//}

void integrator::runAI( double milliseconds )
{
	for( auto it = vesselAIObjects.begin(); it != vesselAIObjects.end();  )
	{
		if( runAI(*it, milliseconds) == false )
			it = vesselAIObjects.erase(it);
		else
			++it;
	}
}

bool integrator::runAI(std::shared_ptr<VesselAI> AI, double milliseconds)
{
	AI->RunAI(milliseconds);
	if (!AI->isAlive())
	{
		AI->disableAIAndVessel();
		return false;
	}
	else
		return true;
}

//std::shared_ptr<UnitEntity> integrator::GetUnitEntityFromID( int ID )
//{
//	for( auto it = this->enemyVesselObjects.begin(); it != this->enemyVesselObjects.end(); ++it )
//	{
//		if( (*it)->GetID() == ID )
//			return *it;
//	}
//	for (auto it = this->alliedVesselObjects.begin(); it != this->alliedVesselObjects.end(); ++it)
//	{
//		if ((*it)->GetID() == ID)
//			return *it;
//	}
//	return nullptr;
//}

//std::list<std::shared_ptr<UnitEntity>> integrator::GetVessels(bool enemy)
//{
//	if (enemy)
//		return this->enemyVesselObjects;
//	else
//		return this->alliedVesselObjects;
//}

void integrator::runAnimator( double milliseconds)
{
	if(pAnimator!=NULL)
	{
		pAnimator->RunFrames(milliseconds);
	}
}

//void integrator::runMaskEffects(double milliseconds)
//{
//	//if (pMaskEffects != nullptr)
//	//	pMaskEffects->Run(milliseconds);
//}

void integrator::runDebri( double milliseconds )
{
	for( auto it =  vesselDebri.begin(); it != vesselDebri.end();  )
	{
		(*it)->updateVelocity( milliseconds );
		(*it)->move(milliseconds);

		(*it)->TriggerParticleEffect();

		if( (*it)->getGraphics()->isTimed() )
			(*it)->getGraphics()->reduceTimer(milliseconds);

		if( (*it)->getGraphics()->isActive() == false )
		{
			//Debri* tempPointer = it->get();
			it = this->vesselDebri.erase(it);
			//delete tempPointer;
		}
		else
			++it;
	}
}

void integrator::runParticleEffect( double milliseconds )
{
	//not in use?? look at thread instead
	//pParticleEngine->runEffects(milliseconds,0, particleEffects::ParticleEngine::MaxParticles());
	pParticleEngine->Move(milliseconds, 0, particleEffects::MaxParticles);
	pParticleEngine->ReduceTimeOnParticle( milliseconds, 0, particleEffects::MaxParticles);
}

void integrator::refreshEntity( DynamicEntity* entity)
{
	if( entity->reqRefresh )
	{
		entity->getGraphics()->getGraphicalObject()->refreshTexture();
		entity->reqRefresh = false;
	}
}

bool integrator::collDetectEnemyVessel(std::shared_ptr<UnitEntity> enemyObject)
{
	if (enemyObject->isDestroyed())
		return false;

	for (int i = 0; i < pDefaultProjectiles.ProjectileCount(); i++)
	{
		auto pe = (*pDefaultProjectiles.CurrentActiveSet())[i];
		if (pe->GetAllegiance() == enumAllegiance::aENEMY)
			continue;

		if (pe->ProjectileHit())
		{
			if (CollisionDetect::projectileWithinBoundary(*enemyObject, *pe))
			{
				CollisionDetect::colData collisionResult = std::move(CollisionDetect::dynamicEntityHullDetect(*enemyObject, *pe));
				if (collisionResult.isValidCollision)
					AddCollisionData((new CollisionHandle_DE_PR(enemyObject, pe, collisionResult))); // add position and velocity, get this from hull detect (which return these values in a struct ?? )
			}
		}
	}
	return true;
}

bool integrator::collDetectAlliedVessel(std::shared_ptr<UnitEntity> allyObject)
{
	if (allyObject->isDestroyed())
		return false;

	for (int i = 0; i < pDefaultProjectiles.ProjectileCount(); i++)
	{
		auto pe = (*pDefaultProjectiles.CurrentActiveSet())[i];
		if (pe->GetAllegiance() == enumAllegiance::aFRIENDLY)
			continue;

		if (pe->ProjectileHit())
		{
			if (CollisionDetect::projectileWithinBoundary(*allyObject, *pe))
			{
				CollisionDetect::colData collisionResult = std::move(CollisionDetect::dynamicEntityHullDetect(*allyObject, *pe));
				if (collisionResult.isValidCollision)
					AddCollisionData(new CollisionHandle_DE_PR(allyObject, pe, collisionResult)); // add position and velocity, get this from hull detect (which return these values in a struct ?? )
			}
		}
	}
	return true;
}

void integrator::AddCollisionData(CollisionHandle_DE_PR* add)
{
	SDL_LockMutex(integrator::pAddCollisionData);
	DEPR_CollisionHandle.push_back(add);
	SDL_UnlockMutex(integrator::pAddCollisionData); // pAddCollisionData
}

bool integrator::collDetectPlayer()
{
	return collDetectPlayer(0, pDefaultProjectiles.ProjectileCount());
}

bool integrator::collDetectPlayer(int lowBound, int higherBound)
{
	if (higherBound > this->pDefaultProjectiles.ProjectileCount())
		higherBound = this->pDefaultProjectiles.ProjectileCount();

	bool result = false;
	auto player = this->CurrentPlayer().lock();

	if (player.get() == false)
		return result;

	for (int i = lowBound; i < higherBound; i++)
	{
		std::shared_ptr<Projectile>& pe = (*pDefaultProjectiles.CurrentActiveSet())[i];
			if (pe->IsPlayeAlly() || !pe->CanTargetVessel())
				continue;
		
		if (CollisionDetect::projectileWithinBoundary(*player, *pe))
		{
			CollisionDetect::colData collisionResult = std::move(CollisionDetect::dynamicEntityHullDetect(*player, *pe));
			if (collisionResult.isValidCollision)
			{
				AddCollisionData(new CollisionHandle_DE_PR(player, pe, collisionResult)); // add position and velocity, get this from hull detect (which return these values in a struct ?? )
				result = true;
			}
		}
	}
	return result;
}

void integrator::CollDetectAntiMissile()
{
	//temporarily disabeld
	//for( auto pt = pDefaultProjectiles.CurrentTorpedoSet()->begin(); pt != pDefaultProjectiles.CurrentTorpedoSet()->end(); ++pt )
	//{
	//	CollDetectAntiMissile( *pt );
	//}
}

void integrator::CollDetectAntiMissile(std::shared_ptr<Projectile> preyprojectile)
{
	for( int i = 0; i < pDefaultProjectiles.ProjectileCount(); i++)
	{
		auto pe = (*pDefaultProjectiles.CurrentAMissileSet())[i];
		if (pe->GetAllegiance() == preyprojectile->GetAllegiance())
			continue;

		if (pe->GetTarget().lock() == preyprojectile)
		{
			CollisionDetect::colData collisionResult = CollisionDetect::ProjectileProjectileCollisionDetect(*preyprojectile, *pe);
			if (collisionResult.isValidCollision)
				PRPR_CollisionHandle.push_back(new CollisionHandle_PR_PR(preyprojectile, pe, collisionResult.targetHitpos, collisionResult.hunterHitPos, collisionResult.vel_A, collisionResult.vel_B));
		}


		// CODE BELOW SHOULD HAPPEN AT A DIFFERENT PART OF THE ITERATION
		//else
		//{
		//	if (geometry::PointWithinRange((*pe)->getPosition(), preyprojectile->getPosition(), (*le)->GetTargetScanRange()))
		//	{
		//		(*le)->SetAimTarget(preyprojectile);
		//	}
		//}

	}
}
	
 //  	for( auto le =  launcherObjects[lAntiMissile].begin(); le != launcherObjects[lAntiMissile].end(); le++ )
	//{
	//	if( (*le)->IsPlayeAlly() == projectilePlayerally)
	//		continue;

	//	if( (*le)->HasTarget() )
	//	{
	//		if( (*le)->GetAimTarget().lock() == preyprojectile )
	//		{
	//			//run collision detect
	//			for( auto pt = (*le)->pbegin(); pt != (*le)->pend(); ++pt )
	//			{
	//				//if ((*pt)->isDestroyed())
	//				//	pt = this->launchedProjects.erase(it);

	//				CollisionDetect::colData collisionResult = CollisionDetect::ProjectileProjectileCollisionDetect( *preyprojectile, **pt );
	//				if( collisionResult.isValidCollision )
	//					PRPR_CollisionHandle.push_back( new CollisionHandle_PR_PR( preyprojectile, *pt, collisionResult.targetHitpos, collisionResult.hunterHitPos, collisionResult.vel_A, collisionResult.vel_B ));

	//			}
	//		}
	//	}
	//	else
	//	{
	//		if( geometry::PointWithinRange( (*le)->getPosition(), preyprojectile->getPosition(), (*le)->GetTargetScanRange() ) )
	//		{
 // 				(*le)->SetAimTarget( preyprojectile );
	//		}
	//	}
	//}

void integrator::ClearCollisionDetect()
{
	for( auto it = DEPR_CollisionHandle.rbegin(); it != DEPR_CollisionHandle.rend(); ++it ) //list already contains all vessels when map loads??
	{
		delete *it;
	}
	DEPR_CollisionHandle.clear();
	for( auto it = DEDE_CollisionHandle.begin(); it != DEDE_CollisionHandle.end(); ++it )
	{
		delete *it;
	}
	DEDE_CollisionHandle.clear();

	for( auto it = PRPR_CollisionHandle.begin(); it != PRPR_CollisionHandle.end(); ++it )
	{
		delete *it;
	}
	PRPR_CollisionHandle.clear();
}

void integrator::RemoveLaunchersOfEntity(std::shared_ptr<LauncherProvider> vessel)
{
	for (auto lt = vessel->LE_begin(); lt != vessel->LE_end(); ++lt)
	{
		this->RemoveLauncher(*lt);
	}
}

void integrator::collHandle( int ms)
{
	if( DEPR_CollisionHandle.size() > 0 ) 
	{
		//std::list<std::shared_ptr<Debri>> tempDebri = std::list<std::shared_ptr<Debri>>();
		//SDL_SemWait(particleEffects::sem);
		this->cHandler.Handle( DEPR_CollisionHandle, *pParticleEngine, *pAnimator );
		//SDL_SemPost(particleEffects::sem);
		//for( auto it = tempDebri.begin(); it != tempDebri.end(); ++it )
		//{
		//	vesselDebri.push_back( *it );
		//}
	}

	if( PRPR_CollisionHandle.size() > 0 ) 
	{
		//std::list<std::shared_ptr<Debri>> tempDebri = std::list<std::shared_ptr<Debri>>();
		//SDL_SemWait(particleEffects::sem);
		this->cHandler.Handle( PRPR_CollisionHandle, *pParticleEngine, *pAnimator );
		//SDL_SemPost(particleEffects::sem);
		//for( auto it = tempDebri.begin(); it != tempDebri.end(); ++it )
		//{
		//	vesselDebri.push_back( *it );
		//}
	}

	if (DEDE_CollisionHandle.size() > 0)
	{
		this->cHandler.Handle(DEDE_CollisionHandle, ms);
	}
}

void integrator::addLauncher( std::shared_ptr<LauncherEntity> object )
{  
	if(AntiProjectileMissileLauncher* derived = dynamic_cast<AntiProjectileMissileLauncher*>(object.get()))
    {
		launcherObjects[lAntiMissile].push_back(object); 
    }
	else if(MissileLauncherEntity* derived = dynamic_cast<MissileLauncherEntity*>(object.get()))
	{
		launcherObjects[lMissile].push_back(object); 
	}
	else if(TorpedoLauncherEntity* derived = dynamic_cast<TorpedoLauncherEntity*>(object.get()))
	{
		launcherObjects[lTorpedo].push_back(object); 
	}
	else if(RailgunLauncher* derived = dynamic_cast<RailgunLauncher*>(object.get()))
	{
		launcherObjects[lRail].push_back(object); 
	}
	else
	{
		launcherObjects[lcannon].push_back(object); 
	}
	
};

void integrator::RemoveLauncher(std::shared_ptr<LauncherEntity> object)
{
	if (AntiProjectileMissileLauncher* derived = dynamic_cast<AntiProjectileMissileLauncher*>(object.get()))
	{
		RemoveLauncher(lAntiMissile, object);
	}
	else if (MissileLauncherEntity* derived = dynamic_cast<MissileLauncherEntity*>(object.get()))
	{
		RemoveLauncher(lMissile, object);
	}
	else if (TorpedoLauncherEntity* derived = dynamic_cast<TorpedoLauncherEntity*>(object.get()))
	{
		RemoveLauncher(lTorpedo, object);
	}
	else if (RailgunLauncher* derived = dynamic_cast<RailgunLauncher*>(object.get()))
	{
		RemoveLauncher(lRail, object);
	}
	else
	{
		RemoveLauncher(lcannon, object);
	}
}

void integrator::RemoveLauncher(string key, std::shared_ptr<LauncherEntity> object)
{
	SDL_LockMutex(integrator::pRemoveLaunchersLock);
	for (auto it = launcherObjects[key].begin(); it != launcherObjects[key].end(); ++it)
	{
		if (*it == object)
		{
			it->reset();
			launcherObjects[key].erase(it);
			SDL_UnlockMutex(integrator::pRemoveLaunchersLock);
			return;
		}
	}
	SDL_UnlockMutex(integrator::pRemoveLaunchersLock);
}

void integrator::DisableOtherThreads() 
{
	SDL_AtomicSet(&this->pClearParticleAndProjectileThreads, 1);
	this->MParticleBarrier.Break();
	this->MProjectileBarrier.Break();
	this->MCollisionPlayerBarrier.Break();
	while (SDL_AtomicGet(&this->tProjectiles) != 0)
	{
		//wait for all projectile handles to finish
	}
	while (SDL_AtomicGet(&this->tParticles) != 0)
	{
		//wait for all particle handles to finish
	}
	
}

void integrator::ResetObjects()
{
	this->pause();
	this->dynamicObjects.clear();
	this->ClearLauncherObjects();
	this->gunPlatforms.clear();

	this->ClearUnitEntities();
	this->DisableOtherThreads();
	pThreadsInitialized = false;
	pDisabled = true;
	
	this->vesselAIObjects.clear();
	this->vesselDebri.clear();
	this->interiorObject.reset();
	this->pDefaultProjectiles.ClearAll(false);
	/*this->pParticleEngine->ResetAllParticles();*/

	this->DEPR_CollisionHandle.clear();
	this->DEDE_CollisionHandle.clear();

	this->pParticleEngine->clearInactiveEffects(); //redundant
	this->pParticleEngine->clearInactiveParticles();
	this->pParticleEngine->ResetAllParticles();

	LoadPlayerLaunchers();
	this->unpause();
}

void integrator::ResetVesselThreads()
{
	MCollisionBarrier.SetBreak(SDL_FALSE);
	MCollisionBarrier.SetThreadAmount(1);
	MCollisionBarrier.ArriveCounter = 0;
	MCollisionBarrier.LeaveCounter = 1;
	MCollisionBarrier.Flag = 0;
	MUnitBarrier.SetBreak(SDL_FALSE);
	MUnitBarrier.SetThreadAmount(1);
	MUnitBarrier.ArriveCounter = 0;
	MUnitBarrier.LeaveCounter = 1;
	MUnitBarrier.Flag = 0;
}

void integrator::ClearLauncherObjects()
{

	//if( this->currentPlayer != nullptr )
	//{

	//	for( auto it = this->launcherObjects.begin(); it != this->launcherObjects.end(); ++it )
	//	{
	//		if( this->currentPlayer->wplatforms. )

	//	}
	//}
	//else
	for( auto it = this->launcherObjects.begin(); it != this->launcherObjects.end(); ++ it )
	{
		(*it).second.clear();
	}
	this->launcherObjects.clear();
}

void integrator::ClearUnitEntities()
{
	this->ClearUnitThreads();
	this->MUnitBarrier.Break();
	this->MCollisionBarrier.Break();
	while (SDL_AtomicGet(&this->tUnits) != 0)
	{
		//wait for all vessel threads to finish
	}
	while (SDL_AtomicGet(&this->tCollision) != 0)
	{
		//wait for all collision threads to finish
	}
	ClearVessels();
	ClearWalkers();
	ResetVesselThreads();

}

void integrator::ClearVessels()
{

}
void integrator::ClearWalkers()
{

}

void integrator::setSpeed( Uint32 set )
{
	if( set > MAXSPEED )
		set = MAXSPEED;
	else if( set < MINSPEED )
		set = MINSPEED;
	else 
		pSpeed = set;

}

void integrator::DecrementThreadProjectileHandler()
{
	if (!pThreadsInitialized)
		return;
	SDL_AtomicDecRef(&this->tProjectiles);
}
void integrator::IncrementThreadProjectileHandler()
{
	if (!pThreadsInitialized)
		return;
	SDL_AtomicIncRef(&this->tProjectiles);
}

void integrator::DecrementThreadParticleHandler()
{
	if (!pThreadsInitialized)
		return;
	SDL_AtomicDecRef(&this->tParticles);
}
void integrator::IncrementThreadParticleHandler()
{
	if (!pThreadsInitialized)
		return;
	SDL_AtomicIncRef(&this->tParticles);
}

void integrator::DecrementThreadCollisionCounter()
{
	if (!pThreadsInitialized)
		return;
	this->MCollisionBarrier.DecThreadAmount();
	SDL_AtomicDecRef(&this->tCollision);
}
void integrator::IncrementThreadCollisionCounter()
{
	if (!pThreadsInitialized)
		return;
	this->MCollisionBarrier.IncThreadAmount();
	SDL_AtomicIncRef(&this->tCollision);
}

void integrator::DecrementThreadUnitCounter()
{
	if (!pThreadsInitialized)
		return;
	this->MUnitBarrier.DecThreadAmount();
	SDL_AtomicDecRef(&this->tUnits);
}
void integrator::IncrementThreadUnitCounter() 
{ 
	if (!pThreadsInitialized)
		return;
	this->MUnitBarrier.IncThreadAmount();
	SDL_AtomicIncRef(&this->tUnits);
}

void integrator::TriggerParticleBarrier(int color)
{
	if(pThreadsInitialized)
		barrier::SetSDLCondWaitBarrier(&MParticleBarrier, color > 0 ? 1 : 0);

}

void integrator::TriggerCollisionBarrier(int color)
{
	if (pThreadsInitialized)
		barrier::SetSDLCondWaitBarrier(&MCollisionBarrier, color > 0 ? 1 : 0);
}

void integrator::TriggerUnitBarrier(int color)
{
	if (pThreadsInitialized)
		barrier::SetSDLCondWaitBarrier(&MUnitBarrier, color > 0 ? 1 : 0);
}

void integrator::TriggeProjectileBarrier(int color)
{
	if (pThreadsInitialized)
		barrier::SetSDLCondWaitBarrier(&MProjectileBarrier, color > 0 ? 1 : 0);
}

void integrator::TriggerPlayerCollisionBarrier(int color)
{
	if (pThreadsInitialized)
		barrier::SetSDLCondWaitBarrier(&MCollisionPlayerBarrier, color > 0 ? 1 : 0);
}

Millisecond integrator::GetCurrentTimeStep(bool includeSpeed)
{
	if (includeSpeed)
		return Millisecond(this->currentStepTime.Value*pSpeed);
	return this->currentStepTime;
}


//integrator::~integrator()
//{
//	delete pTimer;
//	DEPR_CollisionHandle.clear();
//	DEDE_CollisionHandle.clear();
//
//}

ProjectileEngine::ProjectileEngine()
{
	pProjectiles = SDL_atomic_t{ 0 };
	pNewProjectileCounter = SDL_atomic_t{ 0 };
	pDeleteProjectiles = SDL_atomic_t{ 0 };
	std::vector<std::shared_ptr<Projectile>> pActiveSet1 = std::vector<std::shared_ptr<Projectile>>();
	std::vector<std::shared_ptr<Projectile>> pActiveSet2 = std::vector<std::shared_ptr<Projectile>>();
	std::vector<std::shared_ptr<Projectile>> pToDelete = std::vector<std::shared_ptr<Projectile>>();

	std::vector<std::shared_ptr<Projectile>>* pCurrentActiveSet = &pActiveSet1;
	std::vector<std::shared_ptr<Projectile>>* pNextActiveSet = &pActiveSet2;

	Set1IsActive = true;
}

void  ProjectileEngine::HandleProjectiles()
{
	if (Set1IsActive)
	{
		pCurrentActiveSet = &pActiveSet2;
		pNextActiveSet = &pActiveSet1;
	}
	else
	{
		pCurrentActiveSet = &pActiveSet1;
		pNextActiveSet = &pActiveSet2;
	}

	//if ((*pCurrentActiveSet)[0].use_count() > 1 && (!(*pCurrentActiveSet)[0].get() || (*pCurrentActiveSet)[0] == nullptr))
	//	int feiltest = 2;

	Set1IsActive = !Set1IsActive;
	SDL_AtomicSet(&pProjectiles, pNewProjectileCounter.value);
	SDL_AtomicSet(&pNewProjectileCounter, 0);


	for (int i = 0; i < pDeleteProjectiles.value; i++)
		pToDelete[i].reset();
	SDL_AtomicSet(&pDeleteProjectiles, 0);
	//SDL_AtomicSet(this->pProjectiles,
}

void ProjectileEngine::ClearAll(bool clearVectors)
{
	for (int i = 0; i < pActiveSet1.size(); i++)
		pActiveSet1[i].reset();

	for (int i = 0; i < pActiveSet2.size(); i++)
		pActiveSet2[i].reset();

	for (int i = 0; i < pToDelete.size(); i++)
		pToDelete[i].reset();

	SDL_AtomicSet(&pNewProjectileCounter, 0);
	SDL_AtomicSet(&pProjectiles, 0);
	SDL_AtomicSet(&pDeleteProjectiles, 0);

	if(clearVectors == false)
	{
		if (Set1IsActive)
		{
			pCurrentActiveSet = &pActiveSet2;
			pNextActiveSet = &pActiveSet1;
			pGenericSet = pCurrentActiveSet;
		}
		else
		{
			pCurrentActiveSet = &pActiveSet1;
			pNextActiveSet = &pActiveSet2;
			pGenericSet = pCurrentActiveSet;
		}
	}
}

//void  ProjectileEngine::AddNewProjectile(std::shared_ptr<Projectile>& pro)
//{
//	if (isNewProjectile)
//		this->pProjectiles++;
//	(*pCurrentActiveSet).push_back(std::move(pro));
//}
void ProjectileEngine::MoveToDeleteProjectile(std::shared_ptr<Projectile>& pro)
{
	//SDL_AtomicDecRef(&this->pProjectiles);
	pToDelete[SDL_AtomicIncRef(&this->pDeleteProjectiles)] = std::move(pro);
}
void ProjectileEngine::MoveToNextActiveSetProjectile(std::shared_ptr<Projectile>& pro, bool isNewProjectile)
{
	(*pNextActiveSet)[SDL_AtomicIncRef(&this->pNewProjectileCounter)] = std::move(pro);
	//if (isNewProjectile)
	//	SDL_AtomicIncRef(&this->pProjectiles);
}