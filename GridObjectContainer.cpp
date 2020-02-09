#include"GridObjectContainer.h"

GridObjectContainer::GridObjectContainer()
{
	//this->ID = ID;
	Initialize();
}

GridObjectContainer::GridObjectContainer( std::shared_ptr<GridObject> object )
{
	this->pGObject = object;
	Initialize();
}

GridObjectContainer::GridObjectContainer( GridObjectContainer& other)
{
	//this->ID = other.ID;
	this->pType = other.pType;
	this->pGObject = other.pGObject;
}

GridObjectContainer::GridObjectContainer( GridObjectContainer&& other)
{
	//this->ID = std::move(other.ID);
	this->pType = std::move(other.pType);
	this->pGObject = std::move(other.pGObject);
}
	
GridObjectContainer& GridObjectContainer::operator=(const GridObjectContainer& other)
{
	//this->ID = other.ID;
	this->pType = other.pType;
	this->pGObject = other.pGObject;
	return *this;
}

GridObjectContainer& GridObjectContainer::operator=(const GridObjectContainer&& other)
{
	//this->ID = std::move(other.ID);
	this->pType = std::move(other.pType);
	this->pGObject = std::move(other.pGObject);
	return *this;
}

void GridObjectContainer::AddGridObject( std::shared_ptr<GridObject> object )
{
	this->pGObject = object;
}

void GridObjectContainer::Initialize()
{
	pType = enGOContainerType::Pure;
}

std::shared_ptr<GridObject> GridObjectContainer::GetGridObject()
{
	return this->pGObject;
}