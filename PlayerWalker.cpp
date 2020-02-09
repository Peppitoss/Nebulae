#include"PlayerWalker.h"
#include"WalkerGlobalList.h"
#include"GameManager.h"
#include"SDLKeyboardSyntax.h"

PlayerWalker::PlayerWalker()
{
	initialize();
}

PlayerWalker::PlayerWalker(const Coord& pos)
	: WalkingEntity(pos)
{
	initialize();
}

void PlayerWalker::initialize()
{
	setStateToNormal();
	InitializeHotkeys();
	this->pRotateEntitiesMap = enExternalEntityRotationTag::ErFrame | enExternalEntityRotationTag::ErLauncher | enExternalEntityRotationTag::ErWing | enExternalEntityRotationTag::ErBridge;
}


void PlayerWalker::setStateToNormal()
{
	pState = enumPlayerState::PLAYERPILOT;
	this->SetDriveType(DriverControlStyle::enManualControl);
	this->EnableWaypointTracking(false);
}
bool PlayerWalker::isDestroyed()
{
	return false;
}

void PlayerWalker::RefreshHotKeyList()
{

}

void PlayerWalker::SetModulePlayerOwned()
{

}

void PlayerWalker::HandleLauncherSelection(SDL_Event& e, SDL_Scancode& c)
{


}
void PlayerWalker::HandleKeyButtonUp(SDL_Event& e)
{
	//Adjust the velocity
	SDL_Scancode code = e.key.keysym.scancode;
	if (code == htForward)
		//enEntityDrive.drive = this->IsWalkerState() ? EnumEntityDrive::Stationary : EnumEntityDrive::Stable;
		this->KeyboardForward(false);
	else if (code == htBackward)
		this->KeyboardBackward(false);
	//enEntityDrive.drive = this->IsWalkerState() ? EnumEntityDrive::Stationary : EnumEntityDrive::Stable;
	else if (code == htLeft)
		this->KeyboardLeft(false);
	//enEntityDrive.drive = EnumEntityDrive::Stationary;
		//else
		//	SetRotationDriveToStable();
	else if (code == htRight)
		this->KeyboardRight(false);
	//enEntityDrive.drive = EnumEntityDrive::Stationary;
		//else
		//	SetRotationDriveToStable();
	else if (code == htFire)
		DeFire();
	else if (code == htBlink)
	{
	}
	else if (code == htDelay)
	{
	}
	else if (code == htBoost)
	{
	}

}
void PlayerWalker::HandleKeyButtonDown(SDL_Event& e)
{
	SDL_Scancode code = e.key.keysym.scancode;
	if (code == htForward)
	{
		//this->SetDrive(EnumEntityDrive::Keyboard);
		this->KeyboardForward(true);
		//if (this->enVesselState != enumPlayerState::PLAYERPILOT)
		//	setStateToNormal();
		return;
	}
	if (code == htBackward)
	{
		//this->SetDrive(EnumEntityDrive::Keyboard);
		this->KeyboardBackward(true);
		//if (this->enVesselState != enumPlayerState::PLAYERPILOT)
		//	setStateToNormal();
		return;
	}
	if (code == htLeft)
	{
		/*if (this->IsWalkerState())
		{
		this->setVesselDrive(EnumEntityDrive::Movement);
		enEntityDrive.SetMoveLeft(true);
		}
		else
		{*/
		//this->SetRotationDrive(EnumEntityDrive::Keyboard);
		this->KeyboardLeft(true);
		//if (this->enVesselState != enumPlayerState::PLAYERPILOT)
		//	setStateToNormal();
		//}
		return;
	}
	if (code == htRight)
	{
		//if (this->IsWalkerState())
		//{
		//	this->setVesselDrive(EnumEntityDrive::Movement);
		//	enEntityDrive.SetMoveRight(true);
		//}
		//else
		//{
		//this->SetRotationDrive(EnumEntityDrive::Keyboard);
		this->KeyboardRight(true);
		//if (this->enVesselState != enumPlayerState::PLAYERPILOT)
		//	setStateToNormal();
		//}
		return;
	}
	if (code == htFire)
	{
		Fire();
		return;
	}
	if (code == htBlink)
	{
	}
	if (code == htDelay)
	{
	}
	if (code == htBoost)
	{
	}
	if (SDLKeyBible::IsNumber(e.key.keysym.scancode))
		HandleLauncherSelection(e, e.key.keysym.scancode);

}
void PlayerWalker::HandleMouseUp(SDL_Event& e)
{
	if (e.button.button == htMouseTurn)
	{
		pSteering = enumPlayerSteering::NOSTEERING;
	}
	if (e.button.button == htMouseFire)
	{
		DeFire();
	}

}

