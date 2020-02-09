#include"ParticleTrailEffect.h"
#include"RandomNumberGenerator.h"

namespace particleEffects
{


	ParticleTrailEffect::ParticleTrailEffect(const Coord& pos, vector direction, ParticleCollection& collection, int time, int delayBetweenParticles)
	{
		pDirection = direction;
		pCurrentPosition = pos;
		pTimeTillFinish = time;
		pCollection = collection;
		pDelayBetweenParticles = delayBetweenParticles;
		pDelayTime = 0;
	}

	std::vector<ParticleTemplate> ParticleTrailEffect::RunEffect(double seconds)
	{
		std::vector<ParticleTemplate> result = std::vector<ParticleTemplate>();
		
		pCurrentPosition = vector(pCurrentPosition.x + pDirection.x*seconds, pCurrentPosition.y + pDirection.y*seconds);
		if (this->IsFinished() == false)
		{
			this->pRunTimeSeconds += seconds;
			this->pDelayTime += seconds;
			if (this->pRunTimeSeconds > this->pTimeTillFinish)
			{
				this->pIsFinished = true;
			}
			else if(pDelayTime > this->pDelayBetweenParticles)
			{
				pDelayTime = 0;
				ParticleTemplate newparticle = ParticleTemplate();
				newparticle.SetPosition((pCurrentPosition).toCoord());
				//newparticle.SetVelocity(pDirection);
				int timeleft = 35000;
				newparticle.SetTimeLeft(timeleft);

				newparticle.SetUpdateFunction([](Particle * particle)
				{
					double rate = 0.9996;
					particle->SetParticleColor(particle->GetParticleColor().ChangeColorsByPercentAmount(rate));
				});
				result.push_back(newparticle);
			}

		}
		return result;
	}
}