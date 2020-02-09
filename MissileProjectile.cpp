#include"MissileProjectile.h"

MissileProjectile::MissileProjectile( std::shared_ptr<GraphicalEntity> ge, double acc, double spread, game::Damage damage, double P, double I, double D, double Ph, double Ih, double Dh, string name) : Projectile( ge, name )
{
	projectileAttributes attributes( acc, spread, damage, enHitAnimation::MISSILEEXPLOSION);
	this->pAttributes = attributes;
	this->setControllerGain( P );
	this->setControllerDerivative( D );
	this->setControllerIntergral( I );
	this->setControllerGainHeading( Ph );
	this->setControllerDerivativeHeading( Dh );
	this->setControllerIntegralHeading( Ih );
	this->init();
}

MissileProjectile::MissileProjectile( std::shared_ptr<GraphicalEntity> ge, projectileAttributes attributes, double P, double I, double D, double Ph, double Ih, double Dh, string name) : Projectile( ge, name )
{
	this->pAttributes = attributes;
	this->setControllerGain( P );
	this->setControllerDerivative( D );
	this->setControllerIntergral( I );
	this->setControllerGainHeading( Ph );
	this->setControllerDerivativeHeading( Dh );
	this->setControllerIntegralHeading( Ih );
	this->init();
}


//MissileProjectile::MissileProjectile( const MissileProjectile & other )
//{
//	GraphicalEntity* newEntity = new GraphicalEntity( this->pGraphics->getSprites(), this->getTimer(), EntityClass::PROJECTILE, this->pGraphics->isActive() );
//
//}

void MissileProjectile::init()
{
	this->hitTarget.reset();
	this->pDriver.SetVelocityMagnitude(1000);
	this->pDriver.SetAngularVelocityMagnitude(0);
	//this->velocityHeadingSetPointAbsValue = 0.0;
	this->SetDrive( EnumEntityDrive::DynamicMissileBoost );
	this->SetBoost(true);
	this->lifeTimeMs = 0;
	this->boostLength = 300;
	//this->guidanceByReference = false;
	this->reqCollisionDetect = false;
	this->EnableWaypointTracking(true);
	//this->GuidanceBox.setWaypointHeadingDependency( false );
}

std::shared_ptr<Projectile> MissileProjectile::copyProjectile()
{

	std::shared_ptr<InGameGraphicalObjectTree> newObject = this->pGraphics.get()->getGraphicalObject()->copy();
	//std::shared_ptr<InGameGraphicalObject> newSprite = this->pGraphics.get()->getGraphicalObject()->getMainObject()-> //((SpriteGraphics*)(this->pGraphics.get()))->copy();

	std::shared_ptr<GraphicalEntity> newEntity = std::shared_ptr<GraphicalEntity>(new GraphicalEntity( newObject, this->getTimer(), EntityClass::PROJECTILE, this->pGraphics->isActive() ));
	MissileProjectile* result = new MissileProjectile(newEntity, this->pAttributes, this->getControllerGain(), this->getControllerIntergral(), this->getControllerDerivative(), 
																					this->getControllerGainHeading(), this->getControllerIntegralHeading(), this->getControllerDerivativeHeading(), this->GetProjectileName());
	result->SetDrive( this->GetDrive() );
	//result->pCenterPosition = this->pCenterPosition;
	result->pPosition = this->pPosition;
	result->pVelocity = this->pVelocity;
	result->pAttributes = this->pAttributes;
	if( !this->hitTarget.expired() )
		result->setTarget( this->hitTarget.lock() );
	result->SetSimpleMass(this->GetSimpleMass());
	result->SetSimpleMomentOfInertia(this->GetSimplifiedMomentOfInertia());
	result->pThrust = this->pThrust;
    result->SetAbsAngularAcceleration(this->GetAbsAngularAcceleration());
  	return std::shared_ptr<Projectile>(result);
}

void MissileProjectile::TriggerParticleEffect()
{
}

int MissileProjectile::GetThrust()
{
	return this->pThrust;
}

void MissileProjectile::setCurrentThrust(primitives::vector set)
{
	this->pThrust = set.ABS();
	this->currentThrust = set;
}

void MissileProjectile::setCurrentThrust( int set )
{
	this->pThrust = set;
}

bool MissileProjectile::HasConstantAcceleration() const
{
	return false;
}

void MissileProjectile::setTarget(std::shared_ptr<DynamicEntity> target)
{
	this->hitTarget = target; 
	this->setGuidanceWaypoint( primitives::waypoint( target->CenterPosition(), 2 ) );
	//this->GuidanceBox.setWaypointHeadingDependency( true );
}

bool MissileProjectile::HasTarget(std::shared_ptr<DynamicEntity> target)
{
	if (this->hitTarget.expired())
		return false;

	if (this->hitTarget.lock()->isDestroyed())
		return false;

	return true;
}

bool MissileProjectile::DriveTriggered(int milliseconds)
{
	if (hitTarget.expired())
		return false;

	this->setTargetState(hitTarget.lock()->GetStateAsTarget(false));
	UpdateBoost(milliseconds);
	return true;
}

void MissileProjectile::UpdateBoost(int milliseconds)
{
	if( this->boostMode  && this->lifeTimeMs <  this->boostLength )
		this->lifeTimeMs += milliseconds;
	else
	{
		this->reqCollisionDetect = true;
		this->SetBoost(false);
	}
}

