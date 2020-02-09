#include"Driver.h"

MovementDirection::MovementDirection()
{
	value = false;
	enabled = true;
}

bool MovementDirection::GetValue(bool masked) const
{
	if (masked)
		return this->GetMaskedValue();
	return value;
};


EntityMovementDrive::EntityMovementDrive(bool forward, bool left, bool right, bool backward)
{
	pForward.SetValue(forward);
	pLeft.SetValue(left);
	pRight.SetValue(right);
	pBackward.SetValue(backward);
	Turnleft = false;
	Turnright = false;
}

EntityMovementDrive::EntityMovementDrive()
{
	pForward.SetValue(false);
	pLeft.SetValue(false);
	pRight.SetValue(false);
	pBackward.SetValue(false);
	Turnleft = false;
	Turnright = false;
	//defaultValue = EnumEntityDrive::Stable;
}

void EntityMovementDrive::SetMoveForward(bool value)
{
	pForward.SetValue(value);
	if (value)
		pBackward.SetValue(false);
	//else if (!HasMovement())
	//	drive = defaultValue;

}
void EntityMovementDrive::SetMoveBackward(bool value)
{
	pBackward.SetValue(value);
	if (value)
		pForward.SetValue(false);
	//else if (!HasMovement())
	//	drive = defaultValue;
}
void EntityMovementDrive::SetMoveLeft(bool value)
{
	pLeft.SetValue(value);
	if (value)
		pRight.SetValue(false);
	//else if (!HasMovement())
	//	drive = defaultValue;
}
void EntityMovementDrive::SetMoveRight(bool value)
{
	pRight.SetValue(value);
	if (value)
		pLeft.SetValue(false);
	//else if (!HasMovement())
	//	drive = defaultValue;
}

bool EntityMovementDrive::HasMovement() const
{
	return Forward() || Backward() || Left() || Right();
}

double EntityMovementDrive::GetHeadingFromMovement(bool masked)
{
	if (this->pForward.GetValue(masked))
	{
		if (this->pLeft.GetValue(masked))
			return 0.785;
		else if (this->pRight.GetValue(masked))
			return -0.785;
		else
			return 0;
	}
	else if (this->pBackward.GetValue(masked))
	{
		if (this->pLeft.GetValue(masked))
			return -0.785 + 3.14;
		else if (this->pRight.GetValue(masked))
			return 0.785 + 3.14;
		else
			return 3.14;
	}
	else
	{
		if (this->pLeft.GetValue(masked))
			return 1.57;
		else if (this->pRight.GetValue(masked))
			return -1.57;
	}
	return 0;
}

void EntityMovementDrive::SetMovement(bool forward, bool left, bool right, bool backward, bool masked)
{
	if (masked == false)
	{
		pForward.SetValue(forward);
		pLeft.SetValue(left);
		pRight.SetValue(right);
		pBackward.SetValue(backward);
	}
	else
	{
		pForward.SetEnableValue(!forward);
		pLeft.SetEnableValue(!left);
		pRight.SetEnableValue(!right);
		pBackward.SetEnableValue(!backward);
	}
	SetNoTurn();
}
void EntityMovementDrive::SetTurnLeft()
{
	Turnleft = true;
	Turnright = false;
}
void EntityMovementDrive::SetTurnRight()
{
	Turnleft = false;
	Turnright = true;
}
void EntityMovementDrive::SetNoMovement()
{
	pForward.SetValue(false);
	pLeft.SetValue(false);
	pRight.SetValue(false);
	pBackward.SetValue(false);
}
void EntityMovementDrive::SetNoTurn()
{
	Turnleft = false;
	Turnright = false;
}



Driver::Driver(Coord vesselPosition) : pReferenceModel(vesselPosition)
{
	this->Initialize();
}

void Driver::SetRotatingEntityDrive(EnumEntityDrive set )
{
	this->pDrive = set;
}
void Driver::SetNormalEntityDrive(EnumEntityDrive set)
{
	this->pRotate = set;
}

void Driver::SetKeyboardMovement(bool forward, bool left, bool right, bool backward, bool masked)
{
	pMovementMap.SetMovement(forward, left, right, backward, masked);
	UpdateManualDriveState();
}

