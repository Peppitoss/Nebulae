#pragma once
#include"GridObject.h"
#include"GridObjectItem.h"
#include"Globals.h"
#include"EntityPrice.h"

using std::string;


enum enItemType
{
	Undefined = 0,
	InternalModuleItem = 1,
	ExternalModuleItem = 2,
	ProjectileItem = 3
};

class InventoryItem : public ITransferObject
{
public:
	
	InventoryItem( enItemType );
	InventoryItem(GridObject* objectGraphics, enItemType type);

	virtual void generateGridObjectForItem(const BoxList& gridPos);
	std::shared_ptr<GridObject> getGridObject() { return pGridObject; };
	bool hasGridObject() const { return pGridObject.get(); }

	virtual void generateGridObjectToolTip(std::string tooltip);
	virtual void generateGridObjectDefaultToolTip() {};

	virtual void IncludeSellValueInTooltip(bool value);
	virtual void IncludePriceValueInTooltip(bool value);
	
	void SetGridObjectPosition(Coord set, bool saveNewPosition);
	enItemType getType() { return pType; };
	void SetItemType(enItemType type) { this->pType = type; }

	EntityPrice GetPrice() const { return this->pPrice; }
	void SetPrice(statistics::RandomNumber newPrice) { this->pPrice.SetNewPrice(newPrice); }
	

protected:

	
	Coord GridObjectPosition() const { return pGridObject->getLastPlacedPosition(); };

	int GetGridObjectSize() { return getGridObject()->boxSize(); };

	virtual void initialize(GridObject* graphicObject);
	void initialize();

	EntityPrice pPrice;
	std::shared_ptr<GridObject> pGridObject;
	bool pIncludePriceInTooltip;
	bool pIncludeSellValueInTooltip;

private:

	enItemType pType;

	//string pDescription;
	
};

class InventoryItemBundle //automatically generate a stack of items
{

public:

	InventoryItemBundle( enItemType, int BundleSize );

	std::shared_ptr<InventoryItem> GetMainItem() { return pBundleHolder; };

	std::list<std::shared_ptr<InventoryItem>>::const_iterator const begin() const { return pBundle.begin(); }
    std::list<std::shared_ptr<InventoryItem>>::const_iterator const end() const { return pBundle.end(); }

private:

	std::shared_ptr<InventoryItem> pBundleHolder;
	std::list<std::shared_ptr<InventoryItem>> pBundle;

};


