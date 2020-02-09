#include"GunProjectile.h"

GunProjectile::GunProjectile( std::shared_ptr< GraphicalEntity> ge, double acc, double spread, double damage, double structdamage, string name ) : Projectile( ge, name )
{
	enHitAnimation hitanimation;
	if (structdamage > 20)
		hitanimation = enHitAnimation::HECANNONHIT;
	else
		hitanimation = enHitAnimation::CANNONHIT;

	//projectileAttributes attributes = projectileAttributes( acc, spread, game::Damage(damage, structdamage, 1), hitanimation);
	this->pAttributes = projectileAttributes(acc, spread, game::Damage(damage, structdamage, 1), hitanimation);//attributes;
	//this->DisbleController();
}

std::shared_ptr<Projectile> GunProjectile::copyProjectile()
{
	std::shared_ptr<GraphicalEntity> newEntity = this->getGraphics()->copy();
	GunProjectile* result = new GunProjectile(newEntity,this->pAttributes.accuracy, this->pAttributes.spreading, this->pAttributes.damage.value, this->pAttributes.damage.AOE, this->GetProjectileName());
	//result->pCenterPosition = this->pCenterPosition;
	result->pPosition = this->pPosition;
	result->pVelocity = this->pVelocity;
	result->SetAbsAngularAcceleration(0);
	result->SetSimpleMass(this->GetSimpleMass());
	result->SetSimpleMomentOfInertia(this->GetSimplifiedMomentOfInertia());
	result->pAttributes = this->pAttributes;
	result->pHasParticleEffect = this->pHasParticleEffect;
	//SDL_SemWait(particleEffects::sem);
	result->particleEngine = this->particleEngine;
	//SDL_SemPost(particleEffects::sem);
	return std::shared_ptr<GunProjectile>(result);
}

int GunProjectile::GetThrust()
{
	return 0;
}

void GunProjectile::TriggerParticleEffect()
{
}