void Driver::SetKeyboardRight(bool value)
{
	pMovementMap.SetMoveRight(value);
	UpdateManualDriveState();
}
void Driver::SetKeyboardDown(bool value)
{
	pMovementMap.SetMoveBackward(value);
	UpdateManualDriveState();
}
void Driver::SetKeyboardLeft(bool value)
{
	pMovementMap.SetMoveLeft(value);
	UpdateManualDriveState();
}
void Driver::SetKeyboardForward(bool value)
{
	pMovementMap.SetMoveForward(value);
	UpdateManualDriveState();
}

void Driver::TurnLeft()
{
	pMovementMap.SetTurnLeft();
}
void Driver::TurnRight()
{
	pMovementMap.SetTurnRight();
}

void Driver::UpdateManualDriveState()
{
	if (IsManualControl())
	{
		if (this->pMovementMap.HasMovement())
			this->pDrive = EnumEntityDrive::Keyboard;
		else
			this->pDrive = EnumEntityDrive::Stable;
	}
}

bool Driver::DriveIsStable()
{
	return this->pDrive == EnumEntityDrive::Stable && this->pRotate == EnumEntityDrive::Stable;
}

void Driver::SetDriveStable()
{
	this->pDrive = EnumEntityDrive::Stable;
	this->pRotate = EnumEntityDrive::Stable;
	this->pOutputData.ResetOutput();
}

void Driver::Initialize()
{
	SetDriveStable();
	shipController.Disable();
	this->pOutputData.ResetOutput();
	pFollowWaypoint = false;
	pReferenceModel.resetReferenceModel();
	pMaxThrust = 1000000;
	pMaxAngularThrust = 1000000;

	pHeadingInput = 0;
	pGlobalHeadingInput = 0;
	pPositionInput = Coord(0);
	pMassInput = 0;
	pVelocityInput = Coord(0);
	pAngularVelocityInput = 0;

	//pDataCollector = std::vector<double>();
}

void Driver::ShutoffDriver()
{
	SetDriveStable();
	shipController.Disable();
}

void Driver::PIDCalcThrust(const Coord& position, const primitives::vector& velocity, double heading, double angularVelocity, int milliseconds)
{
	if (GetEnumDrive() == EnumEntityDrive::DynamicPosition && this->IsWaypointTracking() )//!this->GuidanceBox.allWaypointsReached())
	{
		if (shipController.driveControlisTuned())
			this->SetDriveThrust(this->shipController.calcNewThrustForPositionalRef(position, velocity));
		if (shipController.headingControlisTuned())
			this->SetAngleThrust(this->shipController.calcNewThrustForPositionalHeadingRef(heading, angularVelocity));
		return;
	}
	else if (GetEnumDrive() == EnumEntityDrive::DynamicVelocityHeading || GetEnumDrive() == EnumEntityDrive::DynamicTargetTracking || GetEnumDrive() == EnumEntityDrive::DynamicMissileTargetTracking)
	{
		if (shipController.headingControlisTuned()) //&& GuidanceBox.isWaypointHeadingDependenent() )
			this->SetAngleThrust(this->shipController.calcNewThrustForPositionalHeadingRef(heading, angularVelocity));
		if (shipController.driveControlisTuned())
			this->SetDriveThrust(this->shipController.calcNewThrustForVelocityRef(velocity));
		return;
	}
	else if (GetEnumDrive() == EnumEntityDrive::None)
	{
	}
	else
	{
		if (shipController.driveControlisTuned())
			this->SetDriveThrust(this->shipController.calcNewThrustForVelocityRef(velocity));
	}

	if (GetEnumDriveRotate() == EnumEntityDrive::DynamicHeading)
	{
		if (shipController.headingControlisTuned()) //&& GuidanceBox.isWaypointHeadingDependenent() )W
			this->SetAngleThrust(this->shipController.calcNewThrustForPositionalHeadingRef(heading, angularVelocity));
		
		//below will also work, 
		//this->SetAngleThrust(this->shipController.calcNewThrustForVelocityHeadingRef(angularVelocity));
		//this->SetAngleThrust(this->shipController.calcNewThrustForPositionalHeadingRef(heading, angularVelocity));
	}
	else if (shipController.headingControlisTuned())
	{
		this->SetAngleThrust(this->shipController.calcNewThrustForVelocityHeadingRef(angularVelocity));
	}
}

void Driver::CalculateDiscreteMovement()
{

}

