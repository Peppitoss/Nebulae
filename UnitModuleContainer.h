#pragma once
#include"ExternalModule.h"
#include"HitBoxEntity.h"
#include"LauncherEntity.h"
#include"LauncherEntity.h"
#include"EngineEntity.h"
#include"RadarEntity.h"
#include"PowerSupplyEntity.h"
#include"ControllerEntity.h"
#include"AmmoContainer.h"
#include"DecoModule.h"
#include"DoorModule.h"
#include"VectorHelper.cpp"
#include<map>

//using Coord;
//class DecoModule;

class UnitModuleContainer
{

public:
	UnitModuleContainer();

	std::vector<std::shared_ptr<LauncherEntity>>::const_iterator const LE_begin() const { return pWeaponPlatforms.begin(); };
	std::vector<std::shared_ptr<LauncherEntity>>::const_iterator const LE_end() const { return pWeaponPlatforms.end(); };

	std::vector<std::shared_ptr<ExternalModule>>::const_iterator const EModules_begin() const { return pExtModList.begin(); };
	std::vector<std::shared_ptr<ExternalModule>>::const_iterator const EModules_end() const { return pExtModList.end(); };

	std::vector<std::shared_ptr<HitBoxEntity>>::const_iterator const HB_begin() const { return pHitBoxList.begin(); };
	std::vector<std::shared_ptr<HitBoxEntity>>::const_iterator const HB_end() const { return pHitBoxList.end(); };

	std::vector<std::shared_ptr<EngineEntity>>::const_iterator const EE_begin() const { return pEngines.begin(); };
	std::vector<std::shared_ptr<EngineEntity>>::const_iterator const EE_end() const { return pEngines.end(); };

	std::vector<std::shared_ptr<RadarEntity>>::const_iterator const RE_begin() const { return pRadars.begin(); };
	std::vector<std::shared_ptr<RadarEntity>>::const_iterator const RE_end() const { return pRadars.end(); };

	std::vector<std::shared_ptr<PowerSupplyEntity>>::const_iterator const PSE_begin() const { return pPowerSupplies.begin(); };
	std::vector<std::shared_ptr<PowerSupplyEntity>>::const_iterator const PSE_end() const { return pPowerSupplies.end(); };

	std::vector<std::shared_ptr<ControllerEntity>>::const_iterator const CE_begin() const { return pControllers.begin(); };
	std::vector<std::shared_ptr<ControllerEntity>>::const_iterator const CE_end() const { return pControllers.end(); };

	std::vector<std::shared_ptr<AmmoContainer>>::const_iterator const AC_begin() const { return pAmmos.begin(); };
	std::vector<std::shared_ptr<AmmoContainer>>::const_iterator const AC_end() const { return pAmmos.end(); };

	std::vector<std::shared_ptr<ModuleEntity>>::const_iterator const Detached_begin() const { return pDetachedModules.begin(); };
	std::vector<std::shared_ptr<ModuleEntity>>::const_iterator const Detached_end() const { return pDetachedModules.end(); };

	std::vector<std::shared_ptr<DecoModule>>::const_iterator const Deco_begin() const { return pDecorationModules.begin(); };
	std::vector<std::shared_ptr<DecoModule>>::const_iterator const Deco_end() const { return pDecorationModules.end(); };

	std::vector<std::shared_ptr<DoorModule>>::const_iterator const Door_begin() const { return pDoorModules.begin(); };
	std::vector<std::shared_ptr<DoorModule>>::const_iterator const Door_end() const { return pDoorModules.end(); };

	int DetachedModulesCount() const { return this->pDetachedModules.size(); }

	void AddModuleToList(std::shared_ptr<ModuleEntity> module, bool AddToExternalModule );
	void AddExtModuleToList(std::shared_ptr<ExternalModule> extModule );
	void AddHitBoxToList(std::shared_ptr<HitBoxEntity> hbModule );
	void AddDetachedModuleToList(std::shared_ptr<ModuleEntity> dModule);
	void AddLauncherToList(std::shared_ptr<LauncherEntity> lModule, bool AddToExternalModule);
	void AddDecorationToList(std::shared_ptr<DecoModule> dModule );
	void AddDoorToList(std::shared_ptr<DoorModule> dModule);

	bool ContainsInternalModule(std::shared_ptr<ModuleEntity> entity);
	bool ContainsGridObject(std::shared_ptr<GridObject> object, bool AllowDetached);

	int GetModulesAmount(enModuleType type) const;

	bool RemoveModuleEntity(std::shared_ptr<ModuleEntity> in);
	bool RemoveLauncherEntity(std::shared_ptr<LauncherEntity> in);
	bool RemoveExternalEntity(std::shared_ptr<ExternalModule> extModule);
	void RemoveDetachedEntity(std::vector<int> indices);//, bool resetArgumentPointer );

	void UpdateContainedListOfExternalModules();

	void SetUnitCenter(const Coord set) { this->pUnitCenter = set; }
	Coord GetUnitCenter() { return this->pUnitCenter; }

