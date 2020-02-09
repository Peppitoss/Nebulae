#pragma once
#include"Projectile.h"

class RailProjectile : public Projectile
{

public:

	RailProjectile( std::shared_ptr<GraphicalEntity> , double accuracy, double spread, game::Damage damage, string name );	
	int GetThrust() override;

	std::shared_ptr<Projectile> copyProjectile() override;
	void TriggerParticleEffect() ;
	//void SetParticleEngine(particleEffects::ParticleEngine & set ) override;

protected:


private:

	//std::shared_ptr<GenericEntity> pSelfReference;

};