#include"PlayerVessel.h"
#include"Behaviours.h"
#include"GameManager.h"
#include"VesselGlobalList.h"
#include"SDLKeyboardSyntax.h"
#include"Behaviours.h"


PlayerVessel::PlayerVessel( Coord pos) : Vessel(pos)
{
	initialize();
}

PlayerVessel::PlayerVessel() : Vessel()
{
	initialize();
}

void PlayerVessel::setStateToNormal()
{
	if (this->GetUnitState() == enumUnitState::NORMAL &&
		pState == enumPlayerState::PLAYERPILOT)
		return;

	pState = enumPlayerState::PLAYERPILOT;
	this->SetUnitState(enumUnitState::NORMAL);
	this->SetDriveType(DriverControlStyle::enManualControl);
	this->EnableWaypointTracking(false);
	//this->pSteering = enumPlayerSteering::NOSTEERING;
	//simpleThrust = 2000;
	this->SetAbsVelocity(1000);
	this->SetAbsAngularVelocity(50);
	this->SetAbsAngularAcceleration(10);
	//this->pDriver.SetMaxAngularThrust(9999);
	this->SetReferenceSpeed(100);
	this->SetHeadingReferenceSpeed(20);
	this->DefaultControllerGains();
	//this->pDriver.SetVelocityMagnitude(1000);

}

void PlayerVessel::DefaultControllerGains()
{
	this->setControllerGain(2000);
	this->setControllerDerivative(10);
	this->setControllerIntergral(0.00);
	this->setControllerGainHeading(5000);
	this->setControllerDerivativeHeading(2200);
	this->setControllerIntegralHeading(0);
}

void PlayerVessel::initialize()
{
	currentThrust = primitives::vector(0, 0);
	autoPilot = new VesselAI(this);
	pHotKeyList.clear();
	pBoostInterval = 6500;
	this->SetIteration(DriverIteratonStyle::enContinuously);
	this->SetDrive(EnumEntityDrive::Stable);
	this->SetRotationDrive(EnumEntityDrive::Stable);
	this->SetSideslip(false);
	InitializePlayerStates();
	this->EnableWaypointTracking(false);
	InitializeHotkeys();
	setStateToNormal();

	this->pEntitiesNotTracked = std::set<std::shared_ptr<Vessel>>();
}

void PlayerVessel::SetRotationDriveToStable()
{
	if (this->GetRotationDrive() != EnumEntityDrive::Stable)
	{
		this->SetRotationDrive(EnumEntityDrive::Stable);
		this->DefaultControllerGains();
	}
}



void PlayerVessel::AddWPlatform( std::shared_ptr<LauncherEntity> in, Coord pos, Coord anchorPoint )
{
	Vessel::AddWPlatform(in,pos,anchorPoint);
	in->MakePlayerAlly(true);
	in->SetHotKey(SDLKeyBible::GetNumberAsScancode(this->pModuleContainer.GetModulesAmount(enModuleType::mLauncher))); // pHotKeyRegister[48 + (pHotKeyRegister.size() % 10)].push_back(in);
	this->RefreshHotKeyList();
}
void PlayerVessel::RefreshHotKeyList()
{
	pHotKeyList.clear();
	for (auto it = this->pModuleContainer.LE_begin(); it != this->pModuleContainer.LE_end(); ++it)
	{
		if(!(*it)->IsDetached() && !(*it)->isDestroyed())
			pHotKeyList.insert((*it)->GetHotKey());
	}
}

//void PlayerVessel::handleVesselState(Uint32 milliseconds)
//{
//	if (pSteering == enumPlayerSteering::MOUSESTEERING)
//	{
//		HandleMouseSteering();
//	}
//	Vessel::handleVesselState(milliseconds);
//}


