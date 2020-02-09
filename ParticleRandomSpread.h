#pragma once
#include"ParticleEffect.h"
#include"VectorHelper.cpp"


namespace particleEffects
{
	class ParticleRandomSpread : public ParticleEffect
	{
	public:

		ParticleRandomSpread(const Coord& pos, double speed, ParticleCollection& collection, int particles, double particleLaunchInterval, int averageTime);
		std::vector<ParticleTemplate> RunEffect(double seconds);

	private:
		std::vector<ParticleTemplate> pParticles;
	};
}

template void RemoveItemsFromVector<particleEffects::ParticleTemplate>(std::vector<particleEffects::ParticleTemplate>&, std::vector<int>);
