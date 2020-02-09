#pragma once
#include"Projectile.h"

class GunProjectile : public Projectile
{

public:

	GunProjectile( std::shared_ptr<GraphicalEntity> , double accuracy, double spread, double damage, double structdamage, string name);
	int GetThrust() override;

	std::shared_ptr<Projectile> copyProjectile() override;
	void TriggerParticleEffect() override;

protected:


};