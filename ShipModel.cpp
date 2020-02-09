#include"ShipModel.h"

ShipModel::ShipModel(string shipName, int ID, const Coord& initPos)
{
	this->Initialize();
	this->pShipName = shipName;
	if (ID == PlayerModelID)
		throw new std::exception("Error ID -1 is reserved for player");
	this->pID = ID;
	this->pCurrentPosition = initPos;
	
}

ShipModel::ShipModel(string shipName, int ID, const Coord& initPos, double heading )
{
	this->Initialize();
	this->pShipName = shipName;
	if (ID == PlayerModelID)
		throw new std::exception("Error ID -1 is reserved for player");
	this->pID = ID;
	this->pCurrentPosition = initPos;
	this->pKinematics.CurrentHeading = heading;
	
}


ShipModel::ShipModel( const ShipModel& other )
{
	this->pShipName = other.pShipName;
	//this->pRole = other.pRole;
	//this->pTargetPosition = other.pTargetPosition;
	//this->isMovable = other.isMovable;
	//this->pHandled = other.pHandled;
	this->pID = other.pID;
	//this->pHasOrder = other.pHasOrder;
	this->pBehaviour = other.pBehaviour;
	this->pAllegiance = other.pAllegiance;
	//this->pShipLinks = other.pShipLinks;
	this->pIsInitiallyActive = other.pIsInitiallyActive;
	this->pKinematics = other.pKinematics;
	this->pCurrentPosition = other.pCurrentPosition;
	this->pExternalEntityKinematics = other.pExternalEntityKinematics;
}
ShipModel::ShipModel( ShipModel&& other )
{
	this->pShipName = std::move(other.pShipName);
	//this->pRole = std::move(other.pRole);

	//this->pTargetPosition = std::move(other.pTargetPosition);
	//this->isMovable = std::move(other.isMovable);
	//this->pHandled = std::move(other.pHandled);
	this->pID = std::move(other.pID);
	//this->pHasOrder = std::move(other.pHasOrder);
	this->pBehaviour = std::move(other.pBehaviour);
	this->pAllegiance = std::move(other.pAllegiance);
	//this->pShipLinks = std::move(other.pShipLinks);
	this->pKinematics = std::move(other.pKinematics);
	this->pIsInitiallyActive = std::move(other.pIsInitiallyActive);
	this->pCurrentPosition = std::move(other.pCurrentPosition);
	this->pExternalEntityKinematics = std::move(other.pExternalEntityKinematics);

}

ShipModel& ShipModel::operator=(const ShipModel& other)
{   
	this->pShipName = other.pShipName;
	//this->pRole = other.pRole;

	//this->pTargetPosition = other.pTargetPosition;
	//this->isMovable = other.isMovable;
	//this->pHandled = other.pHandled;
	this->pID = other.pID;
	//this->pHasOrder = other.pHasOrder;
	this->pBehaviour = other.pBehaviour;
	this->pAllegiance = other.pAllegiance;
	//this->pShipLinks = other.pShipLinks;
	this->pKinematics = other.pKinematics;
	this->pIsInitiallyActive = other.pIsInitiallyActive;
	this->pCurrentPosition = other.pCurrentPosition;
	this->pExternalEntityKinematics = other.pExternalEntityKinematics;
	return *this;
}

ShipModel& ShipModel::operator=(const ShipModel&& other)
{   
	this->pShipName = std::move(other.pShipName);
	//this->pRole = std::move(other.pRole);
	//this->pTargetPosition = std::move(other.pTargetPosition);
	//this->isMovable = std::move(other.isMovable);
	//this->pHandled = std::move(other.pHandled);
	this->pID = std::move(other.pID);
	//this->pHasOrder = std::move(other.pHasOrder);
	this->pBehaviour = std::move(other.pBehaviour);
	this->pAllegiance = std::move(other.pAllegiance);
	//this->pShipLinks = std::move(other.pShipLinks);
	this->pIsInitiallyActive = std::move(other.pIsInitiallyActive);
	this->pKinematics = std::move(other.pKinematics);
	this->pCurrentPosition = std::move(other.pCurrentPosition);
	this->pExternalEntityKinematics = std::move(other.pExternalEntityKinematics);
	return *this;
}

void ShipModel::Initialize()
{
	/*pTargetPosition = primitives::waypoint(Coord(0),1);
	pHandled = false;
	isMovable = true;*/
	pIsInitiallyActive = true;
	//pRole = enShipRole::eFighter;
	pBehaviour = BehaviorTree();
	pAllegiance = enumAllegiance::aENEMY;
	pCurrentPosition = Coord(0);
	pExternalEntityKinematics = std::map<std::string, Kinematics>();
}

bool ShipModel::operator<(const ShipModel& other) const
{
	return this->GetID() < other.GetID();
}

void ShipModel::SetCurrentPosition( const Coord set )
{
	this->pCurrentPosition = set;
}
Coord ShipModel::GetCurrentPosition() const
{
	return this->pCurrentPosition;
}

//primitives::waypoint ShipModel::GetTargetPosition() const
//{
//	return this->pTargetPosition;
//}
//
//void ShipModel::SetShipOrder( bool set )
//{
//	this->pHasOrder = set;
//}

//enOrderType ShipModel::GetOrderType(std::default_random_engine& rengine)
//{
//	ShipModelProbabilitySet orderset = ShipModelProbabilitySet();
//	switch( this->GetShipRole() )
//	{
//	case(enShipRole::eFighter) :
//		orderset = FighterSet();
//		return orderset.GetOrderType( rengine );
//	case(enShipRole::eTrader) :
//		orderset = TraderSet();
//		return orderset.GetOrderType( rengine );
//	default:
//		return enOrderType::oTrade;
//	}
//
//}

//bool ShipModel::SetNewPosition()
//{
//	if( this->GetHandled() )
//		return false;
//
//	this->SetHandled( true );
//
//	Coord difference = this->TargetLocation() - this->GetCurrentPosition();
//
//	if( difference == Coord(0) )
//	{
//		this->pHasOrder = false;
//		return false;
//	}
//
//	Coord moveDirection = Coord(0);
//
//	if( difference.x == 0 )
//		moveDirection = Coord(0,1)*geometry::Sign(difference.y);
//	else if( difference.y == 0 )
//		moveDirection = Coord(1,0)*geometry::Sign(difference.x);
//	else if( std::abs(difference.x) > std::abs(difference.y) ) 
//	{
//		moveDirection = Coord(1,0)*geometry::Sign(difference.x);
//	}
//	else
//	{
//		moveDirection = Coord(0,1)*geometry::Sign(difference.y);
//	}
//
//	this->pCurrentPosition += moveDirection;
//
//	return true;
//}

//bool ShipModel::IsFinishedMoving()
//{
//	return this->TargetLocation() == this->GetCurrentPosition();
//}

//void ShipModel::SetTargetLocation( const primitives::waypoint loc )
//{
//	this->pTargetPosition = loc;
//	this->pHasOrder = true;
//}