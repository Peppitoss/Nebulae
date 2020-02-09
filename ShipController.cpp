#include"shipController.h"

ReferenceModelData::ReferenceModelData()
{
	ResetValues();
}

void ReferenceModelData::ResetValues()
{
	ThrustSetPoint = 0;
	AngularThrustSetPoint = 1;
	AngularAccelerationSetPoint = 0;
	VelocityMagnitudeSetPoint = 0;
	VelocityAngularHeadingSetPoint = 0;

	posRef = primitives::vector(0);
	velRef = primitives::vector(0);
	accelRef = primitives::vector(0);
	headingRef = 0;
	headingVelRef = 0;
	headingAccelRef = 0;
}

PID::PID()
{
	Gain = 0;
	Derivative = 0;
	Integration = 0;
	GainHeading = 0;
	DerivativeHeading = 0;
	IntegrationHeading = 0;
	init();

}

PID::PID( double P, double I, double D, double Ph, double Ih, double Dh)
{
	Gain = P;
	Integration = I;
	Derivative = D;
	GainHeading = Ph;
	DerivativeHeading = Ih;
	IntegrationHeading = Dh;
	init();
}


void PID::init()
{
	sumVelocityError = primitives::vector(0,0);
	sumPositionError = primitives::vector(0,0);
	sumVelocityHeadingError = 0;
	sumPositionHeadingError = 0;

	pPositionHeadingSetPoint = 0;
	pVelociyHeadingSetPoint = 0;
	pVelociySetPoint = primitives::vector(0, 0);
	pPositionSetPoint = primitives::vector(0, 0);

}

primitives::vector PID::calcNewThrustForVelocityRef( const primitives::vector & velocity )
{
	primitives::vector error = pVelociySetPoint - velocity;
	sumVelocityError += error;
	return error*Gain + sumVelocityError*Integration;
}

double PID::calcNewThrustForVelocityHeadingRef( const double & currentHeadingVelocity )
{
	double error = pVelociyHeadingSetPoint - currentHeadingVelocity;
	sumVelocityHeadingError += error;
	return error*GainHeading + sumVelocityHeadingError*IntegrationHeading;
}

primitives::vector PID::calcNewThrustForPositionalRef( const Coord & pos, primitives::vector vel )
{
	primitives::vector posError =  pPositionSetPoint- primitives::vector(pos.x,pos.y);
	primitives::vector velError = vel;
	sumPositionError += posError;
	sumPositionError.xANDyLimit( 100 );
	return posError*Gain - velError*Derivative + sumPositionError*Integration;

}

double PID::calcNewThrustForPositionalHeadingRef( double heading, double headingVelocity )
{
	double posError = geometry::shortestAngleDifference(pPositionHeadingSetPoint, heading);
	double velError = headingVelocity-this->pVelociyHeadingSetPoint;//headingVelocity - this->pVelociyHeadingSetPoint;
	sumPositionHeadingError += posError;
	return posError*GainHeading - velError*DerivativeHeading + sumPositionHeadingError*IntegrationHeading;
}

void PID::setVelocityHeadingSetPoint(double set)
{ 
	pVelociyHeadingSetPoint = set; 
};

void PID::resetSummedPositionAndVelocityErrors()
{
	this->sumPositionError = primitives::vector(0,0);
	this->sumVelocityError = primitives::vector(0,0);
}

void PID::resetSummedAngularErrors()
{
	this->sumPositionHeadingError = 0;
	this->sumVelocityHeadingError = 0;
}

ReferenceModel::ReferenceModel(const Coord vesselPosition)
{
	Initialize();
	pReferenceData.posRef = vesselPosition;
}

void ReferenceModel::ResetStates()
{
	this->pReferenceData.posRef = primitives::vector(0);
	this->pReferenceData.velRef = primitives::vector(0);
	this->pReferenceData.accelRef = primitives::vector(0);
	this->pReferenceData.headingRef = 0;
	this->pReferenceData.headingVelRef = 0;
	this->pReferenceData.headingAccelRef = 0;
}

void ReferenceModel::Initialize()
{
	ResetStates();
	pReferenceGeneratorRate = 10;
	pReferenceHeadingGeneratorRate = 20;
}

void ReferenceModel::resetReferenceModel()
{
	this->pReferenceData.accelRef.clear();
	this->pReferenceData.velRef.clear();
	this->pReferenceData.posRef.clear();
	this->pReferenceData.headingRef = 0;
	this->pReferenceData.headingVelRef = 0;
	this->pReferenceData.headingAccelRef = 0;

	//pDataCollector.clear();
	//pDataCollector.reserve(50000);
}

void ReferenceModel::resetReferenceModel(Coord entitySize)
{
	resetReferenceModel(entitySize, 0, 0);
}

void ReferenceModel::resetReferenceModel(Coord entitySize, double heading, double angularVelocity )
{
	this->pReferenceData.accelRef.clear();
	this->pReferenceData.velRef.clear();
	this->pReferenceData.posRef = primitives::vector(entitySize.x, entitySize.y);
	this->pReferenceData.headingRef = heading;
	this->pReferenceData.headingVelRef = angularVelocity;
	this->pReferenceData.headingAccelRef = 0;

	//pDataCollector.clear();
	//pDataCollector.reserve(50000);
}

