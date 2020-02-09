#pragma once
#include"ParticleColorDefinitions.h"
#include "ParticleCollections.h"
#include"Particles.h"
#include <array>
#include <random>


enum EnumParticleEffect
{
	noeffect = 0,
	particledustspread = 1,
	randomDirectionOnePoint = 2,
	randomDirectedSpread = 3,
	explosionSpread1 = 4,

};
//
enum enParticleColoring
{
	greyscale = 0,
};

namespace particleEffects
{
	extern SDL_mutex *ParticleBaseSem;
	extern SDL_mutex *AddParticleEffectSem;
	extern const int MaxParticles;
	//extern SDL_mutex *NextInLineBase;

	class ParticleTemplate
	{
	public:

		ParticleTemplate() {};
		ParticleTemplate(const ParticleTemplate&&);
		ParticleTemplate(const ParticleTemplate&);

		primitives::vector GetVelocity() const { return this->pInitVelocity; }
		Coord GetPosition() const { return this->pInitPosition; }
		int GetMsTimeLeft() const { return this->pMsTimeLeft; }
		double GetSLaunchTime() const { return this->pSLaunchTime; }
		ColorRGB GetColor() const { return pColor; }
		std::function<void(Particle*)> GetUpdateFunction() { return pUpdateFunction; };

		void SetVelocity(const primitives::vector& set) { this->pInitVelocity = set; }
		void SetPosition(const Coord& set) { this->pInitPosition = set; }
		void SetTimeLeft(int time) { this->pMsTimeLeft = time; }
		void SetColor(const ColorRGB& set) { this->pColor = set; }
		void SetLaunchTime(double launcTimeSeconds) { this->pSLaunchTime = launcTimeSeconds; }

		void SetUpdateFunction(std::function<void(Particle*)> callback) { pUpdateFunction = callback; }
		bool HasUpdateFunction() { return pUpdateFunction != nullptr; }


	private:


		std::function<void(Particle*)> pUpdateFunction;
		Coord pInitPosition;
		primitives::vector pInitVelocity;
		ColorRGB pColor;
		int pMsTimeLeft;
		double pSLaunchTime;

	};

	class ParticleEffect
	{

	public:

		ParticleEffect();

		virtual std::vector<ParticleTemplate> RunEffect(double seconds) = 0;
		bool IsFinished();

		void SetEffectSource(std::weak_ptr<GenericEntity>  set) { this->sourceEffect = set; }
		bool HasSourcePosition() const;
		Coord sourcePosition() const;

		~ParticleEffect();

	protected:

		std::weak_ptr<GenericEntity> sourceEffect;
		double pRunTimeSeconds;
		bool pIsFinished;

	};
}