#include"ParticleRandomSpread.h"
#include"RandomNumberGenerator.h"



namespace particleEffects
{

	ParticleRandomSpread::ParticleRandomSpread(const Coord& pos, double speed, ParticleCollection& collection, int particles, double particleLaunchInterval, int averageTime)
	{
		std::normal_distribution<double> distribution_pos_x(pos.x, 1);
		std::normal_distribution<double> distribution_pos_y(pos.y, 1);

		std::normal_distribution<double> distribution_vel_x(0.1, speed);
		std::normal_distribution<double> distribution_vel_y(0.1, speed);

		std::normal_distribution<double> distribution_time(averageTime, averageTime + averageTime / 2);

		this->pParticles = std::vector<ParticleTemplate>();
		double launchTime = 0;
		for (int i = 0; i < particles; i++)
		{
			ParticleTemplate newparticle = ParticleTemplate(); // ParticleEngine::particleFromColor(collection.getRandomColor());
															   //if (newparticle.get() == false)
															   //	break;

			Coord newPos = Coord(distribution_pos_x(RANDGENERATOR()), distribution_pos_y(RANDGENERATOR()));
			Coord newVel = Coord(distribution_vel_x(RANDGENERATOR()), distribution_vel_y(RANDGENERATOR()));
			int time = distribution_time(RANDGENERATOR());
			if (time < 0)
				time = averageTime;

			newparticle.SetPosition(newPos);
			newparticle.SetVelocity(newVel);
			newparticle.SetTimeLeft(time);
			newparticle.SetLaunchTime(launchTime);
			newparticle.SetColor(collection.getRandomColor());
			pParticles.push_back(newparticle);
			launchTime += particleLaunchInterval / 1000;
		}
	}

	std::vector<ParticleTemplate> ParticleRandomSpread::RunEffect(double seconds)
	{
		std::vector<ParticleTemplate> result = std::vector<ParticleTemplate>();
		if (this->IsFinished() == false)
		{
			if (this->pParticles.size() == 0)
			{
				this->pIsFinished = true;
				return result;
			}

			this->pRunTimeSeconds += seconds;
			std::vector<int> indexToKeep = std::vector<int>();
			for (int i = 0; i< this->pParticles.size(); i++)
			{
				if (this->pParticles[i].GetSLaunchTime() < this->pRunTimeSeconds)
				{
					result.push_back(pParticles[i]);
				}
				else
					indexToKeep.push_back(i);
			}
			RemoveItemsFromVector<ParticleTemplate>(pParticles, indexToKeep);
		}
		return result;
	}
}