void PlayerWalker::handleWalkerState(Uint32 milliseconds)
{
	if (pSteering == enumPlayerSteering::MOUSESTEERING)
	{
		HandleMouseSteering();
	}
	this->HandleUnitState(milliseconds);
}

void PlayerWalker::HandleMouseDown(SDL_Event& e)
{
	if (e.button.button == htMouseTurn)
	{
		pSteering = enumPlayerSteering::MOUSESTEERING;
		//if (this->enVesselState != enumPlayerState::PLAYERPILOT)
		//	setStateToNormal();
	}
	if (e.button.button == htMouseFire)
	{
		Fire();
	}
}
void PlayerWalker::HandleMouseSteering()
{
	int xMouse, yMouse;
	SDL_GetMouseState(&xMouse, &yMouse);

	Coord MousePos = Coord(1 * xMouse, yMouse * -1);
	double newHeading = geometry::angleBetweenTwoPoints(Coord(1 * GameManager::SCREEN_WIDTH  / 2, -1 * GameManager::SCREEN_HEIGHT / 2), MousePos);
	double translateAngle = newHeading - geometry::PI_div2 - this->getUnitHeading();

	bool changeWingAngle = !this->pDriver.GetEnumDrive() == EnumEntityDrive::Keyboard;
	ChangeExternalModuleDirection(this->pRotateEntitiesMap & enExternalEntityRotationTag::ErBridge, this->pRotateEntitiesMap & enExternalEntityRotationTag::ErFrame, changeWingAngle && this->pRotateEntitiesMap & enExternalEntityRotationTag::ErWing, translateAngle);
	if(this->pRotateEntitiesMap & enExternalEntityRotationTag::ErLauncher)
		ChangeLaunchersDirection(translateAngle);
}

void PlayerWalker::UpdateDiscreteHeading(bool masked)
{
	if (pDriver.GetDiscreteMovementMap().HasMovement() )
	{
		this->ResetRelativeHeading();
		this->setUnitHeading(geometry::radLimit2PI(this->pDriver.GetDiscreteMovementMap().GetHeadingFromMovement(masked)));
	}
	else if(pSteering == enumPlayerSteering::MOUSESTEERING)
		this->setUnitHeading(geometry::radLimit2PI(this->pDriver.GetDiscreteMovementMap().GetHeadingFromMovement(masked)));
}

void PlayerWalker::ChangeExternalModuleDirection(bool bridge, bool frame, bool wings, double angle)
{
	for (auto it = this->pModuleContainer.EModules_begin(); it != this->pModuleContainer.EModules_end(); ++it)
	{
		if (bridge && (*it)->GetModuleType() == enModuleType::mBridge)
			(*it)->getRelativePosition() == Coord(0) ? (*it)->SetRelativeHeading(angle) : this->getGraphics()->getGraphicalObject()->UpdateLinkHeading(angle, (*it)->GetID());
		if (frame && (*it)->GetModuleType() == enModuleType::mFrame)
			(*it)->getRelativePosition() == Coord(0) ?  (*it)->SetRelativeHeading(angle) : this->getGraphics()->getGraphicalObject()->UpdateLinkHeading(angle, (*it)->GetID());
		if (wings && (*it)->GetModuleType() == enModuleType::mWing)
			(*it)->getRelativePosition() == Coord(0) ?  (*it)->SetRelativeHeading(angle) : this->getGraphics()->getGraphicalObject()->UpdateLinkHeading(angle, (*it)->GetID());
	}
}

