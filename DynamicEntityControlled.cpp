#include"DynamicEntityControlled.h"




DynamicEntityControlled::DynamicEntityControlled() : pDriver(this->getPosition())
{
	this->init();
}

DynamicEntityControlled::DynamicEntityControlled( Coord initpos ) : DynamicEntity( initpos ), pDriver(initpos)
{
	this->init();
}
void DynamicEntityControlled::init()
{
	this->currentAngularThrust = 0;
	this->currentThrust = 0;
	this->pDriver.SetSideslip(false);
	this->pDriver.SetIterationStyle(DriverIteratonStyle::enContinuously);
	this->pDriver.SeControlStyle(DriverControlStyle::enAutoControl);
	//this->SetAbsVelocity(1000);
	//this->SetAbsAngularVelocity(50);
	//this->SetAbsAngularAcceleration(10);
	this->pDriver.SetMaxAngularThrust(9999); //
}

int DynamicEntityControlled::GetThrust()
{
	return 2000;
}

void DynamicEntityControlled::move(Uint32 milliseconds)
{
	this->pDriver.RunController(milliseconds);
	DynamicEntity::move(milliseconds);
	//DynamicEntity::updateHeading(milliseconds);
	
}

void DynamicEntityControlled::ResetGuidanceStates()
{
	resetGuidanceReference( this->getPosition() );
}

void DynamicEntityControlled::resetGuidanceReference( Coord newPos )
{
	this->pDriver.ResetReferenceModel(newPos, this->getGlobalHeading(), this->getAngularVelocity() );
}

void DynamicEntityControlled::updateVelocity(Uint32 millisecond)
{
	currentThrust += GetImpulseForce();
	pAngularAcceleration += getImpulseAngularForce();

	pAcceleration = currentThrust / (1 + (double)this->GetSimpleMass());
	pAngularAcceleration = currentAngularThrust / ((double)(1 + this->GetSimplifiedMomentOfInertia())); // not 1/mass but should be inertia
	//pAngularAcceleration = primitives::limitDouble(pAngularAcceleration, this->pDriver.GetMaxAngularAcceleration() );

	pVelocity += pAcceleration *(double)millisecond / 1000;
	pAngularVelocity += pAngularAcceleration* (double)millisecond / 1000;

	this->resetImpulseForce();

	DriveEntity(millisecond);
	UpdateOutputFromDrive();
}

void DynamicEntityControlled::UpdateOutputFromDrive()
{
	if (!this->pDriver.IsDiscrete())
	{
		this->currentThrust = this->pDriver.Output().OutputThrust;
		this->currentAngularThrust = this->pDriver.Output().OutputAngularThrust;
	}
	else
	{
		this->setVelocity(this->pDriver.Output().OutputVelocity);
		this->setAngularVelocity(this->pDriver.Output().OutputAngularVelocity);
	}
}

void DynamicEntityControlled::updateHeading(Uint32 milliseconds)
{
	if (this->pDriver.IsDiscrete())
		UpdateDiscreteHeading(false);
	else
		DynamicEntity::updateHeading(milliseconds);
}

void DynamicEntityControlled::UpdateDiscreteHeading(bool masked)
{
	this->setUnitHeading(geometry::radLimit2PI(this->pDriver.GetDiscreteMovementMap().GetHeadingFromMovement(masked)));
 }

void DynamicEntityControlled::DriveEntity(int milliseconds)
{
	if (DriveTriggered(milliseconds))
	{
		this->pDriver.Drive(milliseconds, this->getPosition(), this->getUnitHeading(), this->getGlobalHeading(), this->getVelocity(), this->getAngularVelocity(), this->GetSimpleMass());
		//this->setUnitHeading(this->pDriver.Output().OutputUnitHeading);
	}
}

bool DynamicEntityControlled::DriveTriggered(int milliseconds)
{
	return true;
}

void DynamicEntityControlled::SetRotationDrive(EnumEntityDrive set)
{
	SetRotationDrive(set, true);
}

void DynamicEntityControlled::SetDrive(EnumEntityDrive set)
{
	SetDrive(set, true);
}

void DynamicEntityControlled::SetRotationDrive(EnumEntityDrive set, bool resetStates)
{
	if (this->pDriver.GetEnumDriveRotate() == set)
		return;
	this->pDriver.SetRotateDrive(set, resetStates);
}

void DynamicEntityControlled::SetDrive(EnumEntityDrive set, bool resetStates)
{ 
	if( this->pDriver.GetEnumDrive() == set )
		return;
	this->pDriver.SetDrive(set, resetStates);
}

bool DynamicEntityControlled::currentWaypointReached()
{
	return this->pDriver.GetGuidance().currentWaypointReached(this->getPosition());
}

bool DynamicEntityControlled::vesselHeadingWithinBounds( Coord point, double bounds)
{
	double angleToTarget = -geometry::angleBetweenTwoPoints( this->getPosition(),point ); 
	double angleDifference = geometry::shortestAngleDifference( angleToTarget, this->getUnitHeading() );

	if( std::abs(angleDifference) < bounds )
		return true;
}

void DynamicEntityControlled::SetDisableDrive()
{
	DynamicEntity::setEntityDead();
	pDriver.ShutoffDriver();
}

void DynamicEntityControlled::setGuidanceWaypoint( primitives::waypoint wp )
{
	this->pDriver.GetGuidance().clearWaypoints();
	this->pDriver.GetGuidance().addWaypoint( wp );

}

void DynamicEntityControlled::changeGuidanceCurrentWaypoint( primitives::waypoint wp )
{
	if(this->pDriver.GetGuidance().waypointCount() > 0 )
		this->pDriver.GetGuidance().updateCurrentWaypoint( wp );
}

void DynamicEntityControlled::addGuidanceWaypoint( primitives::waypoint wp )
{
	this->pDriver.GetGuidance().addWaypoint( wp );
}

void DynamicEntityControlled::setTargetState( const primitives::TargetState & src )
{
	this->pDriver.GetGuidance().UpdateCurrentWaypointByTarget(src, 1);
}

void DynamicEntityControlled::setWaypointHeading(double heading)
{
	if (this->pDriver.HasWaypoints())
		this->pDriver.GetGuidance().updateCurrentWaypointFinalHeading(heading);
	else
		setGuidanceWaypoint(primitives::waypoint(Coord(0), 0, heading));
}
