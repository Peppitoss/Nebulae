#include"ParticleEffect.h"
#include"Graphics.h"
#include "Sprite.h";
#include"SpriteGraphics.h"
#include"RandomNumberGenerator.h"



namespace particleEffects
{

	SDL_mutex* particleEffects::ParticleBaseSem = SDL_CreateMutex();
	SDL_mutex* particleEffects::AddParticleEffectSem = SDL_CreateMutex();
	//SDL_mutex* particleEffects::NextInLineBase = SDL_CreateMutex();

	//void ParticleEffect::initialize()
	//{
	//	if( currentStep < totalSteps )
	//	{

	//		it_particle = particleTemplate[currentStep].begin();

	//	}
	//}

	ParticleEffect::ParticleEffect()
	{
		//totalSteps = 0;
		//currentStep = 0;
		pIsFinished = false;
		pRunTimeSeconds = 0;
		sourceEffect = std::weak_ptr<GenericEntity>();// nullptr;
	}

	/*bool ParticleEffect::increment()
	{
		if( it_particle != particleTemplate[currentStep].end() )
		{
			it_particle++;

			if( it_particle == particleTemplate[currentStep].end() )
				return incrementVector();
			
			return true;
		}
		else
			return incrementVector();
	}

	bool ParticleEffect::incrementVector()
	{
		if( currentStep >= totalSteps - 1)
			return false;
		else
		{
			currentStep++;
			initialize();
			return true;
		}
	}*/

	bool ParticleEffect::IsFinished()
	{
		return pIsFinished;
	}

	bool ParticleEffect::HasSourcePosition() const
	{
		if (this->sourceEffect.expired())
			return false;
		return true;
	};

	Coord ParticleEffect::sourcePosition() const
	{
		if(this->HasSourcePosition())
			return sourceEffect.lock()->getPosition().yflip() + sourceEffect.lock()->getSize() / 2;
		return Coord(0);
	}

	

	//void ParticleEffect::ParticleExplosionSpread(const Coord& pos, double radius, ParticleCollection& collection, int particles, int averageTime)
	//{
	//	double spread = radius / 2;

	//	std::normal_distribution<double> distribution_pos_x(pos.x, 1);
	//	std::normal_distribution<double> distribution_pos_y(pos.y, 1);

	//	std::normal_distribution<double> distribution_vel_1(radius, spread);
	//	std::normal_distribution<double> distribution_vel_2(-1 * radius, spread);

	//	std::normal_distribution<double> distribution_time(averageTime, averageTime + averageTime / 2);

	//	this->pParticles = std::vector<ParticleTemplate>();
	//	for (int i = 0; i < particles; i++)
	//	{
	//		ParticleTemplate newparticle = ParticleTemplate(); // ParticleEngine::particleFromColor(collection.getRandomColor());

	//		Coord newPos = Coord(distribution_pos_x(RANDGENERATOR()), distribution_pos_y(RANDGENERATOR()));
	//		Coord newVel;
	//		if(i%4)
	//			newVel = Coord(distribution_vel_1(RANDGENERATOR()), distribution_vel_1(RANDGENERATOR()));
	//		else if (i % 3)
	//			newVel = Coord(distribution_vel_2(RANDGENERATOR()), distribution_vel_1(RANDGENERATOR()));
	//		else if( i%2)
	//			newVel = Coord(distribution_vel_2(RANDGENERATOR()), distribution_vel_2(RANDGENERATOR()));
	//		else
	//			newVel = Coord(distribution_vel_2(RANDGENERATOR()), distribution_vel_1(RANDGENERATOR()));

	//		int time = distribution_time(RANDGENERATOR());

	//		newparticle.SetVelocity(newVel);
	//		newparticle.SetPosition(newPos);
	//		newparticle.SetTimeLeft(time);
	//		newparticle.SetColor(collection.getRandomColor());
	//		pParticles.push_back(newparticle);
	//	}
	//}

	/*void ParticleEffect::ParticleOnePointTrail(const Coord& pos, const primitives::vector& direction, double speed, ParticleCollection& collection, EnumColor color, int time)
	{*/
		//if (ParticleEngine::BaseParticleSetIsEmpty())
		//	return;

		//std::normal_distribution<double> distribution_pos_x(pos.x, 0);
		//std::normal_distribution<double> distribution_pos_y(pos.y, 0);

		//std::normal_distribution<double> distribution_vel_x(direction.x, 0);
		//std::normal_distribution<double> distribution_vel_y(direction.y, 0);

		//std::normal_distribution<double> distribution_time(time, 0);

		//this->particleTemplate.push_back(std::list<ParticleTemplate>());
		//for (int i = 0; i < particles; i++)
		//{
		//	ParticleTemplate newparticle = ParticleTemplate(); // ParticleEngine::particleFromColor(collection.getRandomColor());

		//	Coord newPos = Coord(distribution_pos_x(RANDGENERATOR()), distribution_pos_y(RANDGENERATOR()));
		//	Coord newVel;
		//	if (i % 4)
		//		newVel = Coord(distribution_vel_1(RANDGENERATOR()), distribution_vel_1(RANDGENERATOR()));
		//	else if (i % 3)
		//		newVel = Coord(distribution_vel_2(RANDGENERATOR()), distribution_vel_1(RANDGENERATOR()));
		//	else if (i % 2)
		//		newVel = Coord(distribution_vel_2(RANDGENERATOR()), distribution_vel_2(RANDGENERATOR()));
		//	else
		//		newVel = Coord(distribution_vel_2(RANDGENERATOR()), distribution_vel_1(RANDGENERATOR()));

		//	int time = distribution_time(RANDGENERATOR());

		//	newparticle.SetVelocity(newVel);
		//	newparticle.SetPosition(newPos);
		//	newparticle.SetTimeLeft(time);
		//	newparticle.SetColor(collection.getRandomColor());
		//	this->particleTemplate[particleTemplate.size() - 1].push_back(newparticle);
		//}
		//totalSteps++;
		//initialize();
	//}

	ParticleEffect::~ParticleEffect()
	{
		//SDL_SemWait(sem);
		/*for( int i = 0; i < pParticles.size(); i++ )
		{
			particleTemplate[i].clear();
		}*/
		//SDL_SemPost(sem);
		//pParticles.clear();
	}

	ParticleTemplate::ParticleTemplate(const ParticleTemplate&& other)
	{
		this->pInitPosition = other.pInitPosition;
		this->pInitVelocity = other.pInitVelocity;
		this->pColor = other.pColor;
		this->pMsTimeLeft = other.pMsTimeLeft;
		this->pSLaunchTime = other.pSLaunchTime;
	}
	ParticleTemplate::ParticleTemplate(const ParticleTemplate& other)
	{
		this->pInitPosition = other.pInitPosition;
		this->pInitVelocity = other.pInitVelocity;
		this->pColor = other.pColor;
		this->pMsTimeLeft = other.pMsTimeLeft;
		this->pSLaunchTime = other.pSLaunchTime;
	}

}