void Driver::RunController(int milliseconds)
{
	if (this->IsContinuously())
		this->PIDCalcThrust( this->GetReadPosition(), this->GetReadVelocity(),this->GetReadGlobalHeading(), this->GetReadAngularVelocity(), milliseconds);
	else
		CalculateDiscreteMovement();
}

void Driver::SetInputValues(int mass, const Coord position, double heading, double globalHeading, const primitives::vector& velocity, double angularVelocity )
{
	pHeadingInput = heading;
	pGlobalHeadingInput = globalHeading;
	pPositionInput = position;
	pMassInput = mass;
	pVelocityInput = velocity;
	pAngularVelocityInput = angularVelocity;
}

void Driver::Drive(int milliseconds, const Coord& position, double heading, double globalHeading, const primitives::vector& velocity, double angularVelocity, double mass)
{
	SetInputValues(mass, position, heading, globalHeading, velocity, angularVelocity);

	if (this->pControlStyle == enAutoControl)
	{
		DriveAutomatic(milliseconds);
		DriveRotateAutomatic(milliseconds);
	}
	else
	{
		DriveManual (milliseconds);
		DriveRotateManual(milliseconds);
	}
	this->Output().OutputUnitHeading = this->GetReadHeading();	// in case of slid3heading

}

void Driver::DriveDefaultHeading(int milliseconds)
{
	switch (this->pRotate)
	{
	case EnumEntityDrive::Stable:
		driveStableHeading(milliseconds);
		break;
	case EnumEntityDrive::None:
		break;
	default:
		break;
	}
}

void Driver::DriveDefault(int milliseconds)
{
	switch (pDrive)
	{
	case EnumEntityDrive::Stable:
		driveStable();
		break;
	case EnumEntityDrive::Stationary:
		pOutputData.OutputThrust = primitives::vector(0, 0);
		pOutputData.OutputVelocity = primitives::vector(0, 0);
		break;
	case EnumEntityDrive::None:
		break;
	}
}

void Driver::DriveAutomatic(int milliseconds)
{
	if (this->pFollowWaypoint == false || this->HasWaypoints() == false)
		return;

	switch (pDrive)
	{
	case EnumEntityDrive::DynamicPosition:
		driveUpdateForDynamicPositioning(milliseconds);
		return;
	case EnumEntityDrive::DynamicVelocityHeading:
		driveUpdateForDynamicPositioningHeading(milliseconds);
		return;
	case EnumEntityDrive::DynamicTargetTracking:
			driveUpdateForDynamicTargetTracking(milliseconds);
		return;
	case EnumEntityDrive::DynamicMissileTargetTracking:
		driveUpdateForDynamicMissileTargetTracking(milliseconds);
		break;
	case EnumEntityDrive::DynamicMissileBoost:
		driveUpdateForDynamicMissileBoost(milliseconds);
		break;
	case EnumEntityDrive::DockedTarget:
		pOutputData.OutputVelocity = this->GuidanceBox.getCurrentWaypointVelocity();
		return;
	default:
		DriveDefault(milliseconds);
	}
	
}

void Driver::DriveManual(int milliseconds)
{
	if (pMovementMap.HasMovement() == false)
		pDrive = EnumEntityDrive::Stable;
	else
		pDrive = EnumEntityDrive::Keyboard;

	switch (pDrive)
	{
	case EnumEntityDrive::Keyboard:
		driveUpdate(milliseconds);
		break;
	default:
		DriveDefault(milliseconds);
	}
}

void Driver::DriveRotateAutomatic(int milliseconds)
{
	switch (pRotate)
	{
	case EnumEntityDrive::Keyboard:
		//generate movement based on waypoint here check sideslip ----------------------- -- - - - - - 
			this->driveTurnUpdate(milliseconds);
		break;
	default:
		DriveDefaultHeading(milliseconds);
	}
}
void Driver::DriveRotateManual(int milliseconds)
{
	switch (pRotate)
	{
	case EnumEntityDrive::Keyboard:
		this->driveTurnUpdate(milliseconds);
		break;
	case EnumEntityDrive::DynamicHeading:
		this->driverUpdateForDynamicHeading(milliseconds);
		break;
	default:
		DriveDefaultHeading(milliseconds);
	}
}

void Driver::driveStableHeading(int milliseconds)
{
	if (this->shipController.headingControlisTuned())
		this->shipController.setVelocityHeadingSetPoint(0);
	else
		this->pOutputData.OutputAngularThrust = 0;
}

