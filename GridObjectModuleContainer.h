#pragma once
#include"GridObjectContainer.h"
#include"ModuleEntity.h"



class GridObjectModuleContainer : public GridObjectContainer
{
public:

	GridObjectModuleContainer();
	GridObjectModuleContainer( std::shared_ptr<ModuleEntity> module );
	GridObjectModuleContainer( GridObjectModuleContainer& );
	GridObjectModuleContainer( GridObjectModuleContainer&& );
	
	GridObjectModuleContainer& operator=(const GridObjectModuleContainer& other);
	GridObjectModuleContainer& operator=(const GridObjectModuleContainer&& other);

	void AddGridObject( std::shared_ptr<ModuleEntity> object );
	std::shared_ptr<ModuleEntity> GetModule();

protected:

	std::shared_ptr<ModuleEntity> pModuleEntity;
	void Initialize();
};