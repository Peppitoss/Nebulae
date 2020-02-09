#pragma once
#include"ShipController.h"
#include"BoundedDouble.h"

enum DriverControlStyle
{
	enAutoControl = 1,
	enManualControl = 2
};

enum DriverIteratonStyle
{
	enContinuously = 1,
	enDiscrete = 2
};

enum EnumEntityDrive
{
	Keyboard,
	DynamicPosition,
	DynamicVelocityHeading,
	DynamicTargetTracking,
	DynamicMissileTargetTracking,
	DynamicMissileBoost,
	DynamicHeading,
	DockedTarget,
	Stable,
	Stationary,
	None
};

struct MovementDirection
{

	MovementDirection();

	void SetEnableValue(bool set) { this->enabled = set; }
	bool IsEnabled() const { return this->enabled; }
	void Disable() { this->enabled = false; }
	void Enable() { this->enabled = true; }

	bool GetMaskedValue() const { return value && enabled; }

	void SetValue(bool set) { value = set; }
	bool GetValue(bool masked) const;

	bool value;
	bool enabled;

};

struct EntityMovementDrive
{

	EntityMovementDrive();
	EntityMovementDrive(bool forward, bool left, bool right, bool backward);

	void SetMovement(bool forward, bool left, bool right, bool backward, bool masked);
	void SetMoveForward(bool);
	void SetMoveBackward(bool);
	void SetMoveLeft(bool);
	void SetMoveRight(bool);
	bool HasMovement() const;

	double GetHeadingFromMovement(bool masked);

	void SetTurnLeft();
	void SetTurnRight();
	void SetNoMovement();
	void SetNoTurn();

	bool Forward() const { return pForward.GetMaskedValue(); }
	bool Left() const { return pLeft.GetMaskedValue(); }
	bool Right() const  { return pRight.GetMaskedValue(); }
	bool Backward() const  { return pBackward.GetMaskedValue(); }

	MovementDirection pForward;
	MovementDirection pLeft;
	MovementDirection pRight;
	MovementDirection pBackward;
	bool Turnleft;
	bool Turnright;

};

//static const double MAXANGULARTHRUST = 99999;//0.5*std::_Pi;

struct DriverOutput
{
	DriverOutput();

	void ResetOutput();

	primitives::vector OutputThrust;
	double OutputAngularThrust;

	primitives::vector OutputVelocity;
	double OutputAngularVelocity;

	primitives::vector OutputAcceleration;
	double OutputAngularAcceleration;

	//double OutputGlobalHeading;
	double OutputUnitHeading;

};

class Driver
{

public:

	Driver( Coord vesselPosition );
	void Drive(int milliseconds, const Coord& position, double heading, double globalheading, const primitives::vector& velocity, double angularVelocity, double mass);
	
	void SetIterationStyle(DriverIteratonStyle set) { this->pIterationStyle = set; }; //DefineDefaultDriveEntity();
	void SetDrive(EnumEntityDrive set, bool resetStates );
	void SetRotateDrive(EnumEntityDrive set, bool resetStates);

	bool IsDiscrete() const { return this->pIterationStyle == enDiscrete; }
	bool IsContinuously() const { return this->pIterationStyle == enContinuously; }

	void SeControlStyle(DriverControlStyle set) { this->pControlStyle = set; } //DefineDefaultDriveEntity(); }
	bool IsAutoControl() const { return this->pControlStyle == enAutoControl; };
	bool IsManualControl() const { return this->pControlStyle == enManualControl; }

	void SetSideslip(bool set) { this->pSideslip = set; } //DefineDefaultDriveEntity(); }
	bool IsSideslip() const { return this->pSideslip; }

	bool DriveIsStable();
	void SetDriveStable();

	void ShutoffDriver();
	void SetInputValues(int mass, const Coord pos, double heading, double globalheading, const primitives::vector& velocity, double angularVelocity );

	EnumEntityDrive GetEnumDrive() const { return this->pDrive; };
	EnumEntityDrive GetEnumDriveRotate() const { return this->pRotate; };

	void SetKeyboardMovement(bool forward, bool left, bool right, bool backward, bool masked);
	EntityMovementDrive& GetDiscreteMovementMap() { return this->pMovementMap; };

	void SetKeyboardRight(bool value);
	void SetKeyboardDown(bool value);
	void SetKeyboardLeft(bool value);
	void SetKeyboardForward(bool value);
	void TurnLeft();
	void TurnRight();
	DriverOutput& Output() { return pOutputData; };

	void RunController(int milliseconds);

	bool IsWaypointTracking() const { return this->pFollowWaypoint; };
	void SetWaypointTracking(bool set) { this->pFollowWaypoint = set; };
	bool HasWaypoints() const { return this->GuidanceBox.waypointCount() > 0; };

