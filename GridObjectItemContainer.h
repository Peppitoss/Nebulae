#pragma once
#include"GridObjectContainer.h"
#include"InventoryItem.h"

class GridObjectItemContainer : public GridObjectContainer
{
public:

	GridObjectItemContainer();
	GridObjectItemContainer( std::shared_ptr<InventoryItem> module );
	GridObjectItemContainer( GridObjectItemContainer& );
	GridObjectItemContainer( GridObjectItemContainer&& );
	
	GridObjectItemContainer& operator=(const GridObjectItemContainer& other);
	GridObjectItemContainer& operator=(const GridObjectItemContainer&& other);

	void AddGridObject( std::shared_ptr<InventoryItem> object );
	std::shared_ptr<InventoryItem> GetItem();

protected:

	std::shared_ptr<InventoryItem> pItem;
	void Initialize();
};