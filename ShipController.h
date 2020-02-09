#pragma once
#include"Graphics.h"
#include"Angle.h"

class PID
{

public:
	
	PID();
	PID( double P, double I, double D, double, double, double);
	primitives::vector setPoint;

	//------------------------
	//Guidance LAWS
	


	//-------------------------

	primitives::vector calcNewThrustForVelocityRef( const primitives::vector & currentVelocity );
	double calcNewThrustForVelocityHeadingRef( const double & currentHeadingVelocity );
	primitives::vector calcNewThrustForPositionalRef( const Coord & pos , primitives::vector vel);
	double calcNewThrustForPositionalHeadingRef( double heading, double headingVelocity );
	void resetError() { sumVelocityError = primitives::vector(0,0); sumPositionError = primitives::vector(0,0); sumVelocityHeadingError = 0; };

	void Disable() { Gain = 0; GainHeading = 0; };

	bool driveControlisTuned() { return (Gain>0); };
	bool headingControlisTuned() { return (GainHeading>0); };

	double Gain;
	double Derivative;
	double Integration;

	double GainHeading;
	double DerivativeHeading;
	double IntegrationHeading;

	void setVelocitySetPoint( primitives::vector set ) { pVelociySetPoint = set; };
	void setVelocityHeadingSetPoint(double set);
	void setPositionSetPoint( primitives::vector set ) { pPositionSetPoint = set; };
	void setPositionalHeadingSetPoint( double set ) {pPositionHeadingSetPoint = set; };

	//void Coord getpPosition

	double getPositionHeadingSetPoint() const { return pPositionHeadingSetPoint; };
	double getVelocityHeadingSetPoint() const { return pVelociyHeadingSetPoint; };

	primitives::vector getPositionSetPoint() { return pPositionSetPoint; };
	primitives::vector getVelocitySetPoint() { return pVelociySetPoint; };

private:

	void resetSummedPositionAndVelocityErrors();
	void resetSummedAngularErrors();
	//double Derivative;
	primitives::vector pPositionSetPoint;
	primitives::vector sumPositionError;

	primitives::vector pVelociySetPoint;
	primitives::vector sumVelocityError;

	double pPositionHeadingSetPoint;
	double sumPositionHeadingError;

	double pVelociyHeadingSetPoint;
	double sumVelocityHeadingError;

	void init();

};

struct ReferenceModelData
{
	ReferenceModelData();

	void ResetValues();

	double ThrustSetPoint;
	double AngularThrustSetPoint;
	double AngularAccelerationSetPoint;
	double VelocityMagnitudeSetPoint;
	double VelocityAngularHeadingSetPoint;

	primitives::vector posRef;
	primitives::vector velRef;
	primitives::vector accelRef;
	double headingRef;
	double headingVelRef;
	double headingAccelRef;
};

class ReferenceModel
{

public:

	ReferenceModel(const Coord vesselPosition );

	primitives::vector getVelFromRefModel() { return this->pReferenceData.velRef; };
	primitives::vector getPosFromRefModel() { return this->pReferenceData.posRef; };
	double getPosHeadFromRefModel() { return this->pReferenceData.headingRef; };
	void setPosHeadFromRefModel(double set) { this->pReferenceData.headingRef = set; };
	double getVelHeadFromRefModel() { return this->pReferenceData.headingVelRef; };

	void GenerateRefValues(const Coord& pos, double mass, double step_time_milliseconds);
	void GenerateRefValues(double heading, double mass, double step_time_seconds);

	void SetReferenceGeneratorRate(double set) { pReferenceGeneratorRate = set; };
	void SetReferenceGeneratorHeadingRate(double set) { pReferenceHeadingGeneratorRate = set; };

	void resetReferenceModel();
	void resetReferenceModel(Coord entitySize);
	void resetReferenceModel(Coord entitySize, double heading, double angularvelocity);

	void SetAngularThrustSetPoint(double set) { this->pReferenceData.AngularThrustSetPoint = set; }
	void SetThrustSetPoint(double set) { this->pReferenceData.ThrustSetPoint = set; }
	void SetAngularAccelerationSetPoint(double set) { this->pReferenceData.AngularAccelerationSetPoint = set; }
	void SetVelocityMagnitudeSetPoint(double set) { this->pReferenceData.VelocityMagnitudeSetPoint = set; }

