#include"DoorModule.h"
#include"ModuleEntity.h"

DoorModule::DoorModule()
{


}

DoorModule::DoorModule(std::shared_ptr<GraphicalEntity> graphicalentity, string name) : ModuleEntity(graphicalentity, name)
{
	Initialize();
}

bool DoorModule::PointIsInTriggerRegion(const Coord& pos)
{
	return geometry::CoordWIthinRectangle(this->GetTriggerRegion(), pos);
}

void DoorModule::Initialize()
{
	pDoorActive = true;
	pDoorType = enDoorType::dtSlideUp;
	pState = enDoorState::dStateClosed;
	pDoorVelocity = 5;
	pDoorDirection = 1;
	pAccumulatedMovement = primitives::vector();
	pTriggerRegion = { 0,0,0,0 };
	AutoGenerateTriggerRegion();		//might not need to be here
	pWaitUntilClose = 3000;
	pTimeCounter = 0;
}

void DoorModule::move(Uint32 milliseconds)
{
	ModuleEntity::move(milliseconds);

	this->pAccumulatedMovement += this->GetLastChangeInPosition();
	
	switch (this->pDoorType)
	{
	case(enDoorType::dtSlideRight):
		if (std::abs(this->pAccumulatedMovement.x) == this->getDimension().width())
			StopDoorMovement();
		break;
	case(enDoorType::dtSlideLeft):
		if (std::abs(this->pAccumulatedMovement.x) == this->getDimension().width())
			StopDoorMovement();
		break;
	case(enDoorType::dtSlideDown):
		if (std::abs(this->pAccumulatedMovement.y) == this->getDimension().height())
			StopDoorMovement();
		break;
	case(enDoorType::dtSlideUp):
		if (std::abs(this->pAccumulatedMovement.y) == this->getDimension().height())
			StopDoorMovement();
		break;
	default:
		break;
	}

	//if (this->pAccumulatedMovement)

}

void DoorModule::StopDoorMovement()
{
	switch (this->pState)
	{
	case(enDoorState::dStateClosed):
		break;
	case(enDoorState::dStateClosing):
		SetState(enDoorState::dStateClosed);
		break;
	case(enDoorState::dStateOpen):
		break;
	case(enDoorState::dStateOpening):
		SetState(enDoorState::dStateOpen);
		break;
	default:
		break;
	}
	this->setVelocity(primitives::vector(0));
}

void DoorModule::SetState(enDoorState state)
{
	switch (this->pState)
	{
	case(enDoorState::dStateClosed):
		break;
	case(enDoorState::dStateClosing):
		break;
	case(enDoorState::dStateOpen):
		break;
	case(enDoorState::dStateOpening):
		break;
	default:
		break;
	}
	this->pState = state;
}

void DoorModule::OpenDoor()
{
	if (this->pState == enDoorState::dStateOpening || this->pState == enDoorState::dStateOpen)
		return;

	this->setVelocity(GetVelocityFromType(this->pDoorType));
	this->SetState(enDoorState::dStateOpening);

}
void DoorModule::CloseDoor()
{
	if (this->pState == enDoorState::dStateClosing || this->pState == enDoorState::dStateClosed)
		return;

	this->setVelocity(GetVelocityFromType(this->pDoorType)*-1);
	this->SetState(enDoorState::dStateClosing);
}

void DoorModule::HandleDoor(int ms)
{
	switch (this->pState)
	{
	case(enDoorState::dStateClosed):
		this->SetTimer(this->GetTimer() + ms);
		if (this->GetTimer() > this->pWaitUntilClose)
		{
			CloseDoor();
			ResetTimer();
		}
		break;
	case(enDoorState::dStateClosing):
		this->move(ms);
		break;
	case(enDoorState::dStateOpen):
		break;
	case(enDoorState::dStateOpening):
		this->move(ms);
		break;
	default:
		break;
	}

}

primitives::vector DoorModule::GetVelocityFromType(enDoorType type)
{
	switch (this->pDoorType)
	{
	case(enDoorType::dtSlideRight):
		return primitives::vector(this->pDoorVelocity, 0);
	case(enDoorType::dtSlideLeft):
		return primitives::vector(-this->pDoorVelocity, 0);
	case(enDoorType::dtSlideDown):
		return primitives::vector(0, this->pDoorVelocity);
	case(enDoorType::dtSlideUp):
		return primitives::vector(0, -this->pDoorVelocity);
	default:
		break;
	}
}

void DoorModule::AutoGenerateTriggerRegion()
{
	int width = 100;
	pTriggerRegion = { this->getPosition().x - width,this->getPosition().y, 2* width, this->getDimension().height() };
}
