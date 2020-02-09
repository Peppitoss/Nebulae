#pragma once
#include"ParticleEffect.h"

namespace particleEffects
{

	class ParticleTrailEffect : public ParticleEffect
	{
	public:

		ParticleTrailEffect(const Coord& pos, vector direction, ParticleCollection& collection, int time, int delayBetweenParticles);
		std::vector<ParticleTemplate> RunEffect(double seconds);

	private:

		ParticleCollection pCollection;
		vector pDirection;
		vector pCurrentPosition;
		int pTimeTillFinish;
		double pDelayBetweenParticles;
		double pDelayTime;
	};
}