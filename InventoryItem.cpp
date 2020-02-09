#include"InventoryItem.h"
#include"GridObjectItem.h"
#include"StringBuilder.h"

InventoryItem::InventoryItem( enItemType type )
{
	this->pType = type;
	initialize();
}

//InventoryItem::InventoryItem(GridObjectItem* graphicObject )
//{
//	this->pType = enItemType::Undefined;
//	initialize(graphicObject);
//}

InventoryItem::InventoryItem(GridObject* objectGraphics, enItemType type)
{
	this->pType = type;
	initialize();
	initialize(objectGraphics);
}

void InventoryItem:: SetGridObjectPosition(Coord set, bool saveNewPosition)
{
	if(saveNewPosition)
		pGridObject->setProperPosition(set);
	else
		pGridObject->setRelativePosition(set);
}

void InventoryItem::generateGridObjectForItem(const BoxList& gridPos)
{
	pGridObject = std::shared_ptr<GridObject>(new GridObject(gridPos));
}

void InventoryItem::generateGridObjectToolTip(std::string tooltip)
{
	if (pGridObject != NULL)
	{
		if (pIncludePriceInTooltip)
			tooltip += "\n\n" + Text("Price: ").GetCurrentValue() + DoubleToString(this->GetPrice().PriceValue().Average, 10);
		else if (pIncludeSellValueInTooltip)
			tooltip += "\n\n" + Text("Sell value: ").GetCurrentValue() + DoubleToString(this->GetPrice().SellValue().Average, 10);

		pGridObject.get()->setToolTipText(tooltip);
	}
}

void InventoryItem::IncludeSellValueInTooltip(bool value)
{
	if (this->pIncludeSellValueInTooltip == value)
		return;

	this->pIncludeSellValueInTooltip = value;
	if (value)
		this->pIncludePriceInTooltip = !value;

	generateGridObjectDefaultToolTip();
}
void InventoryItem::IncludePriceValueInTooltip(bool value)
{
	if (this->pIncludePriceInTooltip == value)
		return;

	this->pIncludePriceInTooltip = value;
	if (value)
		this->pIncludeSellValueInTooltip = !value;

	generateGridObjectDefaultToolTip();
}

//InventoryItem::InventoryItem(GridObjectItem* graphicObject, std::string description )
//{
//	this->pType = enItemType::Undefined;
//	initialize(graphicObject,description);
//}

void InventoryItem::initialize(GridObject* graphicObject)
{
	this->pGridObject = std::shared_ptr<GridObject>(graphicObject);
	//this->pDescription = description;
}

void InventoryItem::initialize()
{
	this->pIncludePriceInTooltip = false;
	this->pIncludeSellValueInTooltip = false;
}

InventoryItemBundle::InventoryItemBundle( enItemType type, int BundleSize )
{
	for( int i = 0; i< BundleSize; i++ )
	{
		this->pBundle.push_back( std::shared_ptr<InventoryItem>(new InventoryItem( type )) );
	}

	pBundleHolder = *pBundle.begin();

	if( BundleSize > 1 )
	{

		for( auto it = std::next(pBundle.begin()); it != pBundle.end(); ++it)
		{
			pBundleHolder->getGridObject()->addGridObjectToBundle( (*it)->getGridObject().get() );
			//(*it)->getGridObject()->setLastPlacedPosition( (*it)->getGridObject()->getRelativePosition() );
		}
	}

}