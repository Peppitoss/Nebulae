#include"UnitModuleContainer.h"
#include"ReaderException.h"

#include<algorithm>



UnitModuleContainer::UnitModuleContainer()
{
	pUnitCenter = Coord(32);
}

void UnitModuleContainer::AddModuleToList(std::shared_ptr<ModuleEntity> module, bool AddToExternalModule)
{
	switch (module->GetModuleType())
	{
		case(enModuleType::mController):
		{
			pControllers.push_back(std::static_pointer_cast<ControllerEntity>(module));
			break;
		}
		case(enModuleType::mEngine):
		{
			pEngines.push_back(std::static_pointer_cast<EngineEntity>(module));
			break;
		}
		case(enModuleType::mPowerSupply):
		{
			pPowerSupplies.push_back(std::static_pointer_cast<PowerSupplyEntity>(module));
			break;
		}
		case(enModuleType::mRadar):
		{
			pRadars.push_back(std::static_pointer_cast<RadarEntity>(module));
			break;
		}
		case(enModuleType::mAmmoContainer):
		{
			if (pAmmos.size() >= 1)
				throw ReaderException("Error only one ammo container allowed per ship!");

			pAmmos.push_back(std::static_pointer_cast<AmmoContainer>(module));
			break;
		}
		default:
		{
			if (module->IsLauncherType())
				this->pWeaponPlatforms.push_back(std::static_pointer_cast<LauncherEntity>(module));
			else if (module->IsExternalType())
				this->pExtModList.push_back(std::static_pointer_cast<ExternalModule>(module));
			else if (module->GetModuleType() == enModuleType::mHitBox)
				this->pHitBoxList.push_back(std::static_pointer_cast<HitBoxEntity>(module));
		}
	}

	if (AddToExternalModule)
	{
		this->AddModuleToExternalModule(module);
	}
}

bool UnitModuleContainer::ContainsInternalModule(std::shared_ptr<ModuleEntity> entity)
{
	std::vector<std::shared_ptr<ModuleEntity>> modulelist = GetInternalModulesList();
	for (auto it = modulelist.begin(); it != modulelist.end(); ++it)
	{
		if ((*it) == entity)
			return true;
	}
	return false;
}

bool UnitModuleContainer::ContainsGridObject(std::shared_ptr<GridObject> object, bool allowDetached)
{
	std::vector<std::shared_ptr<ModuleEntity>>& modulelist = GetInternalModulesList();
	for (auto it = modulelist.begin(); it != modulelist.end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return true;
	}

	std::vector<std::shared_ptr<ExternalModule>>& extModulelist = GetExternalModulesList(allowDetached);
	for (auto it = extModulelist.begin(); it != extModulelist.end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return true;
	}

	return false;
}

bool UnitModuleContainer::RemoveModuleEntity(std::shared_ptr<ModuleEntity> in)
{
	switch (in->GetModuleType())
	{
		case(enModuleType::mController):
		{
			return RemoveItemFromVector(pControllers, std::static_pointer_cast<ControllerEntity>(in));
		}
		case(enModuleType::mEngine):
		{
			return RemoveItemFromVector(pEngines, std::static_pointer_cast<EngineEntity>(in));
		}
		case(enModuleType::mPowerSupply):
		{
			return RemoveItemFromVector(pPowerSupplies, std::static_pointer_cast<PowerSupplyEntity>(in));
		}
		case(enModuleType::mRadar):
		{
			return RemoveItemFromVector(pRadars, std::static_pointer_cast<RadarEntity>(in));
		}
		case(enModuleType::mAmmoContainer):
		{
			return RemoveItemFromVector(pAmmos, std::static_pointer_cast<AmmoContainer>(in));
		}
	}

	if (in->IsLauncherType())
		return RemoveItemFromVector(this->pWeaponPlatforms, std::static_pointer_cast<LauncherEntity>(in));

	if ( in->IsExternalType())
		return RemoveItemFromVector(pExtModList, std::static_pointer_cast<ExternalModule>(in));

	if (in->GetModuleType() == enModuleType::mHitBox)
		return RemoveItemFromVector(pHitBoxList, std::static_pointer_cast<HitBoxEntity>(in));

	return false;
}

