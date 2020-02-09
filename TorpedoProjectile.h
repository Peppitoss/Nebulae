#pragma once
#include"MissileProjectile.h"

class TorpedoProjectile : public MissileProjectile
{
public:

	TorpedoProjectile( std::shared_ptr<GraphicalEntity> , double accuracy, double spread, game::Damage damage, double P, double I, double D, double Ph, double Ih, double Dh, string name);
	TorpedoProjectile( std::shared_ptr<GraphicalEntity> , projectileAttributes, double P, double I, double D, double Ph, double Ih, double Dh, string name);
	
	std::shared_ptr<Projectile> copyProjectile() override;


private:

	void Initialize();

};