void ReferenceModel::GenerateRefValues(const Coord& pos, double mass, double step_time_milliseconds)
{
	double w_0 = pReferenceGeneratorRate;
	double damping = 1;

	step_time_milliseconds /= 1000;

	primitives::vector acaccel = (this->pReferenceData.posRef*pow(w_0, 3) + this->pReferenceData.velRef * 3 * damping*pow(w_0, 2) + this->pReferenceData.accelRef * 3 * damping*w_0)*-1 + primitives::vector(pos.x, pos.y)*pow(w_0, 3);
	primitives::vector newAccel = this->pReferenceData.accelRef + acaccel*step_time_milliseconds;
	newAccel.xANDyLimit(this->GetData().AngularAccelerationSetPoint);
	primitives::vector newVelocity = this->pReferenceData.velRef + newAccel*step_time_milliseconds;
	newVelocity.xANDyLimit(this->GetData().VelocityMagnitudeSetPoint);
	primitives::vector newPosition = this->pReferenceData.posRef + newVelocity*step_time_milliseconds;

	this->pReferenceData.accelRef = newAccel;
	this->pReferenceData.velRef = newVelocity;
	this->pReferenceData.posRef = newPosition;
}
 
void ReferenceModel::GenerateRefValues(double heading, double mass, double step_time_milliseconds)
{
	double w_0 = pReferenceHeadingGeneratorRate;
	double damping = 1;

	step_time_milliseconds /= 1000;

	double acHaccel = -1 * (this->pReferenceData.headingRef*pow(w_0, 3) + this->pReferenceData.headingVelRef * 3 * damping*pow(w_0, 2) + this->pReferenceData.headingAccelRef*damping * 3 * w_0) + heading*pow(w_0, 3);
	double newHAccel = primitives::limitDouble(this->pReferenceData.headingAccelRef + acHaccel*step_time_milliseconds, this->GetData().AngularAccelerationSetPoint);
	double newHVelocity = primitives::limitDouble( this->pReferenceData.headingVelRef + newHAccel*step_time_milliseconds, this->GetData().VelocityAngularHeadingSetPoint );
	double newHPosition = this->pReferenceData.headingRef + newHVelocity*step_time_milliseconds;
	this->pReferenceData.headingRef = newHPosition;
	this->pReferenceData.headingVelRef = newHVelocity;
	this->pReferenceData.headingAccelRef = newHAccel; 

}


Guidance::Guidance()
{
 	init();
	//resetReferenceModel();
}

//Guidance::Guidance( Coord vesselPos, Coord vesselCenter )
//{
//	init();
//}

void Guidance::init()
{
	this->currentWaypoint = 0;
	//isHeadingDependent = true;
	headingRotations = 0;
	//referenceGeneratorRate = 1;
	this->distanceToWaypointRate = 0;
	this->distanceToWaypoint = 0;
	this->distanceToWaypoint_previous = 0;
	this->headingToWaypointRate = 0;
}

bool Guidance::currentWaypointReached( Coord currentPos ) const
{
	if( this->getCurrentWaypoint().positionWithinWaypoint( currentPos ) )
		return true;
	return false;
}

void Guidance::updateCurrentWaypoint( primitives::waypoint wp )
{
	this->waypoints[this->currentWaypoint] = wp;
}

void Guidance::updateCurrentWaypointFinalHeading(double heading)
{
	this->waypoints[this->currentWaypoint].finalHeading = heading;
}

primitives::waypoint Guidance::getCurrentWaypoint() const
{
	if (!this->allWaypointsReached())
	{
		return waypoints[currentWaypoint];
	}
	else
	{
		return primitives::waypoint(waypoints[currentWaypoint - 1].pos, waypoints[currentWaypoint - 1].vel, waypoints[currentWaypoint - 1].circleRadius, waypoints[currentWaypoint - 1].finalHeading);
	}
}

void Guidance::setNextWaypoint()
{
	if( this->currentWaypoint < this->waypoints.size()-1 )
	{
		this->currentWaypoint++;
	}
}

void Guidance::UpdateCurrentWaypointByTarget(primitives::TargetState newTarget, int radius)
{
	updateCurrentWaypoint(primitives::waypoint(newTarget.Position, newTarget.Velocity, radius, newTarget.Heading));
}
         
void Guidance::updateHeadingToWaypoint( Coord vesselPosition, int milliseconds )
{
	updateHeadingToWaypoint(vesselPosition, GetNextHeadingForWaypoint(vesselPosition), milliseconds);
}

void Guidance::updateHeadingToWaypoint(Coord vesselPosition, double heading, int milliseconds)
{
	this->headingToWaypoint_previous = this->headingToWaypoint;
	this->headingToWaypoint.value = GetNextHeadingForWaypoint(vesselPosition);
	this->headingToWaypointRate = 1000 * geometry::shortestAngleDifference(headingToWaypoint.value, headingToWaypoint_previous.value) / (milliseconds);

	this->distanceToWaypoint_previous = this->distanceToWaypoint;
	this->distanceToWaypoint = geometry::realDistance(vesselPosition, this->getCurrentWaypointCoord());
	this->distanceToWaypointRate = 1000*(distanceToWaypoint - distanceToWaypoint_previous) / (milliseconds);

}

double Guidance::GetNextHeadingForWaypoint( const Coord& vesselPosition)
{
	if( this->allWaypointsReached() )
		return this->getCurrentWaypoint().finalHeading;
	else
		return geometry::angleBetweenTwoPoints( vesselPosition, this->getCurrentWaypoint().pos ); //use shortest angle in this calc?
}

