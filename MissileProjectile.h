#pragma once
#include"Projectile.h"

class MissileProjectile : public Projectile
{

public:

	MissileProjectile( std::shared_ptr<GraphicalEntity> , double accuracy, double spread, game::Damage damage, double P, double I, double D, double Ph, double Ih, double Dh, string name);
	MissileProjectile( std::shared_ptr<GraphicalEntity> , projectileAttributes, double P, double I, double D, double Ph, double Ih, double Dh, string name);
	//MissileProjectile( const MissileProjectile & other );

	void init();
	std::shared_ptr<Projectile> copyProjectile() override;

	//void driveUpdateForDynamicPositioningHeading( int milliseconds );
	Coord getPosition() const override  { return this->getGlobalPos(); };
	void setTarget( std::shared_ptr<DynamicEntity> );
	std::weak_ptr<DynamicEntity> GetTarget() const { return this->hitTarget; }
	bool HasTarget(std::shared_ptr<DynamicEntity> target);

	int GetThrust() override;
	void setCurrentThrust(primitives::vector set) override;// { pThrust = set; };
	void setCurrentThrust( int amount );

	void SetBoostTime( int set ) { this->boostLength = set; };
	int GetBoostTime() const { return this->boostLength; };

	void TriggerParticleEffect() override;
	//~MissileProjectile();
	bool HasConstantAcceleration() const override;

	bool CanTargetVessel() const  override  { return pCanTargetVessel; }

protected:

	int pThrust;
	int lifeTimeMs;
	int boostLength;
	bool boostMode;

	//bool guidanceByReference;
	std::weak_ptr<DynamicEntity> hitTarget;

	bool DriveTriggered(int milliseconds) override;
	void UpdateBoost(int milliseconds);
	void SetBoost(bool value);


private:

};