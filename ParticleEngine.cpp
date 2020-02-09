#include"ParticleEngine.h"
#include"ParticleRandomSpread.h"
#include"ParticleRandomDirectedSpread.h"
#include"ParticleTrailEffect.h"

namespace particleEffects
{

	const int MaxParticles = 1000;
	std::array<std::shared_ptr<Particle>, MaxParticles> ParticleEngine::pBaseParticles = std::array<std::shared_ptr<Particle>, MaxParticles>();
	SDL_atomic_t ParticleEngine::pBaseParticleAdded = SDL_atomic_t{ MaxParticles };

	ParticleEngine::ParticleEngine()
	{
		loadParticleSurfaces();
		generateEffect = false;
		pAddParticleLock = SDL_atomic_t{ 0 };
		pSet1Active = true;
		pCurrentActiveParticleSet = &pActiveParticlesSet1;
		pCurrentStorageParticleSet = &pActiveParticlesSet2;
	}

	void ParticleEngine::loadParticleSurfaces()
	{
		for (int i = 0; i < MaxParticles; i++)
			pBaseParticles[i] = std::shared_ptr<Particle>(new Particle(ColorRGB(255, 255, 255)));
	}

	void ParticleEngine::Move(Uint32 ms, int lowerBound, int upperBound)
	{
		//SDL_SemWait(sem);
		if (upperBound >= this->particleCount())
			upperBound = this->particleCount();
		for (int i = lowerBound; i < upperBound; i++)
			(*pCurrentActiveParticleSet)[i]->move(ms);
		//SDL_SemPost(sem);
	}


	void ParticleEngine::ReduceTimeOnParticle(double millisec, int lowerBound, int upperBound)
	{
		if (upperBound >= this->particleCount())
			upperBound = this->particleCount();
		for (int i = lowerBound; i < upperBound; i++)
		{
			bool active = (*pCurrentActiveParticleSet)[i]->getGraphics()->reduceTimer(millisec);
			if (active)
				this->AddNextInLineActive((*pCurrentActiveParticleSet)[i]);
			else
				this->AddParticleToBase((*pCurrentActiveParticleSet)[i]);
		}
	}

	void ParticleEngine::ResetAllParticles()
	{
		for (int i = 0; i < this->particleCount(); i++)
		{
			(*pCurrentActiveParticleSet)[i]->getGraphics()->disable();
			this->AddParticleToBase((*pCurrentActiveParticleSet)[i]);
		}
		particleEffectList.clear();

		pCurrentActiveParticles = 0;
		//revert to initial values
		pCurrentActiveParticleSet = &pActiveParticlesSet1;
		pCurrentStorageParticleSet = &pActiveParticlesSet2;
	}

	void ParticleEngine::RunEffects(double ms, int lowerBound, int upperBound)
	{
		if (!generateEffect)
		{
			//SDL_SemPost(sem);
			return;
		}
		//if (lowerBound >= this->particleEffectList.size())
		//	return;

		if (upperBound >= this->particleEffectList.size())
			upperBound = this->particleEffectList.size();

		for (int i = lowerBound; i < upperBound; i++)
		{
			auto particles = particleEffectList[i]->RunEffect(ms / 1000);
			//if (particleEffectList[i]->GetRunTime() <= currentParticle.GetSLaunchTime())
			//	continue;

			std::for_each(particles.begin(), particles.end(), [&](ParticleTemplate & currentTemplate)
				{
					auto newParticle = ParticleEngine::particleFromColor(currentTemplate.GetColor());
					if (newParticle.get())
					{
						//if (particleEffectList[i]->HasSourcePosition())
						//	newParticle->SetPosition(particleEffectList[i]->sourcePosition()); // + currentParticle->getPosition()
						//else
						newParticle->SetPosition(currentTemplate.GetPosition());
						newParticle->SetLaunchTime(currentTemplate.GetSLaunchTime());
						newParticle->setTimeLeft(currentTemplate.GetMsTimeLeft());
						newParticle->setVelocity(currentTemplate.GetVelocity());
						newParticle->SetParticleColor(currentTemplate.GetColor());
						newParticle->SetUpdateFunction(currentTemplate.GetUpdateFunction());
					this->AddNextInLineActive(newParticle);
				}
			});
			//AddParticle(newParticle);
			//SDL_SemPost(ParticleBaseSem);
		}

	}

	//void ParticleEngine::AddParticleEffect(std::shared_ptr<ParticleEffect> effect)
	//{
	//	SDL_LockMutex(AddParticleEffectSem);
	//	this->pParticleEffectPendingList.push_back(effect);
	//	SDL_UnlockMutex(AddParticleEffectSem);
	//}

	void ParticleEngine::AddNewEffect(ParticleEffect* effect)
	{
		SDL_LockMutex(AddParticleEffectSem);
		this->pParticleEffectPendingList.push_back(std::unique_ptr<ParticleEffect>(effect));
		generateEffect = true;
		SDL_UnlockMutex(AddParticleEffectSem);

		
		//return effectPtr;
	}

	ParticleEffect* ParticleEngine::GetEffectDustSpread(const Coord& pos, double speed, int particles, int particleLaunchInterval, int avgTime)
	{
		return new ParticleRandomSpread(pos, speed, particleEffects::GreyScaleParticles(), particles, particleLaunchInterval, avgTime);
	}


	ParticleEffect* ParticleEngine::GetEffectParticleTrail(const Coord& pos)
	{
		return new ParticleTrailEffect(pos, vector(-0.6,0.2), particleEffects::GreyScaleParticles(), 1500,1);
	}