void Driver::SetDrive(EnumEntityDrive set, bool resetStates)
{
	this->pDrive = set;
	if (resetStates)
	{
		this->ResetReferenceModel(this->GetReadPosition(), this->GetReadGlobalHeading(), this->GetReadAngularVelocity() );//this->getGlobalHeading());
		this->shipController.resetError();
	}
}

void Driver::SetRotateDrive(EnumEntityDrive set, bool resetStates)
{
	this->pRotate = set;
	if (resetStates)
	{
		this->ResetReferenceModel(this->GetReadPosition(), this->GetReadGlobalHeading(), this->GetReadAngularVelocity());//this->getGlobalHeading());
		this->shipController.resetError();
	}
}


void Driver::driveStable()
{
	if (this->shipController.driveControlisTuned())
		this->shipController.setVelocitySetPoint(primitives::vector(0, 0));
	else
		pOutputData.OutputVelocity = primitives::vector(0, 0);
 }

void Driver::SetDriveThrust(primitives::vector set)
{
	set.xANDyLimit(this->GetMaxThrust());
	this->pOutputData.OutputThrust = set;
}

void Driver::SetAngleThrust(double angleThrust)
{
	angleThrust = primitives::limitDouble(angleThrust, this->GetMaxAngularThrust());
	this->pOutputData.OutputAngularThrust = angleThrust;
}

void Driver::SetupDPDrive(int milliseconds)
{
	if (this->GuidanceBox.currentWaypointReached(this->GetReadPosition()))
		this->GuidanceBox.setNextWaypoint();

	this->GuidanceBox.updateHeadingToWaypoint(this->GetReadPosition(), milliseconds);
}

void Driver::driveUpdateForDynamicPositioning(int milliseconds)
{
	SetupDPDrive(milliseconds);

	if (this->IsContinuously())
	{
		if (shipController.headingControlisTuned())
		{
			this->pReferenceModel.GenerateRefValues(this->GuidanceBox.getHeadingToWaypoint(), this->GetReadMass(), milliseconds);

			this->shipController.setPositionalHeadingSetPoint(this->pReferenceModel.getPosHeadFromRefModel());
			this->SlideHeading(this->GuidanceBox.getHeadingToWaypoint());

		}
		if (this->shipController.driveControlisTuned())
		{
			if(!this->GuidanceBox.allWaypointsReached())
				this->pReferenceModel.GenerateRefValues(this->GetCurrentWaypointCoord(), this->GetReadMass(), milliseconds);
			else
				this->pReferenceModel.GenerateRefValues(this->GetReadPosition(), this->GetReadMass(), milliseconds);
			this->shipController.setPositionSetPoint(this->pReferenceModel.getPosFromRefModel());
		}
		else
			this->pOutputData.OutputThrust = primitives::vector(0, 0);
	}
	else
	{
		this->shipController.setPositionSetPoint(this->GetCurrentWaypointCoord());
	}
}



void Driver::driveUpdateForDynamicPositioningHeading(int milliseconds)
{
	SetupDPDrive(milliseconds);

	if (this->IsContinuously())
	{
		if (shipController.headingControlisTuned())
		{
			this->pReferenceModel.GenerateRefValues(this->GuidanceBox.getHeadingToWaypoint(), this->GetReadMass(), milliseconds);
			
			this->shipController.setPositionalHeadingSetPoint(this->pReferenceModel.getPosHeadFromRefModel());
			this->SlideHeading(this->GuidanceBox.getHeadingToWaypoint());
			
		}
		if (this->shipController.driveControlisTuned())
		{
			this->pReferenceModel.GenerateRefValues(this->GetCurrentWaypointCoord(), this->GetReadMass(), milliseconds);
			this->shipController.setVelocitySetPoint(geometry::rotateVector(primitives::vector(0, this->pReferenceModel.GetData().VelocityMagnitudeSetPoint), this->GetReadHeading())); //get read heading should update value if slideheading was run??
		}
	}
	else
	{
		this->shipController.setPositionSetPoint(this->GetCurrentWaypointCoord());
		
		this->shipController.setPositionalHeadingSetPoint(this->GetGuidance().getHeadingToWaypoint());
		this->SlideHeading(this->GetGuidance().getHeadingToWaypoint());
		 //need this?? //don't think this will work, slide heading has to work with headingtowaypoint
	}
}