void PlayerVessel::HandleMouseSteering()
{
	int xMouse, yMouse;
	SDL_GetMouseState(&xMouse, &yMouse);

	Coord MousePos = Coord(1 * xMouse, yMouse * -1);
	double newHeading = geometry::angleBetweenTwoPoints(Coord(1 * GameManager::SCREEN_WIDTH  / 2, -1 * GameManager::SCREEN_HEIGHT / 2), MousePos);
	//Coord MousePos = Coord(-1 * xMouse, yMouse * 1);
	//double newHeading = geometry::angleBetweenTwoPoints(Coord(-1 * GameManager::SCREEN_HEIGHT / 2, 1 * GameManager::SCREEN_WIDTH / 2), MousePos);
	//newHeading = this->getGlobalHeading() + geometry::shortestAngleDifference(newHeading, this->getGlobalHeading());
	setWaypointHeading(geometry::radLimit2PI(newHeading));
	if (this->GetRotationDrive() != EnumEntityDrive::DynamicHeading)
	{
		this->SetRotationDrive(EnumEntityDrive::DynamicHeading);
		this->setControllerGainHeading(5000);// 0);
		this->setControllerDerivativeHeading(3700);// 0);
		this->setControllerIntegralHeading(0);
		//this->SetAbsAngularVelocity(0.1);
		//this->pDriver.SetMaxAngularThrust(800);
	}
	
	//Walker state
	//if (this->IsWalkerState())
	//{
	//	ChangeLaunchersDirection(newHeading + 1.57 - this->getUnitHeading() );
	//	return;
	//}

	//double anglediff = geometry::shortestAngleDifference(newHeading, this->getGlobalHeading());
	//if (anglediff <= -0.01)
	//{
	//	this->SetRotationDrive(EnumEntityDrive::DynamicHeading);
	//	//this->KeyboardTurnLeft();
	//}
	//else if (anglediff > 0.01)
	//{
	//	this->SetRotationDrive(EnumEntityDrive::DynamicHeading);
	//	//this->KeyboardTurnRight();
	//}
	//else
	//	this->SetRotationDrive(EnumEntityDrive::Stable);
}

//void PlayerVessel::handleCtrlEvent( SDL_Event& e )
//{
//	if (this->pState == CONTROLDISABLED)
//		return;
//
//	//If a key was pressed
//    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
//    {
//		HandleKeyButtonDown(e);
//	}
//	else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
//    {
//		HandleKeyButtonUp(e);
//    }
//	else if (e.type == SDL_MOUSEBUTTONDOWN)
//	{
//		HandleMouseDown(e);
//	}
//	else if (e.type == SDL_MOUSEBUTTONUP)
//	{
//		HandleMouseUp(e);
//	}
//}

void PlayerVessel::HandleMouseDown(SDL_Event& e)
{
	if (e.button.button == htMouseTurn)
	{
		pSteering = enumPlayerSteering::MOUSESTEERING;
		//enEntityDrive.drive = EnumEntityDrive::Forward;
		if (this->GetUnitState() != enumPlayerState::PLAYERPILOT)
			setStateToNormal();
	}
	if (e.button.button == htMouseFire)
	{
		Fire();
	}
}

void PlayerVessel::HandleMouseUp(SDL_Event& e)
{
	if (e.button.button == htMouseTurn)
	{
		SetRotationDriveToStable();
		//enEntityDrive.drive = EnumEntityDrive::Stable;
		pSteering = enumPlayerSteering::NOSTEERING;
		//if (this->enVesselState != enumPlayerState::PLAYERPILOT)
		//	setStateToNormal();
	}
	if (e.button.button == htMouseFire)
	{
		DeFire();
	}
		
}

void PlayerVessel::TriggerBooster()
{
	if (pBoosterTimer.TickSumEqual(this->pBoostInterval))
	{
		this->setStateToNormal();
		//this->resetTimer();
		this->IncreaseVelocity(geometry::rotateVector(primitives::vector(0, 100), this->getUnitHeading()));
		//setImpulseForce(geometry::rotateVector(primitives::vector(0, 500000), this->getUnitHeading()));
		//SetDrive(EnumEntityDrive::None);
	}
}

double PlayerVessel::GetBoosterProgressRatio()
{
	return (double)pBoosterTimer.GetTickSum(this->pBoostInterval, false) / this->pBoostInterval;
}

