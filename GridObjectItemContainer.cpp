#include"GridObjectItemContainer.h"


GridObjectItemContainer::GridObjectItemContainer() : GridObjectContainer()
{
	/*this->ID = ID;*/
	Initialize();
}

GridObjectItemContainer::GridObjectItemContainer( std::shared_ptr<InventoryItem> module )
{
	this->AddGridObject( module );
	Initialize();
	switch (module->getType())
	{
	case(enItemType::ExternalModuleItem):
		this->pType = enGOContainerType::Module;
		break;
	case(enItemType::InternalModuleItem):
		this->pType = enGOContainerType::Module;
		break;
	case(enItemType::ProjectileItem):
		this->pType = enGOContainerType::Item;
		break;
	case(enItemType::Undefined):
		this->pType = enGOContainerType::Pure;
		break;
	}
}

GridObjectItemContainer::GridObjectItemContainer( GridObjectItemContainer& other ) : GridObjectContainer(other)
{
	this->pItem = other.pItem;
}

GridObjectItemContainer::GridObjectItemContainer( GridObjectItemContainer&& other ) : GridObjectContainer(other)
{
	this->pItem = std::move(other.pItem);
}
	
GridObjectItemContainer& GridObjectItemContainer::operator=(const GridObjectItemContainer& other )
{
	this->pType = other.pType;
	this->pGObject = other.pGObject;
	this->pItem = other.pItem;
	return *this;
}
GridObjectItemContainer& GridObjectItemContainer::operator=(const GridObjectItemContainer&& other )
{
	this->pType = std::move(other.pType);
	this->pGObject = std::move(other.pGObject);
	this->pItem = std::move(other.pItem);
	return *this;
}

void GridObjectItemContainer::AddGridObject( std::shared_ptr<InventoryItem> object )
{
	this->pGObject = object->getGridObject();
	this->pItem = object;
}

std::shared_ptr<InventoryItem> GridObjectItemContainer::GetItem()
{
	return this->pItem;
}

void GridObjectItemContainer::Initialize()
{
	//pType = enGOContainerType::Item;
}