void Driver::driverUpdateForDynamicHeading(int milliseconds)
{
	if (this->IsContinuously())
	{
		if (shipController.headingControlisTuned())
		{
			this->pReferenceModel.GenerateRefValues(this->GuidanceBox.getWaypointFinalHeading(), this->GetReadMass(), milliseconds);
			this->shipController.setPositionalHeadingSetPoint(this->pReferenceModel.getPosHeadFromRefModel());// );
			this->shipController.setVelocityHeadingSetPoint(this->pReferenceModel.getVelHeadFromRefModel());
			this->SlideHeading(this->GuidanceBox.getWaypointFinalHeading());
			//this->GuidanceBox.getWaypointFinalHeading() = 
		}
	}
}

void Driver::driveUpdateForDynamicTargetTracking(int milliseconds)
{
	double K = 20;
	double deltaDistanceToTargetReal = K;
	double errorToTarget = 1;

	primitives::vector targetHeading = this->GuidanceBox.getCurrentWaypointVelocity();
	primitives::vector orthogonal = geometry::OrthogonalVector(targetHeading);
	targetHeading.y *= -1;
	targetHeading.x *= -1;

	Coord losC = this->GetReadPosition() - this->GuidanceBox.getCurrentWaypointCoord();
	primitives::vector los = primitives::vector(losC.x, losC.y);//AngleToVector(GuidanceBox.getHeadingToTarget(this->getPosition()));

	//double errorToTarget = geometry::ProjectVectorOntoVector(los, orthogonal)*-1;
	deltaDistanceToTargetReal = geometry::ProjectVectorOntoVector(los, targetHeading);
	double deltaDistanceToTargetAbs = std::abs(deltaDistanceToTargetReal);

	double test1 = geometry::vectorHeading(targetHeading);// - std::_Pi;
	test1 = geometry::radLimit2PI(test1);
	double test2 = std::atan(errorToTarget / (deltaDistanceToTargetAbs + 0.01));
	double headingRef = test1 + test2; //see fossen lookahead based steering

	if (this->IsContinuously())
	{
		if (shipController.headingControlisTuned())
		{
			this->pReferenceModel.GenerateRefValues(headingRef, this->GetReadMass(), milliseconds);

			this->shipController.setVelocityHeadingSetPoint(this->pReferenceModel.getVelHeadFromRefModel()); //errorToTarget/100
			
			this->shipController.setPositionalHeadingSetPoint(this->pReferenceModel.getPosHeadFromRefModel());//this->GuidanceBox.getPosHeadFromRefModel() );
			SlideHeading(this->GetGuidance().getHeadingToWaypoint());
			
		}
		if (this->shipController.driveControlisTuned())
		{
			//this->pReferenceModel.GenerateRefValues(this->GetReadPosition(), this->GetReadMass(), milliseconds);
			this->shipController.setVelocitySetPoint(geometry::rotateVector(primitives::vector(0, -this->GuidanceBox.getCurrentWaypointVelocity().ABS() - deltaDistanceToTargetReal / (50 + 100 * std::abs(GuidanceBox.getHeadingToWaypointRate()))), this->GetReadHeading())); //+this->GuidanceBox.getDistanceToWaypointRate() -std::abs(errorToTarget)
		}
	}
	else
	{
		this->shipController.setPositionalHeadingSetPoint(headingRef);
		this->shipController.setPositionSetPoint(GetCurrentWaypointCoord());
	}
}

bool Driver::driveUpdateForMissile(int milliseconds)
{
	if (shipController.headingControlisTuned())
	{
		this->GuidanceBox.updateHeadingToWaypoint(this->GetReadPosition(), milliseconds);
		return true;
	}
	return false;
}