bool UnitModuleContainer::RemoveLauncherEntity(std::shared_ptr<LauncherEntity> in)
{
	return RemoveItemFromVector(this->pWeaponPlatforms, in);
}
bool UnitModuleContainer::RemoveExternalEntity(std::shared_ptr<ExternalModule> extModule)
{
	return RemoveItemFromVector(pExtModList, extModule);
}

//bool UnitModuleContainer::RemoveDetachedEntity(std::shared_ptr<ModuleEntity> detached, bool resetArgumentPointer)
//{
//	if( RemoveItemFromVector(pDetachedModules, detached) );
//	{
//		if (resetArgumentPointer)
//			detached.reset();
//		return true;
//	}
//	return false;
//}

void UnitModuleContainer::RemoveDetachedEntity(std::vector<int>  indices)//, bool resetArgumentPointer)
{
	//if(resetArgumentPointer)
	//	for (int i = 0; i < indices.size(); i++)
	//		pDetachedModules[indices[i]].reset(); 

	RemoveItemsFromVector(pDetachedModules, indices);
}

void UnitModuleContainer::AddDetachedModuleToList(std::shared_ptr<ModuleEntity> dModule)
{
	this->pDetachedModules.push_back(dModule);
}

void UnitModuleContainer::AddExtModuleToList(std::shared_ptr<ExternalModule> extModule)
{
	this->pExtModList.push_back(extModule);
}
void UnitModuleContainer::AddHitBoxToList(std::shared_ptr<HitBoxEntity> hbModule)
{
	this->pHitBoxList.push_back(hbModule);
}
void UnitModuleContainer::AddLauncherToList(std::shared_ptr<LauncherEntity> lModule, bool AddToExternalModule)
{
	this->pWeaponPlatforms.push_back(lModule);
	if (AddToExternalModule)
		this->AddModuleToExternalModule(lModule);
}

void UnitModuleContainer::AddDecorationToList(std::shared_ptr<DecoModule> dModule)
{
	this->pDecorationModules.push_back(dModule);
}

void UnitModuleContainer::AddDoorToList(std::shared_ptr<DoorModule> dModule)
{
	this->pDoorModules.push_back(dModule);
}

bool UnitModuleContainer::AddModuleToExternalModule( std::shared_ptr<ModuleEntity> module)
{
	try 
	{
		if (module->getGridObject().get() == false)
			return false;

		//auto ext = GetExternalModuleFromCoord(module->getPosition(), false);// getRelativePosition() - module->getGridObject()->ObjectOrigo, false);
		//if (ext != nullptr)
		//{
		//	ext->AddContainedModule(module);
		//	return true;
		//}
		//return false;

		for (auto jt = EModules_begin(); jt != EModules_end(); ++jt)
		{
			if ((*jt)->getGridObject().get())
			{
				if ((*jt)->getGridObject()->GridObjectIntersect(module->getGridObject().get()))// ((*jt)->getGridObject()->mapHasKey((*jt)->getRelativePosition() - module->getRelativePosition() - module->getGridObject()->ObjectOrigo)) // - module->getGridObject()->ObjectOrigo))
				{
					(*jt)->AddContainedModule(module);
					return true;
				}
			}
			//else
			//{
			//	if ((*jt)->getGraphics()->CoordIsCollision((*jt)->getRelativePosition() - module->getRelativePosition() - module->getGridObject()->ObjectOrigo)) //)) 	//does this work?? not tested
			//	{
			//		(*jt)->AddContainedModule(module);
			//		return true;
			//	}
			//}
		}
		return false;
	}
	catch (int e)
	{
		return false;
	}
}