void PlayerVessel::HandleKeyButtonUp(SDL_Event& e)
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
		if (false) //this->IsWalkerState())
			this->KeyboardLeft(false);
	//enEntityDrive.drive = EnumEntityDrive::Stationary;
		else
			SetRotationDriveToStable();
	else if (code == htRight)
		if (false) //this->IsWalkerState()
			this->KeyboardRight(false);
	//enEntityDrive.drive = EnumEntityDrive::Stationary;
		else
			SetRotationDriveToStable();
	else if (code == htFire)
		DeFire();
	else if(code == htBlink)
	{ }
	else if(code == htDelay)
	{ }
	else if (code == htBoost)
	{
		this->TriggerBooster();
	}
}
void PlayerVessel::HandleKeyButtonDown(SDL_Event& e)
{
	SDL_Scancode code = e.key.keysym.scancode;
	if (code == htForward)
	{
		this->SetDrive(EnumEntityDrive::Keyboard);
		this->KeyboardForward(true);
		if (this->GetUnitState() != enumPlayerState::PLAYERPILOT)
			setStateToNormal();
		return;
	}
	if (code == htBackward)
	{
		this->SetDrive(EnumEntityDrive::Keyboard);
		this->KeyboardBackward(true);
		if (this->GetUnitState() != enumPlayerState::PLAYERPILOT)
			setStateToNormal();
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
			this->SetRotationDrive(EnumEntityDrive::Keyboard);
			this->KeyboardTurnLeft();
			if (this->GetUnitState() != enumPlayerState::PLAYERPILOT)
				setStateToNormal();
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
		this->SetRotationDrive(EnumEntityDrive::Keyboard);
		this->KeyboardTurnRight();
		if (this->GetUnitState() != enumPlayerState::PLAYERPILOT)
			setStateToNormal();
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


void PlayerVessel::HandleLauncherSelection(SDL_Event& e, SDL_Scancode& c)
{
	if (this->pHotKeyList.count(c) > 0)
	{
		for (auto lt = this->pModuleContainer.LE_begin(); lt != this->pModuleContainer.LE_end(); ++lt)
		{
			if ((*lt)->GetHotKey() == c)
				this->SelectLauncher(*lt);
			else
				(*lt)->Disable();
		}
		//GameManager::RefreshHUDLauncherList();
	}
}

void PlayerVessel::DetachModule(std::shared_ptr<ExternalModule> toRemove, const primitives::vector& direction)
{
	Vessel::DetachModule(toRemove, direction);
	if (toRemove->GetModuleType() == enModuleType::mEngine || toRemove->GetModuleType() == enModuleType::mBridge)
	{
		//game over
		GameManager::CurrentInstanceManager()->GetIntegrator()->setSpeed(2);
		EnableControlDisabled();
	}
	//if (toRemove->GetModuleType() == enModuleType::mLauncher || 
	//	toRemove->GetModuleType() == enModuleType::mAntiMLauncher ||
	//	toRemove->GetModuleType() == enModuleType::mMissileLauncher ||
	//	toRemove->GetModuleType() == enModuleType::mRailgunLauncher ||
	//	toRemove->GetModuleType() == enModuleType::mTorpedoLauncher)
	RefreshHotKeyList();
	RefreshAmmoMap();
}

void PlayerVessel::AddRadarTarget(std::shared_ptr<Vessel> reference )
{
	if (reference->IsSpawned() == false)
	{
		this->pEntitiesNotTracked.insert(reference);
		return;
	}
	
	if(reference->isDestroyed())
		return;

	for( auto it = this->pModuleContainer.RE_begin(); it != this->pModuleContainer.RE_end(); ++it )
	{
		if( (*it)->TrackVessel( reference ) )
			return;
	}
}

void PlayerVessel::RunRadarTargetTracking( double milliseconds )
{
	std::vector<std::shared_ptr<RadarEntity>> notFullRadarEntities = std::vector<std::shared_ptr<RadarEntity>>();

	for( auto it = this->pModuleContainer.RE_begin(); it != this->pModuleContainer.RE_end(); ++it )
	{
		for( auto tracker = (*it)->pbegin(); tracker != (*it)->pend(); ++tracker )
		{
			if ((*tracker)->IsActive())
			{
				(*tracker)->RefreshState();
				(*tracker)->SetVesselRadarPosition(this->getPosition());
				(*tracker)->move(milliseconds);
			}
			else
				notFullRadarEntities.push_back(*it);
		}
	}
	if(notFullRadarEntities.size() > 0)
		AddRadarTargetFromNotTrackedSet(notFullRadarEntities);

}

void PlayerVessel::UpdateTrackedTargets()
{
	RunRadarTargetTracking(0);
}

void PlayerVessel::AddRadarTargetFromNotTrackedSet(const std::vector<std::shared_ptr<RadarEntity>>& radarSet)
{
	bool entityAdded = false;
	for (auto it = pEntitiesNotTracked.begin(); it != pEntitiesNotTracked.end(); )
	{
		if ((*it)->IsSpawned())
		{
			for (auto itt = radarSet.begin(); itt != radarSet.end(); ++itt)
			{
				if ((*itt)->TrackVessel((*it)))
				{
					entityAdded = true;
					break;
				}
			}
		}

		if (entityAdded)
			it = pEntitiesNotTracked.erase(it);
		else
			++it;
	}
}

void PlayerVessel::ResetRadarEntities()
{
	for( auto it = this->pModuleContainer.RE_begin(); it != this->pModuleContainer.RE_end(); ++it )
	{
		(*it)->DisableTrackers();
	}
}

void PlayerVessel::move(Uint32 milliseconds)
{
	if( autoPilotOn() )
		autoPilot->RunAI(milliseconds);

	Vessel::move(milliseconds);
}

bool PlayerVessel::autoPilotOn() 
{
	return (pState==enumPlayerState::AUTOPILOT ); 
}
//
//void PlayerVessel::moveTo( DynamicEntity* target )
//{
//	EnableAutoPilot();
//	autoPilotMoveToTarget(target);
//}


void PlayerVessel::HandleUnitState(Uint32 milliseconds)
{
	if (pSteering == enumPlayerSteering::MOUSESTEERING)
	{
		HandleMouseSteering();
	}

	switch (this->GetUnitState())
	{
	case(enumUnitState::DOCKING):

		if (this->pDriver.GetGuidance().waypointCount() == 0)
		{
			this->SetUnitState(enumUnitState::NORMAL);
			return;
		}

		if (currentWaypointReached())
		{
			if (getTimer() > getDockingDelay())
			{

				this->SetUnitState(enumUnitState::DOCKED);
				this->SetDrive(EnumEntityDrive::DockedTarget);
			}
			else
			{
				this->SetUnitState(enumUnitState::DOCKING);
				this->SetDockDrive();
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
		if (!currentWaypointReached())
		{
			this->SetUnitState(enumUnitState::DOCKING);
			this->SetDockDrive();
			resetTimer();
		}
		else //This part replace function handlePlayerState in GCManager
		{
			GameManager::HandleDockAction(GetCurrentDockAction());	
			this->SetUnitState(enumUnitState::NORMAL);
		}
		break;
	default:
		break;
	}

}

void PlayerVessel::DockToTarget( DynamicEntity* target, ClickAction docktype)
{
	//EnableAutoPilot();
	//this->SetUnitState(enumUnitState::DOCKING);
	//this->autoPilotMoveToTarget(target);
	//this->SetDockAction( docktype );
}

void PlayerVessel::DockToPoint( const Coord point, double dockedHeading, ClickAction docktype)
{
	EnableAutoPilot();
	this->SetUnitState(enumUnitState::DOCKING);
	this->autoPilot->setStatus( enVesselAIStatus::IDLE );
	this->autoPilotMoveToPoint( point - Coord(this->getSize()/2),dockedHeading);
	this->SetDockAction( docktype );
}

void PlayerVessel::autoPilotMoveToPoint( const Coord point, double finalHeading  )
{
	BehaviorTree bTree = AIModel::GenerateTravelToCoordTree( primitives::waypoint(point,10, finalHeading) );

	this->SetAbsVelocity(10);
	this->pDriver.SetAngularVelocityMagnitude(0.3); 	//velocityHeadingSetPointAbsValue = 0.3;
	this->pDriver.SetAngularAccelerationMagnitude(2);

	this->setControllerGain(20);
	this->setControllerDerivative( 220);
	this->setControllerIntergral(0.000);

	this->setControllerGainHeading(5000);
	this->setControllerDerivativeHeading(1000);
	this->setControllerIntegralHeading(0.0);

	autoPilot->SetBehaviorTree( bTree );
	autoPilot->RunAI(0);
}

//void PlayerVessel::autoPilotMoveToTarget( DynamicEntity* target  )
//{
//	BehaviorTree* bTree = BAITree::generateTrackVessel( target );
//	this->setControllerDerivativeHeading(1);
//	this->setControllerGain(400);
//	this->setControllerDerivative( 400);
//	this->setControllerIntergral(0.000);
//
//	this->setControllerGainHeading(70);
//	this->setControllerDerivativeHeading(10);
//	this->setControllerIntegralHeading(0.0);
//	//this->clearWaypoints();
//	autoPilot->runBehaviorTree( bTree );
//	autoPilot->runAI(0);
//}


void PlayerVessel::EnableAutoPilot()
{
	this->pState = enumPlayerState::AUTOPILOT;
	this->SetDriveType(DriverControlStyle::enAutoControl);
	this->EnableWaypointTracking(true);
}

void PlayerVessel::EnableControlDisabled()
{
	this->pState = enumPlayerState::CONTROLDISABLED;
	this->pSteering = enumPlayerSteering::NOSTEERING;
	this->EnableWaypointTracking(false);
	if (this->GetDrive() != EnumEntityDrive::None)
		this->SetDrive(EnumEntityDrive::Stable);
	if(this->GetRotationDrive() != EnumEntityDrive::None)
		this->SetRotationDrive(EnumEntityDrive::Stable);

}


void PlayerVessel::RefreshInventory()
{
	for( auto lt = this->pModuleContainer.LE_begin(); lt != this->pModuleContainer.LE_end(); ++lt )
	{
		(*lt)->refreshState();
		(*lt)->refreshHotKeys();
	}

	for( auto lt = this->pModuleContainer.PSE_begin(); lt != this->pModuleContainer.PSE_end(); ++lt )
	{
		(*lt)->refreshState();
	}

	for( auto lt = this->pModuleContainer.EE_begin(); lt != this->pModuleContainer.EE_end(); ++lt )
	{
		(*lt)->refreshState();
	}

	for( auto lt = this->pModuleContainer.RE_begin(); lt != this->pModuleContainer.RE_end(); ++lt )
	{
		(*lt)->refreshState();
	}

	for( auto lt = this->pModuleContainer.CE_begin(); lt != this->pModuleContainer.CE_end(); ++lt )
	{
		(*lt)->refreshState();
	}

	for (auto lt = this->pModuleContainer.AC_begin(); lt != this->pModuleContainer.AC_end(); ++lt)
	{
		(*lt)->refreshState();
	}

	for( auto lt = this->pModuleContainer.EModules_begin(); lt != this->pModuleContainer.EModules_end(); ++lt )
	{
		(*lt)->refreshState();
	}

	this->RefreshHotKeyList();
}

void PlayerVessel::TargetVessel( std::shared_ptr<UnitEntity> target )
{
	if (target.get())
		AddTarget(target);
	else
		RemoveTarget();
}

void PlayerVessel::AddTarget(std::shared_ptr<UnitEntity> target)
{
	Vessel::AddTarget(target);
	for (auto rd = this->pModuleContainer.RE_begin(); rd != this->pModuleContainer.RE_end(); ++rd)
		(*rd)->HighlightScreenSlider(target.get());
}

void PlayerVessel::RemoveTarget()
{
	Vessel::RemoveTarget();
	for (auto rd = this->pModuleContainer.RE_begin(); rd != this->pModuleContainer.RE_end(); ++rd)
		(*rd)->HighlightScreenSlider(nullptr);
}

std::vector<std::shared_ptr<UnitEntity>> PlayerVessel::GetTrackedTargets()
{
	std::vector<std::shared_ptr<UnitEntity>> result = std::vector<std::shared_ptr<UnitEntity>>();
	for (auto rd = this->pModuleContainer.RE_begin(); rd != this->pModuleContainer.RE_end(); ++rd)
	{
		auto targets = (*rd)->GetTrackedTargets();
		for (auto it = targets.begin(); it != targets.end(); ++it)
			if (std::shared_ptr<UnitEntity> derived = std::dynamic_pointer_cast<UnitEntity>(*it))
				result.push_back(derived);
	}
	return result;
}

std::shared_ptr<PlayerVessel> PlayerVessel::GeneratePlayerVesselFromVesselName(string vesselName)	//not in use, but kept as a reference
{
	try
	{
		PlayerVessel* pVessel = (PlayerVessel*)VesselGlobalList::GetVessel(vesselName);
		pVessel->SetPosition(Coord(20, 0));
		pVessel->SetModulePlayerOwned();
		pVessel->RefreshAmmoMap();
		return std::shared_ptr<PlayerVessel>(pVessel);
	}
	catch (...)
	{
		GameManager::Screens.TriggerErrorScreen("assert", "Failed to setup player vessel!");
		return nullptr;
	}
}

//void PlayerVessel::SelectLauncher(int launcherID)
//{
//	ActivateModule(enModuleType::mLauncher, launcherID);
//
//}
//void PlayerVessel::SelectLauncher(string launcherTextID)
//{
//	//int newSelectedID = LauncherSelector::GetLauncherIDFromText(launcherTextID);
//	SelectLauncher(newSelectedID);
//}

void PlayerVessel::SelectLauncher(std::shared_ptr<LauncherEntity> entity)
{
	entity->Enable();
}

bool PlayerVessel::isDestroyed()
{
	return false;
}

void PlayerVessel::SetModulePlayerOwned()
{
	for( auto lt = this->pModuleContainer.LE_begin(); lt != this->pModuleContainer.LE_end(); ++lt )
	{
		(*lt)->MakePlayerAlly(true);
	}
}

//void PlayerVessel::SetPlayerState(enumPlayerState state)
//{
//	this->pState = state;
//}

bool PlayerVessel::HasAmmo(string type)
{
	return pAmmoAmmountList[type] > 0;
}


int PlayerVessel::AmmoAmount(string type)
{
	auto iter = pAmmoAmmountList.find(type);
	if (iter != pAmmoAmmountList.end())
		return pAmmoAmmountList[type];
	return 0;
}

int PlayerVessel::AmmoAmount(std::shared_ptr<LauncherEntity> entity)
{
	return AmmoAmount(entity->GetAmmoType());
}

bool PlayerVessel::DecrementAmmo(string type)
{
	for (auto it = this->pModuleContainer.AC_begin(); it != this->pModuleContainer.AC_end(); ++it)
	{
		if ((*it)->DecrementAmmo(type) == true)
		{
			pAmmoAmmountList[type]--;
			return true;
		}
	}
	return false;
}

void PlayerVessel::RefreshAmmoMap()
{
	pAmmoAmmountList = std::map<std::string, int>();
	for (auto it = this->pModuleContainer.AC_begin(); it != this->pModuleContainer.AC_end(); ++it)
	{
		if ((*it)->IsDetached() || (*it)->isDestroyed())
			continue;

		for (auto itt = (*it)->abegin(); itt != (*it)->aend(); ++itt)
		{
			auto iter = pAmmoAmmountList.find((*itt)->GetAmmoType());
			if (iter != pAmmoAmmountList.end())
			{
				pAmmoAmmountList[(*itt)->GetAmmoType()] += (*itt)->GetAmmoAmount();
			}
			else
			{
				pAmmoAmmountList[(*itt)->GetAmmoType()] = (*itt)->GetAmmoAmount();
			}
		}
	}
}

EntityPrice PlayerVessel::GetTotalPrice(bool includeShipValue) const
{
	EntityPrice result = EntityPrice(ShipModuleValue());

	if (includeShipValue)
		result.IncreaseSellValue(this->PlayerShipValue());

	return result;
}

statistics::RandomNumber PlayerVessel::PlayerShipValue() const
{
	return statistics::RandomNumber();	//based on player stats on that ship (repairs, kills, etc) performance gives better result
}

