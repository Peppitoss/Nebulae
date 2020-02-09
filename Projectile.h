#pragma once
#include"AnimationCollection.h"
#include"GameAttributes.h"
#include"DynamicEntityControlled.h"
#include"SpriteGraphics.h"
#include"ParticleEngine.h"



struct projectileAttributes
{
	projectileAttributes()
	{
		accuracy = 0.9;
		spreading = 0.2; //must be larger than 0
		damage.value = 5;
		enumHitAnimation = enHitAnimation::NOANIMATION;
	}

	projectileAttributes( const projectileAttributes& other )
	{
		setAccuracy(other.accuracy);
		setSpread(other.spreading);
		this->damage = other.damage;
		enumHitAnimation = other.enumHitAnimation;
	}

	projectileAttributes( double acc, double spread, game::Damage damage, enHitAnimation hitAnimation )
	{
		setAccuracy(acc);
		setSpread(spread);
		this->damage = damage;
		enumHitAnimation = hitAnimation;
	}
	projectileAttributes( double acc, double spread, game::Damage damage )
	{
		setAccuracy(acc);
		setSpread(spread);
		this->damage = damage;
		enumHitAnimation = enHitAnimation::NOANIMATION;
	}

	projectileAttributes& operator=( const projectileAttributes & other )
	{
		this->setAccuracy(other.accuracy);
		this->setSpread(other.spreading);
		this->damage = other.damage;
		this->enumHitAnimation = other.enumHitAnimation;
		return *this;
	}

	void setSpread(  double spread )
	{
		if(spread <= 0.0 )
			spreading = 0.001;
		else
			spreading = spread;
	}

	void setAccuracy( double acc ) 
	{
		if( acc < 0 )
			acc = 0;
		if( acc > 1 )
			acc = 1;
		accuracy = acc;
	}

	void setAOE( int dmg, int range )
	{
		this->damage.AOE = dmg;
		this->damage.AOE_range = range;
	}

	double accuracy;
	double spreading;
	game::Damage damage;
	enHitAnimation enumHitAnimation;

};

class Projectile : public DynamicEntityControlled, public std::enable_shared_from_this<Projectile>
{

public:

	Projectile( std::shared_ptr<GraphicalEntity> , std::string projectileName);

	int getTimer() { return this->getGraphics()->getTimer(); };
	Coord getLaunchPoint() const {return launchedPoint; };
	Coord getHitPoint() const { return pHitpoint; };

	Coord getGlobalPos() const;
	bool isDestroyed() override ;

	virtual std::shared_ptr<Projectile> copyProjectile() = 0;
	Coord getPosition() const override;
	//Coord getPosition() const override { return this->getGlobalPos(); };
	/*virtual std::shared_ptr<Projectile> copyProjectile(int);*/
	bool ProjectileHit();

	void setLaunchPoint(Coord pos);// { this->launchedPoint = pos; };
	void setTexture( std::shared_ptr<GraphicalEntity> );
	void setHitPoint( Coord set ) { pHitpoint = set; };
	void updateVelocity( Uint32 );
	void move(Uint32 millisec);
	virtual void setCurrentThrust( primitives::vector );
	void disable();
	//
	virtual bool CanTargetVessel() const { return pCanTargetVessel; }
	virtual bool HasConstantAcceleration() const;
	void SetCanTargetVessel(bool set) { this->pCanTargetVessel = set; }
	bool HasParticleEffect() { return this->pHasParticleEffect; }; 

	virtual void TriggerParticleEffect() = 0;
	void SetParticleEngine(particleEffects::ParticleEngine & set );

	double getHitChance() const { return pAttributes.accuracy; };
	void setHitChance( double set );
  	double getSpread() const { return pAttributes.spreading; };
	game::Damage getDamage() const; // { return pAttributes.damage; };
	enHitAnimation getHitAnimation() const { return pAttributes.enumHitAnimation; };
	void SetProjectileHitAnimation(enHitAnimation animation);

	void setHitAnimation( enHitAnimation type ) {pAttributes.enumHitAnimation = type;}; 

	std::string GetProjectileName() { return this->pProjectileName; };

	bool IsPlayeAlly() const { return this->GetAllegiance() == enumAllegiance::aFRIENDLY; }
	enumAllegiance GetAllegiance() const { return this->pAllegiance; }
	void SetAllegiance(enumAllegiance allegiance) { this->pAllegiance = allegiance; }

protected:

	bool vesselHeadingWithinBounds( Coord point, double bounds);
	projectileAttributes pAttributes;
	Coord pHitpoint;
	Coord launchedPoint;
	enumAllegiance pAllegiance;
	

	bool pHasParticleEffect;
	particleEffects::ParticleEngine* particleEngine;
	bool pCanTargetVessel;

private:

	std::string pProjectileName;

};