void Driver::driveUpdateForDynamicMissileTargetTracking(int milliseconds)
{
	if (driveUpdateForMissile(milliseconds) == false)
		return;

	int distanceToPhaseChange = 1400;
	int distanceToTarget = geometry::realDistance(this->GetReadPosition(), this->GetCurrentWaypointCoord());
	
	if (distanceToTarget < distanceToPhaseChange)
	{
		//this->pReferenceModel.SetAngularAccelerationSetPoint(1.5);
		//this->pReferenceModel.setReferenceGeneratorRate(2);
		//this->shipController.setVelocityHeadingSetPoint(3 * this->GuidanceBox.getHeadingToWaypointRate()*this->GuidanceBox.getDistanceToWaypointRate());

		double testData = 3 * this->GuidanceBox.getHeadingToWaypointRate()*this->GuidanceBox.getDistanceToWaypointRate();
		//this->shipController.setVelocityHeadingSetPoint(testData);
		if (std::abs(testData) > 1)
		{
			Coord vLos_c = this->GuidanceBox.getCurrentWaypointCoord() - this->GetReadPosition();
			primitives::vector vLos = primitives::vector(vLos_c.x, vLos_c.y);

			double frameAngle = geometry::vectorHeading(vLos);

			primitives::vector velocityGuidanceFrame = this->GetReadVelocity(); // primitives::vector(this->GetReadVelocity().x, this->GetReadVelocity().y); //this->GetReadVelocity();// getVelocity
			primitives::vector vMissile = geometry::rotateVector(velocityGuidanceFrame, frameAngle*-1);

			vLos.Scale(std::abs(vMissile.x) * 2);

			primitives::vector vNewHeading = vLos - velocityGuidanceFrame;
			double newHeading = geometry::vectorHeading(vNewHeading);

			double currentSetPoint = this->GetReadGlobalHeading(); //this->getGlobalHeading();

			if (currentSetPoint * newHeading < 0)
				newHeading = currentSetPoint + geometry::shortestAngleDifference(newHeading, currentSetPoint);
			
			this->shipController.setPositionalHeadingSetPoint(newHeading);


		}
		//else if(testData < 0)
		//{
		//	int hj = 2;
		//	//this->shipController.setPositionalHeadingSetPoint(this->shipController.getPositionHeadingSetPoint());
		//}
	}
	//if( this->shipController.driveControlisTuned() )
	this->shipController.setVelocitySetPoint( geometry::rotateVector(  primitives::vector( 0, this->pReferenceModel.GetData().VelocityMagnitudeSetPoint ), this->GetReadHeading() ) );
	//this->shipController.setVelocityHeadingSetPoint(this->pReferenceModel.getVelHeadFromRefModel());
	//this->shipController.setPositionalHeadingSetPoint(this->pReferenceModel.getPosHeadFromRefModel());
}

void Driver::driveUpdateForDynamicMissileBoost(int milliseconds)
{
	if (driveUpdateForMissile(milliseconds) == false)
		return;

	//this->pReferenceModel.SetAngularAccelerationSetPoint(0.1);
	//this->pReferenceModel.SetReferenceGeneratorRate(10);
	////this->pReferenceModel.SetVelocityMagnitudeSetPoint(200); //don't change
	//this->pReferenceModel.GenerateRefValues(this->GetGuidance().getHeadingToWaypoint(), this->GetReadMass(), milliseconds);
	//this->shipController.setVelocityHeadingSetPoint(this->pReferenceModel.getVelHeadFromRefModel());
	//this->shipController.setPositionalHeadingSetPoint(this->pReferenceModel.getPosHeadFromRefModel());
}


void Driver::driveUpdate(int milliseconds)
{
	if (pMovementMap.Forward())
	{
		if (pMovementMap.Left())
			driveUpdateToDirection(milliseconds, 1, GetReadHeading() + 0.785);
		else if (pMovementMap.Right())
			driveUpdateToDirection(milliseconds, 1, GetReadHeading() - 0.785);
		else
			driveUpdateForward(milliseconds);
	}
	else if (pMovementMap.Backward())
	{
		if (pMovementMap.Left())
			driveUpdateToDirection(milliseconds, -0.333, GetReadHeading() - 0.785);
		else if (pMovementMap.Right())
			driveUpdateToDirection(milliseconds, -0.333, GetReadHeading() + 0.785);
		else
			driveUpdateBackward(milliseconds);
	}
	else
	{
		if (pMovementMap.Left())
			driveUpdateLeft(milliseconds);
		else if (pMovementMap.Right())
			driveUpdateRight(milliseconds);
	}
}

void Driver::driveUpdateForward(int milliseconds)
{
	driveUpdateToDirection(milliseconds, 1, GetReadHeading());
}
void Driver::driveUpdateBackward(int milliseconds)
{
	driveUpdateToDirection(milliseconds, -0.333, GetReadHeading());
}

void Driver::driveUpdateLeft(int milliseconds)
{
	if (this->pSideslip)
		driveUpdateToDirection(milliseconds, 1, GetReadHeading() + 1.57);
}

