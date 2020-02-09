#include"VesselIntegrator.h"
#include"StringBuilder.h"

int tVesselComputeThread(void* data)
{
	SDL_LockMutex(integrator::pVesselThreadCreationLock);
	//barrier::SetSDLCondWaitBarrier(&MVesselThreadCreationBarrier, 1);
	std::tuple<integrator*, std::shared_ptr<Vessel> >* recievedData = (std::tuple<integrator*, std::shared_ptr<Vessel> >*)data;
	integrator* parent = std::get<0>(*recievedData);//std::get<0>(*recievedData);
	std::shared_ptr<Vessel> vessel = std::get<1>(*recievedData);
	enumAllegiance allegiance = parent->GetUnitAllegiance(vessel);

	SDL_UnlockMutex(integrator::pVesselThreadCreationLock);
	barrier::SetSDLCondWaitBarrier(&parent->MVesselThreadCreationBarrier, 1);
	barrier::SetSDLCondWaitBarrier(&parent->MVesselThreadCreationBarrier, 0);

	if (allegiance == enumAllegiance::aENEMY)
	{
		while (parent->RunUnitThreads())
		{
			barrier::SetSDLCondWaitBarrier(&parent->MUnitBarrier, 1);
			if (vessel->IsSpawned())
			{

				if (parent->ComputeUnitDynamic(vessel, parent->GetCurrentTimeStep(true).Value, enumAllegiance::aENEMY) == false) //false == removed from vessel list
				{
					barrier::SetSDLCondWaitBarrier(&parent->MUnitBarrier, 0);
					break;
				}

				vessel->ChangeLauncherTargets();
				vessel->incrementTimer(parent->GetCurrentTimeStep(true).Value);
			}

			barrier::SetSDLCondWaitBarrier(&parent->MUnitBarrier, 0);
		}
	}
	else
	{
		while (parent->RunUnitThreads())
		{
			barrier::SetSDLCondWaitBarrier(&parent->MUnitBarrier, 1);
			if (vessel->IsSpawned())
			{
				if (parent->ComputeUnitDynamic(vessel, parent->GetCurrentTimeStep(true).Value, enumAllegiance::aFRIENDLY) == false) //false == removed from vessel list
				{
					barrier::SetSDLCondWaitBarrier(&parent->MUnitBarrier, 0);
					break;
				}

				vessel->incrementTimer(parent->GetCurrentTimeStep(true).Value);
				vessel->ChangeLauncherTargets();
			}
			//if (parent->RunVesselThreads() == false) break;
			barrier::SetSDLCondWaitBarrier(&parent->MUnitBarrier, 0);
		}
	}

	parent->DecrementThreadUnitCounter();
	return 0;
}

VesselIntegrator::VesselIntegrator() : integrator()
{
	Initialize();
}

VesselIntegrator::VesselIntegrator(bool fixedIndependentStepTime) : integrator(fixedIndependentStepTime)
{
	Initialize();
}

void VesselIntegrator::Initialize()
{
	enemyVesselObjects = std::vector<std::shared_ptr<UnitEntity>>();
	alliedVesselObjects = std::vector<std::shared_ptr<UnitEntity>>();

}

std::shared_ptr<UnitEntity> VesselIntegrator::GetUnitEntityFromID(int ID)
{
	for (auto it = this->enemyVesselObjects.begin(); it != this->enemyVesselObjects.end(); ++it)
	{
		if ((*it)->GetID() == ID)
			return *it;
	}
	for (auto it = this->alliedVesselObjects.begin(); it != this->alliedVesselObjects.end(); ++it)
	{
		if ((*it)->GetID() == ID)
			return *it;
	}
	return nullptr;
}

std::vector<std::shared_ptr<UnitEntity>> VesselIntegrator::GetUnitEntities(bool enemy)
{
	if (enemy)
		return this->enemyVesselObjects;
	else
		return this->alliedVesselObjects;
}

enumAllegiance VesselIntegrator::GetUnitAllegiance(std::shared_ptr<UnitEntity> vessel)
{
	for (auto it = this->alliedVesselObjects.begin(); it != alliedVesselObjects.end(); ++it)
		if ((*it) == vessel)
			return enumAllegiance::aFRIENDLY;
	return enumAllegiance::aENEMY;
}

