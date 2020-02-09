#include"TorpedoProjectile.h"

//TorpedoProjectile::TorpedoProjectile() : MissileProjectile()
//{
//
//}

TorpedoProjectile::TorpedoProjectile( std::shared_ptr<GraphicalEntity> ge, double accuracy, double spread, game::Damage damage, double P, double I, double D, double Ph, double Ih, double Dh, string name)
	: MissileProjectile( ge, accuracy, spread, damage, P, I, D, Ph, Ih, Dh, name )
{
	Initialize();
}

TorpedoProjectile::TorpedoProjectile( std::shared_ptr<GraphicalEntity> ge, projectileAttributes attributes, double P, double I, double D, double Ph, double Ih, double Dh, string name)
	: MissileProjectile( ge, attributes, P, I, D, Ph, Ih, Dh, name )
{
	Initialize();
}

std::shared_ptr<Projectile> TorpedoProjectile::copyProjectile()
{
	//std::shared_ptr<InGameGraphicalObjectTree> newObject = this->pGraphics.get()->getGraphicalObject()->copy();
	std::shared_ptr<GraphicalEntity> newEntity = this->getGraphics()->copy();
	//std::shared_ptr<GraphicalEntity> newEntity = std::shared_ptr<GraphicalEntity>(new GraphicalEntity( newObject, this->getTimer(), EntityClass::PROJECTILE, this->pGraphics->isActive() ));
	TorpedoProjectile* result = new TorpedoProjectile(newEntity, this->pAttributes, this->getControllerGain(), this->getControllerIntergral(), this->getControllerDerivative(), 
																					this->getControllerGainHeading(), this->getControllerIntegralHeading(), this->getControllerDerivativeHeading(), this->GetProjectileName() );
	result->SetDrive( this->GetDrive() );
	//result->pCenterPosition = this->pCenterPosition;
	result->pPosition = this->pPosition;
	result->pVelocity = this->pVelocity;
	result->pAttributes = this->pAttributes;
	if( !this->hitTarget.expired() )
		result->setTarget( this->hitTarget.lock() );
	result->SetSimpleMass( this->GetSimpleMass() );
	result->pThrust = this->pThrust;
    result->SetAbsAngularAcceleration( this->GetAbsAngularAcceleration() );
  	return std::shared_ptr<Projectile>(result);
}

void TorpedoProjectile::Initialize()
{

	// STILL NOT WORKNIG; TORPEDO STILL DECELLARATES TO 0 !!! 

	this->pAttributes.enumHitAnimation = enHitAnimation::LARGEEXPLOSION;
	this->pDriver.SetVelocityMagnitude(0);
	//this->velocityHeadingSetPointAbsValue = 0.0;
	this->SetDrive( EnumEntityDrive::None );
	this->boostMode = false; //temporary disabled, set to 0, until function TriggerOnDrive() is fixed
	this->lifeTimeMs = 50000;
	this->boostLength = 0;	//temporary disabled, set to 0, until function TriggerOnDrive() is fixed
	this->reqCollisionDetect = true;
}