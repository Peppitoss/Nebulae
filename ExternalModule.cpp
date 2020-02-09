#include"ExternalModule.h"
#include"Statistics.h"
#include"EngineEntity.h"

ExternalModule::ExternalModule() : ModuleEntity()
{
	Initialize();
}
ExternalModule::ExternalModule( std::shared_ptr<GraphicalEntity> graphicalentity, string name) : ModuleEntity(graphicalentity, name)
{
	Initialize();
}

void ExternalModule::AddContainedModule(std::shared_ptr<ModuleEntity> mod)
{
	if (this->ContainsModule(mod->GetModuleType(), mod->GetID()) == false)
	{
		if (std::shared_ptr<EngineEntity> derived = std::dynamic_pointer_cast<EngineEntity>(mod))
		{
			this->pProperties.Thrust = derived->GetThrust();
			this->pProperties.AngularThrust = derived->GetAngularThrust();
		}
		this->pContainedModules.push_back(mod);
	}
}

void ExternalModule::generateGridObjectDefaultToolTip()
{
	string tooltip = "";
	tooltip = this->GetIDName() + "\n";
	this->generateGridObjectToolTip(tooltip);
}

//void ExternalModule::UpdateProperties(std::shared_ptr<ModuleEntity> module)
//{
//	if (this->ContainsModule(mod->GetModuleType(), mod->GetID()) == false)
//	{
//		if (std::shared_ptr<EngineEntity> derived = std::dynamic_pointer_cast<EngineEntity>(mod))
//		{
//			this->pProperties.Thrust = derived->GetThrust();
//			this->pProperties.AngularThrust = derived->GetAngularThrust();
//		}
//		this->pContainedModules.push_back(mod);
//	}
//}

bool ExternalModule::ContainsModule(enModuleType type, int moduleID)
{
	for (auto it = this->pContainedModules.begin(); it != this->pContainedModules.end(); ++it)
	{
		if ((*it)->GetID() == moduleID && (*it)->GetModuleType() == type)
			return true;
	}
	return false;
}

void ExternalModule::Initialize()
{
	this->pCanDetach = true;
	this->SetItemType(enItemType::ExternalModuleItem);
}

bool ExternalModule::ContainsModules() const
{
	return pContainedModules.size() > 0;
}

void ExternalModule::ClearContainedModuleList()
{
	this->pContainedModules.clear();
}

void ExternalModule::AddDamageRecieved(const game::Damage& set)
{
	ModuleEntity::AddDamageRecieved(set);
	DamageRandomContainedModule(set.ReduceDamage(0.5));	// should not be random, but based on module sizes

}

void ExternalModule::DamageRandomContainedModule(const game::Damage& dmg)
{
	if (ContainsModules())
	{
		int ThisBoxAmount = this->GetGridBoxesInFigure();

		int roll100 = statistics::Roll_100();

		for (auto it = pContainedModules.begin(); it != pContainedModules.end(); ++it)
		{
			double ratio = (double)((*it)->GetGridBoxesInFigure()) / ThisBoxAmount;
			if(ratio*100 > roll100)
				DamageContainedModule(*it, dmg);
		}
	}
}
void ExternalModule::DamageContainedModule(std::shared_ptr<ModuleEntity> mod, const game::Damage& dmg)
{
	//TODO damaged module here changes efficiency, this has to be updated for vessel (currently vessel only change attributes if module is added or removed)
	mod->AddDamageRecieved(dmg);
}

void ExternalModule::RemoveAllContainedModules()
{
	for (auto it = this->pContainedModules.begin(); it != this->pContainedModules.end(); ++it)
	{
		it = this->pContainedModules.erase(it);
	}
}

ExternalModuleStoredProperties ExternalModule::GetProperties()
{
	return this->pProperties;
}

ExternalModuleStoredProperties::ExternalModuleStoredProperties()
{
	this->Thrust = 0;
	this->AngularThrust = 0;
}
//ExternalModuleStoredProperties::ExternalModuleStoredProperties(int thrust)
//{
//	this->Thrust = thrust;
//}