void VesselIntegrator::HandleEvents(SDL_Event & e, const Coord& cameraPos)
{
	if (this->CurrentPlayer().expired() == false)
	{
		this->GetPlayerVessel().lock().get()->handleCtrlEvent(e);
		this->CurrentPlayer().lock().get()->handleEvent(e, cameraPos);

		for (auto it = this->enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
		{
			CurrentEventObject = it->get();
			(*it)->handleEvent(e, cameraPos);
		}

		for (auto it = this->alliedVesselObjects.begin(); it != alliedVesselObjects.end(); ++it)
		{
			CurrentEventObject = it->get();
			(*it)->handleEvent(e, cameraPos);
		}

		for (auto it = vesselDebri.begin(); it != vesselDebri.end(); ++it)
		{
			CurrentEventObject = it->get();
			(*it)->handleEvent(e, cameraPos);
		}
	}
}

void VesselIntegrator::HandlePlayer(Uint32 ms)
{
	currentPlayerVessel.lock()->HandleUnitState(ms);
}

void VesselIntegrator::RunUnitEntity(double milliseconds)
{
	if (!ThreadsInitialized())
	{
		for (auto it = enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
		{
			if ((*it)->IsSpawned() && ComputeUnitDynamic(*it, milliseconds, enumAllegiance::aENEMY) == false) //false == removed from vessel list
				break;

			//removed for testing
			(*it)->ChangeLauncherTargets();
			(*it)->incrementTimer(milliseconds);

		}

		for (auto it = alliedVesselObjects.begin(); it != alliedVesselObjects.end(); ++it)
		{
			if ((*it)->IsSpawned() && ComputeUnitDynamic(*it, milliseconds, enumAllegiance::aFRIENDLY) == false) //false == removed from vessel list
			{
				//if (vesselObjects.size() == 0)
				break;
			}
			(*it)->ChangeLauncherTargets();
			//else
			//	++it;
		}
	}
}

void VesselIntegrator::RunPlayer(double milliseconds)
{
	ComputeUnitDynamic(this->GetPlayerVessel().lock(), milliseconds, enumAllegiance::aFRIENDLY);
	this->GetPlayerVessel().lock()->RunRadarTargetTracking(milliseconds);
	this->GetPlayerVessel().lock()->incrementTimer(milliseconds);
}

void VesselIntegrator::CollDetect()
{
	if (!ThreadsInitialized())
	{
		if (enemyVesselObjects.size() > 0)
			for (auto it = enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
				collDetectEnemyVessel(*it);

		if (alliedVesselObjects.size() > 0)
			for (auto it = alliedVesselObjects.begin(); it != alliedVesselObjects.end(); ++it)
				collDetectAlliedVessel(*it);
	}
	CollDetectAntiMissile();
	collDetectPlayer();
}

bool VesselIntegrator::SetPlayer(std::shared_ptr<PlayerInterface> player)
{
	if (auto derivative = std::dynamic_pointer_cast<PlayerVessel>(player))
	{
		SetPlayerVessel(derivative);
		return true;
	}
	return false;
}

void VesselIntegrator::SetPlayerVessel(std::weak_ptr<PlayerVessel> set)
{
	currentPlayerVessel = set;
}

void VesselIntegrator::LoadPlayerLaunchers()
{
	if (!currentPlayerVessel.expired())
	{
		for (auto it = this->currentPlayerVessel.lock()->LE_begin(); it != currentPlayerVessel.lock()->LE_end(); ++it)
			this->addLauncher((*it));
	}
}

void VesselIntegrator::AddVessel(std::shared_ptr<Vessel> object, bool includeLauncher, bool enemy)
{
	if (enemy)
	{
		this->enemyVesselObjects.push_back(object);
		for (auto it = alliedVesselObjects.begin(); it != alliedVesselObjects.end(); ++it)
		{
			(*it)->AddToTargetList(object);
			object->AddToTargetList(*it);
		}
	}
	else
	{
		this->alliedVesselObjects.push_back(object);
		for (auto it = enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
		{
			(*it)->AddToTargetList(object);
			object->AddToTargetList(*it);
		}
	}

	GenerateThreadForVessel(object);

	if (includeLauncher == false)
		return;

	for (auto it = object->LE_begin(); it != object->LE_end(); ++it)
	{
		this->addLauncher((*it));
	}
}


void VesselIntegrator::RemoveVessel(std::shared_ptr<Vessel> object)
{
	for (auto it = this->enemyVesselObjects.begin(); it != this->enemyVesselObjects.end(); ++it)
	{
		if (object == *it)
		{
			it->reset();
			this->enemyVesselObjects.erase(it);
			//this->DecrementThreadVesselCounter();
			return;
		}
	}
	for (auto it = this->alliedVesselObjects.begin(); it != this->alliedVesselObjects.end(); ++it)
	{
		if (object == *it)
		{
			it->reset();
			this->enemyVesselObjects.erase(it);
			//this->DecrementThreadVesselCounter();
			return;
		}
	}
}

void VesselIntegrator::RemoveVesselAI(std::shared_ptr<VesselAI> AI)
{
	for (auto it = vesselAIObjects.begin(); it != vesselAIObjects.end(); )
	{
		if (*it == AI)
			it = vesselAIObjects.erase(it);
	}
}

void VesselIntegrator::RemoveVesselAndLaunchers(std::shared_ptr<Vessel> object)
{
	RemoveLaunchersOfEntity(object);
	RemoveVessel(object);
}

void VesselIntegrator::RemoveUnit(std::shared_ptr<UnitEntity> object)
{
	if (std::shared_ptr<Vessel> derived = std::static_pointer_cast<Vessel>(object))
		this->RemoveVesselAndLaunchers(derived);
}

void VesselIntegrator::RemoveTargetSelectionOfUnit(std::shared_ptr<UnitEntity> IncludeThisVessel) const
{
	for (auto it = this->enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
	{
		if ((*it)->LinkAmount() == 0)
			continue;

		if ((*it) == IncludeThisVessel)
		{
			(*it)->LinkTarget.link->SetWidgetClicked();
			this->GetPlayerVessel().lock()->TargetVessel((*it));	//Hackish here
		}
		else
			(*it)->LinkTarget.link->SetWidgetOff();
	}
}

void VesselIntegrator::RemoveTargetSelectionOfUnit(Widget* ExcludeVesselOfLink) const
{
	for (auto it = this->enemyVesselObjects.begin(); it != enemyVesselObjects.end(); ++it)
	{
		if ((*it)->LinkAmount() == 0)
			continue;

		if ((*it)->LinkTarget.link.get() == ExcludeVesselOfLink)
		{
			(*it)->LinkTarget.link->SetWidgetClicked();
			this->GetPlayerVessel().lock()->TargetVessel((*it));	//Hackish here
		}
		else
			(*it)->LinkTarget.link->SetWidgetOff();
	}
}

void VesselIntegrator::ClearVessels()
{
	this->enemyVesselObjects.clear();
	this->alliedVesselObjects.clear();
}


void VesselIntegrator::GenerateThreadForVessel(std::shared_ptr<Vessel> vessel) //in use
{
	if (!ThreadsInitialized())
		return;

	SDL_LockMutex(pVesselThreadCreationLock);

	IncrementThreadUnitCounter();
	IncrementThreadCollisionCounter();

	std::tuple<integrator*, std::shared_ptr<Vessel> > threadData = std::tuple<integrator*, std::shared_ptr<Vessel> >(this, vessel);
	string Vname = "tV_" + IntToString(vessel->GetID());
	string Cname = "tC_" + IntToString(vessel->GetID());
	SDL_CreateThread(tVesselComputeThread, Vname.c_str(), (void*)&threadData);
	SDL_CreateThread(tCollisionDetectionThread, Cname.c_str(), (void*)&threadData);
	EnableUnitThreads(); // SDL_AtomicSet(&this->pClearUnitThreads, 0);

	SDL_UnlockMutex(pVesselThreadCreationLock);

	barrier::SetSDLCondWaitBarrier(&MVesselThreadCreationBarrier, 1);
	barrier::SetSDLCondWaitBarrier(&MVesselThreadCreationBarrier, 0);

}