void PlayerWalker::ResetRelativeHeading()
{
	ChangeExternalModuleDirection(pSteering == enumPlayerSteering::NOSTEERING, pSteering == enumPlayerSteering::NOSTEERING, this->pDriver.GetEnumDrive() == EnumEntityDrive::Keyboard, 0);
	if (pSteering == enumPlayerSteering::NOSTEERING)
		ChangeLaunchersDirection(0);
}


bool PlayerWalker::HasAmmo(string type)
{
	return true;
}
int PlayerWalker::AmmoAmount(string type)
{
	return 10;
}
int PlayerWalker::AmmoAmount(std::shared_ptr<LauncherEntity> entity)
{
	return 10;
}
bool PlayerWalker::DecrementAmmo(string type)
{
	return true;
}

void PlayerWalker::RefreshAmmoMap()
{
	return;
}

std::shared_ptr<PlayerWalker> PlayerWalker::GeneratePlayerWalkerFromWalkerName(string name)
{
	std::shared_ptr<PlayerWalker> result = nullptr;

	PlayerWalker* pWalker = (PlayerWalker*)WalkerGlobalList::GetWalker(name);
	if (pWalker == nullptr)
		return result;


	result = std::shared_ptr<PlayerWalker>(pWalker);
	result->SetPosition(Coord(20, 0));
	result->SetModulePlayerOwned();
	result->RefreshAmmoMap();
	return result;
}


void PlayerWalker::HandleUnitState(Uint32 milliseconds)
{
	switch (this->GetUnitState())
	{
	case(enumUnitState::DOCKING):

		//if (this->pDriver.GetGuidance().waypointCount() == 0)
		//{
		//	this->SetVesselState(enumUnitState::NORMAL);
		//	return;
		//}

		if (true || currentWaypointReached())
		{
			if (getTimer() > getDockingDelay())
			{

				this->SetUnitState(enumUnitState::DOCKED);
				this->SetDrive(EnumEntityDrive::DockedTarget);
			}
			else
			{
				this->SetUnitState(enumUnitState::DOCKING);
				//this->SetDockDrive();
				//incrementTimer(milliseconds);
			}
		}
		else
		{
			resetTimer();
			this->SetUnitState(enumUnitState::DOCKING);
		}
		break;
	case(enumUnitState::DOCKED):
		if (true || !currentWaypointReached())
		{
			this->SetUnitState(enumUnitState::DOCKING);
			//this->SetDockDrive();
			resetTimer();
		}
		break;
	default:
		break;
	}

}

void PlayerWalker::DockToTarget(DynamicEntity* target, ClickAction docktype)
{
	EnableAutoPilot();
	this->SetUnitState(enumUnitState::DOCKING);
	//this->autoPilotMoveToTarget(target);
	this->SetDockAction(docktype);
}
void PlayerWalker::DockToPoint(const Coord point, double dockedHeading, ClickAction docktype)
{
	EnableAutoPilot();
	this->SetUnitState(enumUnitState::DOCKING);
	//this->autoPilot->setStatus(enVesselAIStatus::IDLE);
	//this->autoPilotMoveToPoint(point - Coord(this->getSize() / 2), dockedHeading);
	this->SetDockAction(docktype);
}

void PlayerWalker::EnableAutoPilot()
{
	this->pState = enumPlayerState::AUTOPILOT;
	this->SetDriveType(DriverControlStyle::enAutoControl);
	this->EnableWaypointTracking(true);
}

void PlayerWalker::EnableControlDisabled()
{
	this->pState = enumPlayerState::CONTROLDISABLED;
	this->EnableWaypointTracking(false);
	this->SetDrive(EnumEntityDrive::Stable);
	this->SetRotationDrive(EnumEntityDrive::Stable);
}