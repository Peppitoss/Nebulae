#pragma once
#include"ParticleEffect.h"

namespace particleEffects
{

	class ParticleRandomDirectedSpread : public ParticleEffect
	{
	public:

		ParticleRandomDirectedSpread(const Coord& pos, const primitives::vector& direction, ParticleCollection& collection, int particles, int averageTime);
		std::vector<ParticleTemplate> RunEffect(double seconds);

	private:
		std::vector<ParticleTemplate> pParticles;
	};
}