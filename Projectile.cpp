#include"Projectile.h"


Projectile::Projectile( std::shared_ptr<GraphicalEntity> gE, std::string projectileName)
{
	setTexture( gE );
	Coord localCenter = this->getGraphics()->getCenter();
	pHitpoint.x = localCenter.x; //to be calculated auto later
	pHitpoint.y = localCenter.y; //to be calculated auto later
	pAttributes = projectileAttributes();
	pHasParticleEffect = false;
	this->pProjectileName = projectileName;
	this->pAllegiance = enumAllegiance::aENEMY;
	//this->pCenterPosition = this->getSize()/2;
	pCanTargetVessel = true;
	this->pAngularVelocity.setLimit(10);
	
	this->resetGuidanceReference( this->getGlobalPos() );
}

bool Projectile::ProjectileHit()
{
	return (rand() % 1000) < (int)(this->getHitChance()*1000);
}

Coord Projectile::getPosition() const
{ 
	return this->getGlobalPos();
};

Coord Projectile::getGlobalPos() const 
{ 
	return launchedPoint + this->pPosition;
};

bool Projectile::isDestroyed()
{
	return (this->getGraphics()->isActive() == false || GenericEntity::isDestroyed());
}

void Projectile::setLaunchPoint(Coord pos)
{
	this->launchedPoint = pos + Coord(this->getSize() / 2).xflip();
}


void Projectile::setTexture( std::shared_ptr<GraphicalEntity> set)
{
	pGraphics = set;
}

bool Projectile::vesselHeadingWithinBounds( Coord point, double bounds)
{
	double angleToTarget = -geometry::angleBetweenTwoPoints( this->getGlobalPos(),point ); 
	double angleDifference = geometry::shortestAngleDifference( angleToTarget, this->getGlobalHeading());

	if( std::abs(angleDifference) < bounds )
		return true;
}

//Optimization
bool Projectile::HasConstantAcceleration() const
{
	return true;
}

void Projectile::updateVelocity( Uint32 millisec )
{
	if (HasConstantAcceleration())
		return;

	DynamicEntityControlled::updateVelocity( millisec );
	this->pAttributes.damage.Direction = this->getVelocity();
}

void Projectile::setHitChance( double set )
{
	pAttributes.accuracy = set;
	if( set == 0 )
		this->reqCollisionDetect = false;
}

void Projectile::SetProjectileHitAnimation(enHitAnimation animation)
{
	this->pAttributes.enumHitAnimation = animation;
}

void Projectile::move(Uint32 millisec) 
{ 
	DynamicEntityControlled::move( millisec ); 
};

void Projectile::setCurrentThrust( primitives::vector set )
{
	this->currentThrust = set;
}

void Projectile::disable()
{ 
	this->pGraphics->disable();  
	this->reqCollisionDetect = false;
}


void Projectile::TriggerParticleEffect()
{
}
void Projectile::SetParticleEngine(particleEffects::ParticleEngine & set )
{
	particleEngine = &set;
}

game::Damage Projectile::getDamage() const
{
	return pAttributes.damage;
}