	std::shared_ptr<ModuleEntity> GetModulesWithGridObject(const std::shared_ptr<GridObject> object);
	std::shared_ptr<ModuleEntity> GetModulesFromID(enModuleType type, int ID);
	ModuleEntity* GetModuleFromCoord(const Coord&, bool allowDetached);
	std::shared_ptr<ExternalModule> GetExternalModuleFromCoord(const Coord&, bool allowDetached);
	std::vector<std::shared_ptr<ModuleEntity>> GetInternalModulesList() const;
	std::vector<std::shared_ptr<ExternalModule>> GetExternalModulesList(bool allowDetached) const;
	std::vector<std::shared_ptr<AmmoContainer>> GetAmmoContainerList(bool allowDetached) const;
	
	std::vector<std::shared_ptr<ModuleEntity>> GetModulesContainedInExternalModule(std::shared_ptr<ExternalModule> extModule);
	Coord TransformPositionToLinkFrame(const Coord& pos, ModuleEntity& module);

	//template<typename  T> bool RemoveItemFromVector(std::vector<T>&, T);
	//template<typename  T> void RemoveItemsFromVector(std::vector<T>&, std::vector<int> remaining);

	int GetStructuralHitPoints() const;
	int GetMaterialHitPoints() const;

	static bool IsLauncherType(enModuleType type);
	static bool IsExternalType(enModuleType type);

private:

	bool AddModuleToExternalModule(std::shared_ptr<ModuleEntity>);

	std::vector<std::shared_ptr<ExternalModule>> pExtModList;
	std::vector<std::shared_ptr<HitBoxEntity>> pHitBoxList;
	std::vector<std::shared_ptr<ModuleEntity>> pDetachedModules;

	std::vector<std::shared_ptr<DecoModule>> pDecorationModules;
	std::vector<std::shared_ptr<DoorModule>> pDoorModules;
	std::vector<std::shared_ptr<LauncherEntity>> pWeaponPlatforms;
	std::vector<std::shared_ptr<EngineEntity>> pEngines;
	std::vector<std::shared_ptr<RadarEntity>> pRadars;
	std::vector<std::shared_ptr<PowerSupplyEntity>> pPowerSupplies;
	std::vector<std::shared_ptr<ControllerEntity>> pControllers;
	std::vector<std::shared_ptr<AmmoContainer>> pAmmos;

	bool IsModuleInCoord(const std::shared_ptr<ModuleEntity>&, const Coord&, bool allowDetached);

	Coord pUnitCenter;

};

template bool RemoveItemFromVector<std::shared_ptr<ExternalModule>>(std::vector<std::shared_ptr<ExternalModule>>&, std::shared_ptr<ExternalModule>);
template bool RemoveItemFromVector<std::shared_ptr<HitBoxEntity>>(std::vector<std::shared_ptr<HitBoxEntity>>&, std::shared_ptr<HitBoxEntity>);
template bool RemoveItemFromVector<std::shared_ptr<AmmoContainer>>(std::vector<std::shared_ptr<AmmoContainer>>&, std::shared_ptr<AmmoContainer>);
template bool RemoveItemFromVector<std::shared_ptr<LauncherEntity>>(std::vector<std::shared_ptr<LauncherEntity>>&, std::shared_ptr<LauncherEntity>);
template bool RemoveItemFromVector<std::shared_ptr<EngineEntity>>(std::vector<std::shared_ptr<EngineEntity>>&, std::shared_ptr<EngineEntity>);
template bool RemoveItemFromVector<std::shared_ptr<RadarEntity>>(std::vector<std::shared_ptr<RadarEntity>>&, std::shared_ptr<RadarEntity>);
template bool RemoveItemFromVector<std::shared_ptr<PowerSupplyEntity>>(std::vector<std::shared_ptr<PowerSupplyEntity>>&, std::shared_ptr<PowerSupplyEntity>);
template bool RemoveItemFromVector<std::shared_ptr<ControllerEntity>>(std::vector<std::shared_ptr<ControllerEntity>>&, std::shared_ptr<ControllerEntity>);
template bool RemoveItemFromVector<std::shared_ptr<DecoModule>>(std::vector<std::shared_ptr<DecoModule>>&, std::shared_ptr<DecoModule>);
template bool RemoveItemFromVector<std::shared_ptr<DoorModule>>(std::vector<std::shared_ptr<DoorModule>>&, std::shared_ptr<DoorModule>);

template void RemoveItemsFromVector<std::shared_ptr<ExternalModule>>(std::vector<std::shared_ptr<ExternalModule>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<HitBoxEntity>>(std::vector<std::shared_ptr<HitBoxEntity>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<AmmoContainer>>(std::vector<std::shared_ptr<AmmoContainer>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<LauncherEntity>>(std::vector<std::shared_ptr<LauncherEntity>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<EngineEntity>>(std::vector<std::shared_ptr<EngineEntity>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<RadarEntity>>(std::vector<std::shared_ptr<RadarEntity>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<PowerSupplyEntity>>(std::vector<std::shared_ptr<PowerSupplyEntity>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<ControllerEntity>>(std::vector<std::shared_ptr<ControllerEntity>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<DecoModule>>(std::vector<std::shared_ptr<DecoModule>>&, std::vector<int>);
template void RemoveItemsFromVector<std::shared_ptr<DoorModule>>(std::vector<std::shared_ptr<DoorModule>>&, std::vector<int>);
