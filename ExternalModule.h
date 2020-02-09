#pragma once
#include"ModuleEntity.h"

struct ExternalModuleStoredProperties
{
	int Thrust;
	int AngularThrust;

	ExternalModuleStoredProperties();
	//ExternalModuleStoredProperties(int thrust);
};

class ExternalModule : public ModuleEntity
{
public:
	ExternalModule();
	ExternalModule( std::shared_ptr<GraphicalEntity>, string name);


	void generateGridObjectDefaultToolTip() override;
	void AddContainedModule(std::shared_ptr<ModuleEntity> mod);

	void AddDamageRecieved(const game::Damage& set) override;

	void DamageRandomContainedModule( const game::Damage& );
	void DamageContainedModule(std::shared_ptr<ModuleEntity> mod, const game::Damage&);

	void RemoveAllContainedModules();
	bool ContainsModule(enModuleType type, int moduleID);

	std::vector<std::shared_ptr<ModuleEntity>>::const_iterator const contained_begin() const { return pContainedModules.begin(); };
	std::vector<std::shared_ptr<ModuleEntity>>::const_iterator const contained_end() const { return pContainedModules.end(); };

	//void UpdateProperties(std::shared_ptr<ModuleEntity> module);

	void ClearContainedModuleList();
	bool ContainsModules() const;

	ExternalModuleStoredProperties GetProperties();

	void SetCanDetach(bool set) { this->pCanDetach = set; };
	bool CanDetach() { return this->pCanDetach; }

private:

	bool pCanDetach;
	ExternalModuleStoredProperties pProperties;
	std::vector<std::shared_ptr<ModuleEntity>> pContainedModules;

	void Initialize();

};