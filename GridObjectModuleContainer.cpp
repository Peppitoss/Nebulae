#include"GridObjectModuleContainer.h"


GridObjectModuleContainer::GridObjectModuleContainer() : GridObjectContainer()
{
	/*this->ID = ID;*/
	Initialize();
}

GridObjectModuleContainer::GridObjectModuleContainer( std::shared_ptr<ModuleEntity> module )
{
	this->AddGridObject( module );
	Initialize();
}

GridObjectModuleContainer::GridObjectModuleContainer( GridObjectModuleContainer& other ) : GridObjectContainer(other)
{
	//this->ID = other.ID;
	//this->pType = other.pType;
	//this->pGObject = other.pGObject;
	this->pModuleEntity = other.pModuleEntity;
}

GridObjectModuleContainer::GridObjectModuleContainer( GridObjectModuleContainer&& other ) : GridObjectContainer(other)
{
	//this->ID = std::move(other.ID);
	//this->pType = std::move(other.pType);
	//this->pGObject = std::move(other.pGObject);
	this->pModuleEntity = std::move(other.pModuleEntity);
}
	
GridObjectModuleContainer& GridObjectModuleContainer::operator=(const GridObjectModuleContainer& other )
{
	//this->ID = other.ID;
	this->pType = other.pType;
	this->pGObject = other.pGObject;
	this->pModuleEntity = other.pModuleEntity;
	return *this;
}
GridObjectModuleContainer& GridObjectModuleContainer::operator=(const GridObjectModuleContainer&& other )
{
	//this->ID = std::move(other.ID);
	this->pType = std::move(other.pType);
	this->pGObject = std::move(other.pGObject);
	this->pModuleEntity = std::move(other.pModuleEntity);
	return *this;
}

void GridObjectModuleContainer::AddGridObject( std::shared_ptr<ModuleEntity> object )
{
	this->pGObject = object->getGridObject();
	this->pModuleEntity = object;
}

std::shared_ptr<ModuleEntity> GridObjectModuleContainer::GetModule()
{
	return this->pModuleEntity;
}

void GridObjectModuleContainer::Initialize()
{
	pType = enGOContainerType::Module;
}