	ReferenceModelData& GetData() { return this->pReferenceData; }

	void ResetStates();

private:

	void Initialize();
	ReferenceModelData pReferenceData;
	double pReferenceGeneratorRate;
	double pReferenceHeadingGeneratorRate;

	//double pMaxHeadingVel;
	//double pMaxHeadingAcceleration;

	//std::vector<double> pDataCollector;
};



class Guidance
{

public:
	Guidance();
	//Guidance( Coord vesselPos, Coord vesselCenter );

	void init();

	void addWaypoint( primitives::waypoint wp ) { waypoints.push_back( wp ); };
	void addWaypoint( Coord pos, double radius ) { waypoints.push_back( primitives::waypoint(pos, radius ) ); };

	primitives::waypoint getCurrentWaypoint() const;// { return waypoints[currentWaypoint]; };
	void updateCurrentWaypoint( primitives::waypoint );
	void updateCurrentWaypointFinalHeading( double heading); 
	Coord getCurrentWaypointCoord() const { return getCurrentWaypoint().pos; };
	primitives::vector getCurrentWaypointVelocity() const { return getCurrentWaypoint().vel; };
	double getWaypointFinalHeading() const { return this->getCurrentWaypoint().finalHeading;}
	double getCurrentWaypointRadius() const  { return getCurrentWaypoint().circleRadius; };
	int waypointCount() const { return this->waypoints.size(); };

	double getHeadingToWaypoint() { return headingToWaypoint.value; };
	void updateHeadingToWaypoint( Coord vesselPosition,  int milliseconds );
	void updateHeadingToWaypoint(Coord vesselPosition, double heading, int milliseconds);
	//void updateHeadingToTarget( Coord vesselPosition, int milliseconds );
	//void updateHeadingToTarget( Coord vesselPosition, double heading, int milliseconds );

	bool currentWaypointReached( Coord currentPos ) const;
	bool allWaypointsReached() const  { return currentWaypoint >= waypoints.size(); };

	//void generateRefModelFromCurrentWp( double vesselMass, int stepTime );
	//void generateRefPosModelFromCurrentWp( double vesselMass, double stepTime );
	//void generateRefHeadingModelFromCurrentWp( double vesselMass, double stepTime );
	//void generateRefHeadingModelFromCurrentWp( double heading, double vesselMass, double seconds_step );

	//void setRefModelPos( Coord pos);// { vesselRefModel.posRef = primitives::vector(pos.x,pos.y); };
	//void setRefModelHeading( double heading );// { vesselRefModel.headingRef = heading; };
	void setNextWaypoint();
	
	void clearWaypoints() { waypoints.clear(); currentWaypoint = 0; };



	double getDistanceToWaypointRate() { return this->distanceToWaypointRate;}

	//bool isWaypointHeadingDependenent() { return isHeadingDependent; };
	//void setWaypointHeadingDependency( bool set ){ isHeadingDependent = set; };

	double getHeadingToWaypointRate() { return headingToWaypointRate; };


	double GetNextHeadingForWaypoint( const Coord& vesselpos);
	primitives::vector getWaypointVelocity() { return primitives::vector(getCurrentWaypoint().vel.x, getCurrentWaypoint().vel.y); };

	void UpdateCurrentWaypointByTarget(primitives::TargetState newTarget, int radius );
	//Coord getTargetPosition() { return pTarget.Position; };
	//primitives::vector getTargetVelocity() { return primitives::vector(pTarget.Velocity.x,pTarget.Velocity.y); };
	//double getTargetHeading() { return pTarget.Heading; };
	//double getHeadingToTarget(Coord vesselPosition);

	
private:
	
	//referenceModel vesselRefModel;
	//TargetState pTarget;
	int currentWaypoint;
	int headingRotations;
	Angle headingToWaypoint;
	Angle headingToWaypoint_previous;
	double distanceToWaypoint;
	double distanceToWaypoint_previous;
	double distanceToWaypointRate;
	double headingToWaypointRate;
	std::vector<primitives::waypoint> waypoints;

	

	//bool isHeadingDependent;

};