#include"ParticleRandomDirectedSpread.h"
#include"RandomNumberGenerator.h"
#include"VectorHelper.h"
namespace particleEffects
{

	ParticleRandomDirectedSpread::ParticleRandomDirectedSpread(const Coord& pos, const primitives::vector& direction, ParticleCollection& collection, int particles, int averageTime)
	{
		std::normal_distribution<double> distribution_pos_x(pos.x, 1);
		std::normal_distribution<double> distribution_pos_y(pos.y, 1);

		std::normal_distribution<double> distribution_vel_x(direction.x, 2);
		std::normal_distribution<double> distribution_vel_y(direction.y, 2);

		std::normal_distribution<double> distribution_time(averageTime, averageTime + averageTime / 2);

		this->pParticles = std::vector<ParticleTemplate>();
		for (int i = 0; i < particles; i++)
		{
			ParticleTemplate newparticle = ParticleTemplate(); // ParticleEngine::particleFromColor(collection.getRandomColor());
															   //if (newparticle.get() == false)
															   //	break;

			Coord newPos = Coord(distribution_pos_x(RANDGENERATOR()), distribution_pos_y(RANDGENERATOR()));
			Coord newVel = Coord(distribution_vel_x(RANDGENERATOR()), distribution_vel_y(RANDGENERATOR()));
			int time = distribution_time(RANDGENERATOR());

			newparticle.SetPosition(newPos);
			newparticle.SetVelocity(newVel);
			newparticle.SetTimeLeft(time);
			newparticle.SetColor(collection.getRandomColor());
			pParticles.push_back(newparticle);
		}
	}

	std::vector<ParticleTemplate> ParticleRandomDirectedSpread::RunEffect(double seconds)
	{
		std::vector<ParticleTemplate> result = std::vector<ParticleTemplate>();
		if (this->IsFinished() == false)
		{
			for (int i = 0; i< this->pParticles.size(); i++)
				result.push_back(pParticles[i]);
			this->pIsFinished = true;
		}
		return result;
	}
}