	ParticleEffect* ParticleEngine::GetEffectRandomDirectedSpread(const Coord& pos, const primitives::vector& direction, enParticleColoring coloring, int particles, int avgTime)
	{
		return new ParticleRandomDirectedSpread(pos, direction.yflip(), *GetCollection(coloring), particles, avgTime);
	}
	//std::shared_ptr<ParticleEffect> ParticleEngine::AddSinglePointTrail(const Coord& pos, const primitives::vector& direction, EnumColor color, int avgTime)
	//{
	//	return AddNewEffect(newParticleOnePointTrail(pos, direction.yflip(), *GetCollection(coloring), particles, avgTime));
	//}


	ParticleCollection* ParticleEngine::GetCollection(enParticleColoring coloring)
	{
		switch (coloring)
		{
		case(enParticleColoring::greyscale):
			return &pGreySet;

		}
	}

	void ParticleEngine::clearInactiveEffects()
	{
		std::vector<std::unique_ptr<ParticleEffect>> newList = std::vector<std::unique_ptr<ParticleEffect>>();
		newList.reserve(particleEffectList.size() / 2);
		for (int i = 0; i < particleEffectList.size(); i++)
		{
			if (particleEffectList[i]->IsFinished())//GetRunTime() <= particleEffectList[i]->GetCurrentParticle().GetSLaunchTime())
			{
				//int drid = particleEffectList[i].use_count();
				//int hihi = 1;
				//particleEffectList[i].reset();

			}
			else
				newList.push_back(std::move(particleEffectList[i]));
			//if (!particleEffectList[i]->increment())
			//else
			//	newList.push_back(particleEffectList[i]);

			//particleEffectList.erase(particleEffectList.begin() + i);
		}
		particleEffectList.clear();
		particleEffectList = std::move(newList);

		if (particleEffectList.size() == 0)
		{
			generateEffect = false;
		}
	}

	void ParticleEngine::clearInactiveParticles()
	{
		if (pSet1Active)
		{
			pCurrentActiveParticleSet = &pActiveParticlesSet2;//
			pCurrentStorageParticleSet = &pActiveParticlesSet1;
		}
		else
		{
			pCurrentActiveParticleSet = &pActiveParticlesSet1;//
			pCurrentStorageParticleSet = &pActiveParticlesSet2;
		}
		pSet1Active = !pSet1Active;
		pCurrentActiveParticles = pAddParticleLock.value;
		pAddParticleLock.value = 0;

		if (pCurrentActiveParticles + pBaseParticleAdded.value != MaxParticles)
			int fail = 2;
	}

	std::shared_ptr<Particle> ParticleEngine::particleFromColor(EnumColor color)
	{
		if (color == EnumColor::YELLOW)
		{
			return particleFromColor(ColorRGB(200, 200, 0));
		}
		else if (color == EnumColor::BLACK)
		{
			return particleFromColor(ColorRGB(0, 0, 0));
		}

		return nullptr;
	}

	void ParticleEngine::UpdateParticleEffectList()
	{
		if (pParticleEffectPendingList.size() > 0)
			generateEffect = true;
		for (int i = 0; i < this->pParticleEffectPendingList.size(); i++)
			this->particleEffectList.push_back(std::move(this->pParticleEffectPendingList[i]));
		pParticleEffectPendingList.clear();
	}

	void ParticleEngine::AddParticleToBase(std::shared_ptr<Particle>& particle)
	{
		//if (pBaseParticles.size() > ParticleEngine::pMaxParticles)
		//	int fail = 2;

		SDL_LockMutex(ParticleBaseSem);
		particle->getGraphics()->activate();
		int val = SDL_AtomicIncRef(&this->pBaseParticleAdded);
		pBaseParticles[val] = std::move(particle);
		SDL_UnlockMutex(ParticleBaseSem);
	}

	void ParticleEngine::AddNextInLineActive(std::shared_ptr<Particle>& particle)
	{
		//SDL_LockMutex(NextInLineBase);
		int val = SDL_AtomicIncRef(&this->pAddParticleLock);
		(*pCurrentStorageParticleSet)[val] = std::move(particle);
		//SDL_UnlockMutex(NextInLineBase);
	}

	std::shared_ptr<Particle> ParticleEngine::particleFromColor(const ColorRGB& col)
	{
		SDL_LockMutex(ParticleBaseSem);
		if (pBaseParticleAdded.value == 0)
		{
			SDL_UnlockMutex(ParticleBaseSem);
			return nullptr;
		}
		auto result = pBaseParticles[pBaseParticleAdded.value - 1];
		result->SetParticleColor(col);
		SDL_AtomicDecRef(&pBaseParticleAdded);
		SDL_UnlockMutex(ParticleBaseSem);
		return result;
	}

	//void ParticleEngine::ResetParticles()
	//{
	//	//SDL_SemWait(sem);
	//	pActiveParticles.clear();
	//	for (int i = 0; i < pActiveParticles.size(); i++)
	//		AddParticleToBase(pActiveParticles[i]);
	//	pActiveParticles.clear();
	//	pNextInLineActiveParticles.clear();
	//	//SDL_SemPost(sem);

	//}

	//ParticleEngine::~ParticleEngine()
	//{
	//	delete pYellow->getGraphics();
	//	delete pBlack->getGraphics();
	//	delete pYellowSmall->getGraphics();
	//	delete pBlackSmall->getGraphics();
	//	delete pYellow;
	//	delete pBlack;
	//	delete pYellowSmall;
	//	delete pBlackSmall;

	//	ResetParticles();
	//}
	//template RemoveItemFromVector<std::shared_ptr<Particle>>;

}