void Driver::driveUpdateRight(int milliseconds)
{
	if (this->pSideslip)
		driveUpdateToDirection(milliseconds, 1, GetReadHeading() - 1.57);
}

void Driver::driveUpdateToDirection(int milliseconds, double speedModifier, double heading)
{
	if (this->pIterationStyle == DriverIteratonStyle::enDiscrete)
	{
		this->pOutputData.OutputVelocity = geometry::rotateVector(primitives::vector(0, pReferenceModel.GetData().VelocityMagnitudeSetPoint*speedModifier), heading);
	}
	else
	{
		if (this->shipController.driveControlisTuned())
			this->shipController.setVelocitySetPoint(geometry::rotateVector(primitives::vector(0, pReferenceModel.GetData().VelocityMagnitudeSetPoint*speedModifier), heading));
		else
			this->SetDriveThrust(geometry::rotateVector(primitives::vector(0, -pReferenceModel.GetData().ThrustSetPoint*speedModifier), heading));
	}
}

void Driver::driveTurnUpdate(int milliseconds)
{
	if (pMovementMap.Turnleft)
		driveTurnLeft(milliseconds);
	else if (pMovementMap.Turnright)
		driveTurnRight(milliseconds);
}

void Driver::driveTurnLeft(int milliseconds)
{
	if (this->shipController.headingControlisTuned())
		this->shipController.setVelocityHeadingSetPoint(this->pReferenceModel.GetData().VelocityAngularHeadingSetPoint);// this->pReferenceModel.GetData().headingVelRef);
	else
		SetAngleThrust(this->pReferenceModel.GetData().AngularThrustSetPoint );
}
void Driver::driveTurnRight(int milliseconds)
{
	if (this->shipController.headingControlisTuned())
		this->shipController.setVelocityHeadingSetPoint(-this->pReferenceModel.GetData().VelocityAngularHeadingSetPoint);// this->pReferenceModel.GetData().headingVelRef);
	else
		SetAngleThrust(this->pReferenceModel.GetData().AngularThrustSetPoint);
}


void Driver::ResetReferenceModel(const Coord& newPos, double newHeading, double angularvelocity)
{
	this->pReferenceModel.resetReferenceModel(newPos, newHeading, angularvelocity);
}

void Driver::SlideHeading(double compareAngle) //to avoid heading to change from 359 deg to 0 on increment
{
	double CurrentHeadingRef = geometry::radLimit2PI(this->pReferenceModel.getPosHeadFromRefModel()); // geometry::radLimit2PI(this->pReferenceModel.getPosHeadFromRefModel());//);
	//this->pReferenceModel.setPosHeadFromRefModel(CurrentHeadingRef);

	double newGlobalHeading = this->GetReadGlobalHeading();	//unit heading OR GLOBAL  - - - ----------------------------------------------
	double unitGlobalDiff = this->GetReadGlobalHeading() - this->GetReadHeading();
	double difference = abs(CurrentHeadingRef - compareAngle); //this->GuidanceBox.getHeadingToWaypoint());

	bool slide = false;

	while (difference > std::_Pi)
	{
		slide = true;
		if (CurrentHeadingRef - compareAngle < 0)
		{
			newGlobalHeading += geometry::PI_2;
			CurrentHeadingRef += geometry::PI_2;
		}
		else
		{
			newGlobalHeading -= geometry::PI_2;
			CurrentHeadingRef -= geometry::PI_2;
		}

		difference = abs(CurrentHeadingRef - compareAngle);

	}
	if (slide)
	{
		this->pReferenceModel.setPosHeadFromRefModel(CurrentHeadingRef);
		//this->ResetReferenceModel(this->GetReadPosition(), CurrentHeadingRef);
		this->pGlobalHeadingInput = newGlobalHeading;	//both must be set
		this->pHeadingInput = this->pGlobalHeadingInput - unitGlobalDiff;  //both must be set
	}
}

DriverOutput::DriverOutput()
{
	ResetOutput();
}

void DriverOutput::ResetOutput()
{
	OutputThrust = primitives::vector(0);
	OutputAngularThrust = 0;

	OutputVelocity = primitives::vector(0);
	OutputAngularVelocity = 0;

	OutputAcceleration = primitives::vector(0);
	OutputAngularAcceleration = 0;

}