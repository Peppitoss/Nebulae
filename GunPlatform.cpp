#include"GunPlatform.h"

GunPlatform::GunPlatform() : DockingEntity()
{
	Initialize();
}

GunPlatform::GunPlatform(const Coord pos) : DockingEntity(pos)
{
	Initialize();
}

void GunPlatform::setGlobalHeading( double newHeading ) //identical set heading as vessel
{
	for( auto it = wplatforms.begin(); it != wplatforms.end(); it++ )
	{
		(*it)->setDirection( geometry::rotateVector( (*it)->getRefDirection(), newHeading ) );
	}
	DynamicEntityControlled::setGlobalHeading( newHeading );
}

void GunPlatform::Disable()
{
	this->pDestroyed = true;
	this->SetDrive(EnumEntityDrive::Stable);
	this->deFire();
}

void GunPlatform::UpdateBehavior()
{
	if( this->pDestroyed )
		return;

	if( this->isDestroyed() )
	{
		Disable();
		return;
	}

	double angleToTarget = -geometry::angleBetweenTwoPoints( this->getPosition(),this->pTarget->getPosition() ); 
	double angleDifference = geometry::shortestAngleDifference( angleToTarget, this->getGlobalHeading() );
	
	if( abs(angleDifference) < std::_Pi/8 )
		this->fire();
	else
		this->deFire();

}

//void GunPlatform::driveUpdateForDynamicPositioningHeading( int milliseconds )
//{
//	if( this->pTarget != nullptr )
//	{
//		this->setTargetState( TargetState( this->pTarget->getPosition(), this->pTarget->getVelocity(), this->pTarget->getGlobalHeading() ) );
//		this->GuidanceBox.updateHeadingToTarget( this->getPosition(),milliseconds );
//		this->GuidanceBox.generateRefHeadingModelFromCurrentWp( this->GetSimpleMass(), (double)milliseconds/1000 );
//
//		if( shipController.headingControlisTuned() )
//		{
//			this->shipController.setVelocityHeadingSetPoint( this->GuidanceBox.getVelHeadFromRefModel() );
//			this->shipController.setPositionalHeadingSetPoint( this->GuidanceBox.getPosHeadFromRefModel() );
//
//			slideHeading();
//		}
//	}
//}

//void GunPlatform::UpdateAim()
//{
//	this->setHeading(0);
//}

void GunPlatform::AddLauncher( std::shared_ptr<LauncherEntity> in, Coord pos, Coord anchorPos ) //identical set heading as vessel
{
	wplatforms.push_back( in ); 

	if( in->getGraphics()->getConnectedObjectsAmount() > 1 )
	{
		//not implemented for a sprite tree of a linked main node
		return;
	}
	else
	{
		InGameGraphicalObjectLink& generatedLink = this->getGraphics()->addGraphicalObject( in->getGraphics(), pos, anchorPos, in->GetRelativeHeading() );
		in->setRelativePosition( generatedLink.relationalZeroHeadingPosition );
		in->setLaunchPoint( this->getPosition() + this->getSize()/2 - in->getProjectile()->getHitPoint() + in->getRelativePosition() + in->getRelativeProjectileLaunchPoint() );
	}
}

void GunPlatform::fire()
{
	for ( auto it = wplatforms.begin(); it != wplatforms.end(); it++)
	{
		(*it)->setLaunch(true);
	}
}

void GunPlatform::deFire()
{
	for( auto it = wplatforms.begin(); it != wplatforms.end(); it++ )
	{
		(*it)->setLaunch(false);
	}
}

void GunPlatform::SetTarget( DynamicEntity* set )
{
	this->pTarget = set;
}

void GunPlatform::Initialize()
{
	this->pTarget = nullptr;
	this->pAccuracy = 0;
	this->pPrecision = 0;
	this->SetDrive(EnumEntityDrive::DynamicVelocityHeading);
	//this->GuidanceBox.setWaypointHeadingDependency(true);
	this->pDestroyed = false;
}

bool GunPlatform::isDestroyed()
{
	if( getPercentHullLeft() < 0.6 ) //0.3 originally, main class in GenericEntity
		return true;
	return false;
}

GunPlatform::~GunPlatform()
{
	this->wplatforms.clear();
	this->FreeMemory();
}