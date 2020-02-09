#pragma once
#include"DynamicEntity.h"
#include"Driver.h"

	//static const double MAXTHRUST = 10000;
	//static const double MAXANGULARTHRUST = 99999;//0.5*std::_Pi;

class DynamicEntityControlled : public DynamicEntity
{

public:

	DynamicEntityControlled();
	DynamicEntityControlled( Coord initpos );

	void setGuidanceWaypoint( primitives::waypoint );
	void setTargetState( const primitives::TargetState & );
	void addGuidanceWaypoint( primitives::waypoint );
	void changeGuidanceCurrentWaypoint( primitives::waypoint );
	void setWaypointHeading(double heading);
	void clearWaypoints() { this->pDriver.GetGuidance().clearWaypoints(); };
	//double GetCurrentFinalHeading() { return this->pDriver.GetGuidance().getWaypointFinalHeading(); };

	void ResetGuidanceStates();

	//virtual void setHeading( double newHeading );
	
	void EnableWaypointTracking(bool set) { this->pDriver.SetWaypointTracking(set); }

	void move(Uint32);
	void updateVelocity(Uint32);
	void updateHeading(Uint32 milliseconds) override;
	virtual void UpdateDiscreteHeading(bool masked);

	double GetHeadingFromDiscreteMovement(bool masked) { return this->pDriver.GetDiscreteMovementMap().GetHeadingFromMovement(masked); };

	//virtual void setVesselDrive(EnumEntityDrive set);
	//void setVesselRotate(EnumEntityDrive set) { enEntityDrive.rotate = set; };
	//EntityMovementDrive& GetMovementDrive() { return this->pDriver.GetDiscreteMovementMap(); };
	void KeyboardForward(bool set) { this->pDriver.SetKeyboardForward(set); }
	void KeyboardBackward(bool set) { this->pDriver.SetKeyboardDown(set); }
	void KeyboardLeft(bool set) { this->pDriver.SetKeyboardLeft(set); }
	void KeyboardRight(bool set) { this->pDriver.SetKeyboardRight(set); }
	void KeyboardTurnLeft() { this->pDriver.TurnLeft(); }
	void KeyboardTurnRight() { this->pDriver.TurnRight(); }


	void setControllerGain( double set )				{ this->pDriver.GetController().Gain = set; };
	void setControllerDerivative( double set )			{ this->pDriver.GetController().Derivative = set; };
	void setControllerIntergral( double set )			{ this->pDriver.GetController().Integration = set; };
	void setControllerGainHeading( double set )			{ this->pDriver.GetController().GainHeading = set; };
	void setControllerDerivativeHeading( double set )	{ this->pDriver.GetController().DerivativeHeading = set; };
	void setControllerIntegralHeading( double set )		{ this->pDriver.GetController().IntegrationHeading = set; };

	double getControllerGain( )				{ return this->pDriver.GetController().Gain;};
	double getControllerDerivative( )		{ return this->pDriver.GetController().Derivative; };
	double getControllerIntergral( )		{ return this->pDriver.GetController().Integration; };
	double getControllerGainHeading( )		{ return this->pDriver.GetController().GainHeading; };
	double getControllerDerivativeHeading( ){ return this->pDriver.GetController().DerivativeHeading; };
	double getControllerIntegralHeading( )	{ return this->pDriver.GetController().IntegrationHeading; };

	virtual int GetThrust();
	//primitives::vector GetAppliedThrust() { return currentThrust; };

	void SetDisableDrive();
	void SetRotationDrive(EnumEntityDrive set);
	void SetRotationDrive(EnumEntityDrive set, bool resetStates );
	EnumEntityDrive GetRotationDrive() const { return this->pDriver.GetEnumDriveRotate(); };
	void SetDrive(EnumEntityDrive set );
	void SetDrive(EnumEntityDrive set, bool resetStates);
	EnumEntityDrive GetDrive() const { return this->pDriver.GetEnumDrive(); };

	void SetIteration(DriverIteratonStyle style) { this->pDriver.SetIterationStyle(style); };
	void SetDriveType(DriverControlStyle style) { this->pDriver.SeControlStyle(style); };
	void SetSideslip(bool set) { this->pDriver.SetSideslip(set); };

	bool currentWaypointReached();

	void SetAbsVelocity(double set) { this->pDriver.SetVelocityMagnitude(set); };
	void SetAbsAngularAcceleration(double set) { this->pDriver.SetAngularAccelerationMagnitude(set); };
	void SetAbsAngularVelocity(double set) { this->pDriver.SetAngularVelocityMagnitude(set); };
	double GetAbsAngularAcceleration() { return this->pDriver.GetMaxAngularAcceleration(); };
	double GetAbsVelocity() { return this->pDriver.GetVelocityMagnitude(); };
	double GetAbsAngularVelocity() { return this->pDriver.GetMaxAngularVelocity(); };
	double GetDriverThrust() { return this->pDriver.GetMaxThrust(); };

	bool KeyboardHasMovement() { return this->pDriver.GetDiscreteMovementMap().HasMovement(); };

protected:

	//void Setsideslip(bool value);
	//void SetUseController(bool set);
	//virtual void PIDCalcThrust(int milliseconds);
	
	primitives::vector currentThrust;
	double currentAngularThrust;

	



	//
	Driver pDriver;
	//PID shipController;

	void resetGuidanceReference( Coord newPos );
	void SetReferenceSpeed(double speed) { this->pDriver.SetReferenceSpeed(speed); }
	void SetHeadingReferenceSpeed(double speed) { this->pDriver.SetHeadingReferenceSpeed(speed); }
	//void slideHeading();

	//virtual void TriggerOnDrive(int milliseconds); //find a more suitable location ( and add it in DynamicEntity , should run every iteration the entity is run by integrator, make a run() function ??? )

	virtual void UpdateOutputFromDrive();
	virtual bool vesselHeadingWithinBounds( Coord point, double bounds);
	//virtual void setDriveThrust( primitives::vector set );
	//virtual void setAngleThrust( double set );
	void DriveEntity(int milliseconds);
	virtual bool DriveTriggered(int milliseconds);

private:

	
	//bool pSideslipOnMove;
	//bool pUseController;
	void init();
	
};
