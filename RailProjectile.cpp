#include"RailProjectile.h"
#include"GameManager.h"
#include <memory>

RailProjectile::RailProjectile( std::shared_ptr< GraphicalEntity> ge, double acc, double spread, game::Damage damage, string name ) : Projectile( ge, name )
{
	projectileAttributes attributes( acc, spread, damage, enHitAnimation::RAILEXPLOSION);	//replace to large explosion in the future, when these animations exist
	this->pAttributes = attributes;
	//this->disbleController();
	this->pHasParticleEffect = true;
	//SDL_SemWait(particleEffects::sem);
	this->SetParticleEngine( *GameManager::GetParticleEngine() );
	//SDL_SemPost(particleEffects::sem);
}

std::shared_ptr<Projectile> RailProjectile::copyProjectile()
{
	std::shared_ptr<GraphicalEntity> newEntity = this->getGraphics()->copy();
	RailProjectile* result = new RailProjectile(newEntity, this->pAttributes.accuracy, this->pAttributes.spreading, this->pAttributes.damage, this->GetProjectileName());
	//result->pCenterPosition = this->pCenterPosition;
	result->pPosition = this->pPosition;
	result->pVelocity = this->pVelocity;
	result->SetAbsAngularAcceleration(0);
	result->pAttributes = this->pAttributes;
	result->pHasParticleEffect = this->pHasParticleEffect;
	result->particleEngine = this->particleEngine;
	result->SetSimpleMass(this->GetSimpleMass());
	result->SetSimpleMomentOfInertia(this->GetSimplifiedMomentOfInertia());
	return std::shared_ptr<RailProjectile>(result);
}

void RailProjectile::TriggerParticleEffect()
{
	this->pHasParticleEffect = false;
	//if( ( this->getTimer()%100 ) > 50  )
	//{
		Coord globalPosition = this->getPosition().yflip() + this->getSize()/2;
		//SDL_SemWait(particleEffects::sem);
		int particles = 70;
		auto effect = this->particleEngine->GetEffectDustSpread( Coord( globalPosition.x , globalPosition.y ) , 0.2, particles, 200, 12000 );
		if (effect != nullptr)
		{
			effect->SetEffectSource(shared_from_this());
			this->particleEngine->AddNewEffect(effect);
		}
		//SDL_SemPost(particleEffects::sem);
	//}
	
}

//void RailProjectile::SetParticleEngine(particleEffects::ParticleEngine & set ) 
//{ 
//	particleEngine = &set; 
//};
int RailProjectile::GetThrust()
{
	return 0;
}