	void ResetReferenceModel(const Coord& newPos, double newHeading, double angularvelocity);
	void SetReferenceSpeed(double set) { this->pReferenceModel.SetReferenceGeneratorRate(set); };
	void SetHeadingReferenceSpeed(double set) { this->pReferenceModel.SetReferenceGeneratorHeadingRate(set); };

	Guidance& GetGuidance() { return this->GuidanceBox; };
	PID& GetController() { return this->shipController; };




	void SetVelocityMagnitude(double set) { this->pReferenceModel.GetData().VelocityMagnitudeSetPoint = set; }
	void SetAngularAccelerationMagnitude(double set) { this->pReferenceModel.GetData().AngularAccelerationSetPoint = set; }
	void SetAngularVelocityMagnitude( double set ) { this->pReferenceModel.GetData().VelocityAngularHeadingSetPoint = set; }
	void SetMaxAngularThrust(int value) { this->pMaxAngularThrust = value; }
	void SetMaxThrust(int value) { this->pMaxThrust = value; }

	double GetMaxAngularAcceleration() { return this->pReferenceModel.GetData().AngularAccelerationSetPoint; };
	double GetVelocityMagnitude() { return this->pReferenceModel.GetData().VelocityMagnitudeSetPoint; };
	double GetMaxAngularVelocity() { return this->pReferenceModel.GetData().VelocityAngularHeadingSetPoint; };
	double GetMaxAngularThrust() const { return this->pMaxAngularThrust; }
	double GetMaxThrust() const { return this->pMaxThrust; }

private:

	void Initialize();

	void DriveAutomatic(int milliseconds);
	void DriveManual(int milliseconds);
	void DriveDefault(int milliseconds);
	void DriveDefaultHeading(int milliseconds);

	void DriveRotateAutomatic(int milliseconds);
	void DriveRotateManual(int milliseconds);

	void driveUpdateForDynamicPositioning(int milliseconds);
	void driveUpdateForDynamicPositioningHeading(int milliseconds);
	void driverUpdateForDynamicHeading(int milliseconds);

	void driveUpdateForDynamicMissileTargetTracking(int milliseconds);
	void driveUpdateForDynamicMissileBoost(int milliseconds);
	bool driveUpdateForMissile(int milliseconds);
	void driveUpdateForDynamicTargetTracking(int milliseconds);
	void SetupDPDrive(int milliseconds);

	void driveStableHeading(int milliseconds);
	void driveStable();

	bool pSideslip;
	DriverControlStyle pControlStyle;
	DriverIteratonStyle pIterationStyle;

	EnumEntityDrive pDrive;
	EnumEntityDrive pRotate;
	EntityMovementDrive pMovementMap;
	
	EnumEntityDrive defaultValue;
	DriverOutput pOutputData;
	ReferenceModel pReferenceModel;

	PID shipController;
	Guidance GuidanceBox;
	
	Coord GetReadPosition() { return this->pPositionInput; };
	double GetReadMass() { return this->pMassInput; };
	double GetReadHeading() { return this->pHeadingInput; };
	double GetReadGlobalHeading() { return this->pGlobalHeadingInput; };
	double GetReadAngularVelocity() { return this->pAngularVelocityInput; }
	primitives::vector GetReadVelocity() { return this->pVelocityInput; }

	double pHeadingInput; //ref
	double pGlobalHeadingInput;
	Coord pPositionInput; //ref
	double pMassInput; //ref
	primitives::vector pVelocityInput;
	double pAngularVelocityInput;

	double pMaxThrust;
	double pMaxAngularThrust;

	bool pFollowWaypoint;
	void SlideHeading(double compareToAngle );

	void CalculateDiscreteMovement();
	void PIDCalcThrust(const Coord& position, const primitives::vector& velocity, double heading, double angularVelocity, int milliseconds);
	void SetDriveThrust(primitives::vector set);
	void SetAngleThrust(double angleThrust);

	void driveUpdate(int milliseconds);
	void driveUpdateForward(int milliseconds);
	void driveUpdateBackward(int milliseconds);
	void driveUpdateLeft(int milliseconds);
	void driveUpdateRight(int milliseconds);
	void driveUpdateToDirection(int milliseconds, double speedModifier, double heading);
	void driveTurnUpdate(int milliseconds);
	void driveTurnLeft(int milliseconds);
	void driveTurnRight(int milliseconds);

	void UpdateManualDriveState();

	void SetRotatingEntityDrive(EnumEntityDrive set);
	void SetNormalEntityDrive(EnumEntityDrive set);

	Coord GetCurrentWaypointCoord() { return this->GuidanceBox.getCurrentWaypointCoord(); };

};