void MissileProjectile::SetBoost(bool value)
{
	if (this->boostMode == value)
		return;

	this->boostMode = value;
	if (value)
		this->SetDrive(EnumEntityDrive::DynamicMissileBoost, false);
	else
	{
		this->SetDrive(EnumEntityDrive::DynamicMissileTargetTracking, false);
		if (this->pDriver.GetController().headingControlisTuned())
		{
			this->pDriver.GetController().setVelocitySetPoint(this->getVelocity());
			this->pDriver.GetController().setVelocityHeadingSetPoint(this->getAngularVelocity());
			this->pDriver.GetController().setPositionalHeadingSetPoint(this->getUnitHeading());
			//this->pDriver.GetController().setVelocityHeadingSetPoint(this->pReferenceModel.getVelHeadFromRefModel());
			//this->pDriver.GetController().setPositionalHeadingSetPoint(this->pReferenceModel.getPosHeadFromRefModel());

			//this->pDriver.SetAngularAccelerationMagnitude(0.1);
			//this->pDriver.SetReferenceSpeed(10);
			
			//this->pReferenceModel.SetVelocityMagnitudeSetPoint(200); //don't change
			/*this->pReferenceModel.GenerateRefValues(this->GetGuidance().getHeadingToWaypoint(), this->GetReadMass(), milliseconds);*/

		}
	}
}

//
//void MissileProjectile::driveUpdateForDynamicPositioningHeading( int milliseconds )
//{
//	UpdateBoost(milliseconds);
//	
//	if( !this->hitTarget.expired() )
//	{
//		this->GuidanceBox.updateCurrentWaypoint( primitives::waypoint( this->hitTarget.lock()->CenterPosition() + Coord( this->hitTarget.lock()->getVelocity().x,this->hitTarget.lock()->getVelocity().y),2 ) );
//		this->GuidanceBox.updateHeadingToWaypoint( this->getPosition(), milliseconds );
//		this->GuidanceBox.generateRefHeadingModelFromCurrentWp( this->GetSimpleMass(), (double)milliseconds/1000 );
//
//		if( shipController.headingControlisTuned() && GuidanceBox.isWaypointHeadingDependenent() )
//		{
//			int distanceToPhaseChange = 1400;
//			int distanceToTarget = geometry::realDistance(this->getGlobalPos(),this->hitTarget.lock()->getPosition() );
//			if( !boostMode && distanceToTarget < distanceToPhaseChange )
//			{
//				this->simpleAngularAcceleration = 1.5;
//				this->GuidanceBox.setReferenceGeneratorRate(2);
//				if( !guidanceByReference )
//				{
//
//					this->shipController.setVelocityHeadingSetPoint( 3*this->GuidanceBox.getHeadingToWaypointRate()*this->GuidanceBox.getDistanceToWaypointRate()  );
//
//					if( velocitySetPointAbsValue > 1 )
//					{
//
//						Coord vLos_c = this->GuidanceBox.getCurrentWaypointCoord() - this->getPosition();
//						primitives::vector vLos = primitives::vector( vLos_c.x, vLos_c.y );
//						
//						double frameAngle = geometry::vectorHeading(vLos);
//
//						primitives::vector velocityGuidanceFrame = primitives::vector( this->getVelocity().x, this->getVelocity().y );
//						primitives::vector vMissile = geometry::rotateVector(velocityGuidanceFrame, frameAngle*-1);
//						
//						vLos.Scale(std::abs(vMissile.x)*2);
//
//						primitives::vector vNewHeading = vLos - velocityGuidanceFrame;
//						double newHeading = geometry::vectorHeading(vNewHeading);
//
//						double currentSetPoint = this->getGlobalHeading();
//
//						if( currentSetPoint * newHeading < 0 )
//							newHeading = currentSetPoint + geometry::shortestAngleDifference(newHeading,currentSetPoint);
//
//						this->shipController.setPositionalHeadingSetPoint( newHeading );
//					}
//					else
//					{
//						this->shipController.setPositionalHeadingSetPoint( this->shipController.getPositionHeadingSetPoint() );
//					}
//				}
//			}
//			else
//			{
//				this->simpleAngularAcceleration = 0.1;
//				this->GuidanceBox.setReferenceGeneratorRate(1);
//				if( !guidanceByReference )
//				{
//					velocitySetPointAbsValue = 200;
//					this->shipController.setVelocityHeadingSetPoint( this->GuidanceBox.getVelHeadFromRefModel() );
//					this->shipController.setPositionalHeadingSetPoint( this->GuidanceBox.getPosHeadFromRefModel() );
//				}
//			}
//			if( guidanceByReference )
//			{
//				this->shipController.setVelocityHeadingSetPoint( this->GuidanceBox.getVelHeadFromRefModel() );
//				this->shipController.setPositionalHeadingSetPoint( this->GuidanceBox.getPosHeadFromRefModel() );
//			}
//	
//			//slideHeading();
//		}
//	}
//	else
//		this->setAngleThrust(0);
//	if( this->shipController.driveControlisTuned() )
//		this->shipController.setVelocitySetPoint( geometry::rotateVector(  primitives::vector( 0, GetVelocitySetPointAbsValue() ), this->getUnitHeading() ) );
//}