int UnitModuleContainer::GetModulesAmount(enModuleType type) const
{
	switch (type)
	{
		case(enModuleType::mController):
		{
			return pControllers.size();
		}
		case(enModuleType::mEngine):
		{
			return pEngines.size();
		}
		case(enModuleType::mPowerSupply):
		{
			return pPowerSupplies.size();
		}
		case(enModuleType::mRadar):
		{
			return pRadars.size();
		}
		case(enModuleType::mAmmoContainer):
		{
			return pAmmos.size();
		}
		case(enModuleType::mDecoration):
		{
			return pDecorationModules.size();
		}
		case(enModuleType::mDoor):
		{
			return pDoorModules.size();
		}
	}

	if (IsLauncherType(type))
		return pWeaponPlatforms.size();

	if (IsExternalType(type))
		return pExtModList.size();

	if (type == enModuleType::mHitBox)
		return pHitBoxList.size();

	return 0;
}
std::shared_ptr<ModuleEntity> UnitModuleContainer::GetModulesWithGridObject(const std::shared_ptr<GridObject> object)
{
	ModuleEntity* result = nullptr;

	for (auto it = EModules_begin(); it != EModules_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	for (auto it = EE_begin(); it != EE_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	for (auto it = RE_begin(); it != RE_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	for (auto it = PSE_begin(); it != PSE_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	for (auto it = CE_begin(); it != CE_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	for (auto it = LE_begin(); it != LE_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	for (auto it = AC_begin(); it != AC_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	for (auto it = Deco_begin(); it != Deco_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	for (auto it = Door_begin(); it != Door_end(); ++it)
	{
		if ((*it)->getGridObject() == object)
			return *it;
	}

	return nullptr;
}


std::shared_ptr<ModuleEntity> UnitModuleContainer::GetModulesFromID(enModuleType type, int ID)
{
	switch (type)
	{
	case(enModuleType::mController):
		for (auto it = this->CE_begin(); it != this->CE_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mEngine):
		for (auto it = this->EE_begin(); it != this->EE_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mLauncher):
		for (auto it = this->LE_begin(); it != this->LE_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mMissileLauncher):
		for (auto it = this->LE_begin(); it != this->LE_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mAntiMLauncher):
		for (auto it = this->LE_begin(); it != this->LE_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mRailgunLauncher):
		for (auto it = this->LE_begin(); it != this->LE_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mPowerSupply):
		for (auto it = this->PSE_begin(); it != this->PSE_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mRadar):
		for (auto it = this->RE_begin(); it != this->RE_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mAmmoContainer):
		for (auto it = this->AC_begin(); it != this->AC_end(); ++it)
		{
			if (it->get()->IsDetached() == false && (*it)->GetID() == ID)
				return (*it);
		}
		break;
	case(enModuleType::mRoom):
		break;
	default:
		return nullptr;
	}
	return nullptr;
}

std::vector<std::shared_ptr<ModuleEntity>> UnitModuleContainer::GetInternalModulesList() const
{
	std::vector<std::shared_ptr<ModuleEntity>> result = std::vector<std::shared_ptr<ModuleEntity>>();
	result.reserve(pWeaponPlatforms.size() + pEngines.size() + pRadars.size() + pPowerSupplies.size() + pAmmos.size() + pControllers.size());
	result.insert(result.end(), pWeaponPlatforms.begin(), pWeaponPlatforms.end());
	result.insert(result.end(), pEngines.begin(), pEngines.end());
	result.insert(result.end(), pRadars.begin(), pRadars.end());
	result.insert(result.end(), pPowerSupplies.begin(), pPowerSupplies.end());
	result.insert(result.end(), pAmmos.begin(), pAmmos.end());
	result.insert(result.end(), pControllers.begin(), pControllers.end());
	return result; 
}

std::vector<std::shared_ptr<ExternalModule>> UnitModuleContainer::GetExternalModulesList( bool allowDetached ) const
{
	std::vector<std::shared_ptr<ExternalModule>> result = std::vector<std::shared_ptr<ExternalModule>>();
	result.reserve(this->pExtModList.size());

	for (auto it = this->pExtModList.cbegin(); it != this->pExtModList.cend(); ++it)
	{
		if (allowDetached == false && it->get()->IsDetached() == true)
			break;
		result.push_back((*it));
	}
	return result;
}

std::vector<std::shared_ptr<AmmoContainer>> UnitModuleContainer::GetAmmoContainerList(bool allowDetached) const
{
	std::vector<std::shared_ptr<AmmoContainer>> result = std::vector<std::shared_ptr<AmmoContainer>>();
	result.reserve(this->pAmmos.size());

	for (auto it = this->pAmmos.cbegin(); it != this->pAmmos.cend(); ++it)
	{
		if (allowDetached == false && it->get()->IsDetached() == true)
			break;
		result.push_back((*it));
	}
	return result;
}

ModuleEntity* UnitModuleContainer::GetModuleFromCoord(const Coord& Coord, bool allowDetached)
{
	ModuleEntity* result = nullptr;

	for (auto it = EModules_begin(); it != EModules_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return it->get();
	}

	for (auto it = EE_begin(); it != EE_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return it->get();
	}

	for (auto it = RE_begin(); it != RE_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return it->get();
	}

	for (auto it = PSE_begin(); it != PSE_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return it->get();
	}

	for (auto it = CE_begin(); it != CE_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return it->get();
	}

	for (auto it = LE_begin(); it != LE_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return it->get();
	}

	for (auto it = AC_begin(); it != AC_end(); ++it)
	{
		if(IsModuleInCoord(*it, Coord,allowDetached))
			return it->get();
	}

	for (auto it = Deco_begin(); it != Deco_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return it->get();
	}

	for (auto it = Door_begin(); it != Door_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return it->get();
	}

	return nullptr;

}

std::shared_ptr<ExternalModule> UnitModuleContainer::GetExternalModuleFromCoord(const Coord& Coord, bool allowDetached)
{
	for (auto it = EModules_begin(); it != EModules_end(); ++it)
	{
		if (IsModuleInCoord(*it, Coord, allowDetached))
			return *it;
	}
	return nullptr;
}

bool UnitModuleContainer::IsModuleInCoord(const std::shared_ptr<ModuleEntity>& module, const Coord& pos, bool allowDetached)
{
	if (allowDetached == false && module->IsDetached() == true)
		return false;
	else if (module->GetModuleGraphics()->CoordIsCollision(TransformPositionToLinkFrame(pos, *module)))
		return true;
	return false;
}

Coord UnitModuleContainer::TransformPositionToLinkFrame(const Coord& pos, ModuleEntity& module)
{
	//Coord moduleObjectSize = module.getDimension().GetDimension();
	////return pos - module.getRelativePosition();// +moduleObjectSize / 2;
	//return pos - module.GetModuleGraphics()->getCenter() - module.getRelativePosition() + moduleObjectSize / 2;
	return module.TransformPositionToModuleFrame(pos, this->GetUnitCenter());
}

std::vector<std::shared_ptr<ModuleEntity>> UnitModuleContainer::GetModulesContainedInExternalModule(std::shared_ptr<ExternalModule> extModule)
{
	std::vector<std::shared_ptr<ModuleEntity>> result = std::vector<std::shared_ptr<ModuleEntity>>();
	for (auto it = extModule->contained_begin(); it != extModule->contained_end(); ++it)
		result.push_back(*it);
	return result;
}

void UnitModuleContainer::UpdateContainedListOfExternalModules()
{
	for (auto it = EModules_begin(); it != EModules_end(); ++it)
		(*it)->ClearContainedModuleList();

	std::vector<std::shared_ptr<ModuleEntity>> internalList = this->GetInternalModulesList();
	for (auto it = internalList.begin(); it != internalList.end(); ++it)
	{
		this->AddModuleToExternalModule((*it));
	}
}

int UnitModuleContainer::GetStructuralHitPoints() const
{
	int result = 0;
	for (auto it = EModules_begin(); it != EModules_end(); ++it)
		result += (*it)->getHull().getHitPoints(game::enHitPointType::HP_STRUCTURAL).maxHitPoints();
	return result;
}
int UnitModuleContainer::GetMaterialHitPoints() const
{
	int result = 0;
	for (auto it = EModules_begin(); it != EModules_end(); ++it)
		result += (*it)->getHull().getHitPoints(game::enHitPointType::HP_MATERIAL).maxHitPoints();
	return result;
}

bool UnitModuleContainer::IsLauncherType(enModuleType type)
{
	return type == enModuleType::mLauncher || 
		type == enModuleType::mMissileLauncher || 
		type == enModuleType::mAntiMLauncher ||
		type == enModuleType::mTorpedoLauncher || 
		type == enModuleType::mRailgunLauncher;
}
bool UnitModuleContainer::IsExternalType(enModuleType type)
{
	return type == enModuleType::mWing || 
		type == enModuleType::mEngineBody || 
		type == enModuleType::mFrame || 
		type == enModuleType::mBridge;
}