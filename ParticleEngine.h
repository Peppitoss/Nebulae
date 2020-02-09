#pragma once
#include"ParticleEffect.h"
#include<SDL_thread.h>

namespace particleEffects
{



	class ParticleEngine
	{
	public:

		ParticleEngine();

		/*	std::vector<std::shared_ptr<Particle>>::const_iterator const begin() const { return pActiveParticles.begin(); }
		std::vector<std::shared_ptr<Particle>>::const_iterator const end() const { return pActiveParticles.end(); }*/

		//void AddParticle( std::shared_ptr<Particle> particle ) { pActiveParticles.push_back( particle ); };

		void Move(Uint32 milliseconds, int lowerBound, int upperBound);
		void ReduceTimeOnParticle(double millisec, int lowerBound, int upperBound);
		void ResetAllParticles();
		void RunEffects(double ms, int lowerBound, int upperBound);

		static std::shared_ptr<Particle> particleFromColor(EnumColor);
		static std::shared_ptr<Particle> particleFromColor(const ColorRGB&);
		static bool BaseParticleSetIsEmpty() { return pBaseParticleAdded.value == 0; };

		int particleCount() { return pCurrentActiveParticles; };
		bool effectIsOn() { return generateEffect; };

		void AddNextInLineActive(std::shared_ptr<Particle>& particle);
		void clearInactiveEffects();
		void clearInactiveParticles();
		void UpdateParticleEffectList();

		void AddNewEffect(ParticleEffect* effect);

		ParticleEffect* GetEffectDustSpread(const Coord& pos, double speed, int particles, int particleLaunchInterval, int avgTime);
		ParticleEffect* GetEffectParticleTrail(const Coord& pos);
		ParticleEffect* GetEffectRandomDirectedSpread(const Coord& pos, const primitives::vector& direction, enParticleColoring, int particles, int avgTime);

		//std::shared_ptr<ParticleEffect> AddSinglePointTrail(const Coord& pos, const primitives::vector& direction, EnumColor color, int avgTime);

		//void AddParticleEffect(std::shared_ptr<ParticleEffect> effect);
		//static int MaxParticles() { return particleEffects::MaxParticles; }

		static std::array<std::shared_ptr<Particle>, 1000> pBaseParticles; //temporarily public		 //1000 should be max particles
		std::array<std::shared_ptr<Particle>, 1000>& ActiveParticleList() { return *pCurrentActiveParticleSet; }		//1000 should be max particles

	protected:

		bool generateEffect;
		ParticleCollection* GetCollection(enParticleColoring coloring);
		//std::shared_ptr<Particle> lastGenParticle;

		//std::shared_ptr<Particle> pYellowSmall;
		//std::shared_ptr<Particle> pBlackSmall;
		GreyScaleParticles pGreySet;
		void AddParticleToBase(std::shared_ptr<Particle>& particle);
		//void RemoveParticleFromActive(std::shared_ptr<Particle> particle);

		std::array<std::shared_ptr<Particle>, 1000> pActiveParticlesSet2;	//1000 should be max particles
		std::array<std::shared_ptr<Particle>, 1000> pActiveParticlesSet1;			//1000 should be max particles
		std::array<std::shared_ptr<Particle>, 1000>* pCurrentActiveParticleSet;
		std::array<std::shared_ptr<Particle>, 1000>* pCurrentStorageParticleSet;
		std::vector<std::unique_ptr<ParticleEffect>> particleEffectList;
		std::vector< std::unique_ptr<ParticleEffect>> pParticleEffectPendingList;

		//std::default_random_engine pRandomEngine;
		//void ResetParticles();
		void loadParticleSurfaces();

	private:

		int pCurrentActiveParticles;
		//static int pMaxParticles;
		SDL_atomic_t pAddParticleLock;
		static SDL_atomic_t pBaseParticleAdded;
		bool pSet1Active;


		ParticleEngine(const ParticleEngine&);
		ParticleEngine(ParticleEngine&&);
		ParticleEngine operator=(const ParticleEngine &);
		ParticleEngine operator=(ParticleEngine&&);

	};

}
