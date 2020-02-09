#include"InventoryItemGrid.h"


InventoryItemGrid::InventoryItemGrid( spriteSize size, Coord position, spriteSize boxesPerSide  ) : GridObjectViewer( size, position,boxesPerSide )
{
	pItemList = std::map<std::shared_ptr<GridObject>, std::shared_ptr<InventoryItem>>();
	pAllowedItemTypes = std::vector<enItemType>();
	pAllowedItemTypes.push_back(enItemType::ExternalModuleItem);
	pAllowedItemTypes.push_back(enItemType::InternalModuleItem);
	pAllowedItemTypes.push_back(enItemType::ProjectileItem);
}

bool InventoryItemGrid::AddItem( std::shared_ptr<InventoryItem> add)
{
	pItemList[add->getGridObject()] = add;
	return this->AddGridObject( add->getGridObject() );
}

bool InventoryItemGrid::IsItemAllowedForGrid(GridObject* object)
{
	enItemType itemType = this->GetItemTypeFromObject(object);
	return this->IsItemAllowedForGrid(itemType);
}

ObjectIntersectResult InventoryItemGrid::GridObjectIntersectionResult( GridObject* object )
{
	ObjectIntersectResult result;
	if (this->IsItemAllowedForGrid(object) == false)
	{
		result.invalid = true;
		object->setBoxesInvalid();
		return result;
	}

	result = GridObjectViewer::ObjectBoxesIntersectWithObjects(this->selectedGridObject.get());

	bool DoesNotIntersectWithGrid = ColourObjectBasedOnGridIntersection(object);
	if (DoesNotIntersectWithGrid)
		result.invalid = true;

	return result;
}

ObjectIntersectResult InventoryItemGrid::ObjectBoxesIntersect(GridObject* A, GridObject* B)
{
	auto item = this->GetItemTypeFromObject(A);

	if (item != this->GetItemTypeFromObject(B) || item == enItemType::Undefined)
		return ObjectIntersectResult();

	return GridObjectViewer::ObjectBoxesIntersect(A, B); //turns standard from this function
}

bool InventoryItemGrid::ColourObjectBasedOnGridIntersection(GridObject* object)
{
	bool result = false;
	for (auto it = object->begin(); it != object->end(); ++it)
	{
		Coord boxFramePosition = object->GetPositionInGridReferenceFrame() + (*it).first;//object->getRelativePosition() - object->ObjectOrigo + (*it).first;
		if (!this->CoordHasBox(boxFramePosition))
		{
			(*it).second->setState(enBoxState::INVALID);
			result = true;
		}
		else if ((*it).second->getBoxState() != enBoxState::INVALID)
			(*it).second->setState(enBoxState::STANDARD);
	}
	return result;
}

void InventoryItemGrid::SetAllowedItemType(enItemType type)
{
	pAllowedItemTypes.clear();
	pAllowedItemTypes.push_back(type);
}
void InventoryItemGrid::SetAllowedItemTypes(std::vector<enItemType> types)
{
	pAllowedItemTypes = types;
}

DynamicGridBox* InventoryItemGrid::GetNewGridBox()
{
	return new DynamicGridBox(GBTextureCollection::ImportGrid_Shop.get());
}

bool InventoryItemGrid::AddNewItem(std::shared_ptr<InventoryItem> item)
{
	return this->AddItem(std::shared_ptr<InventoryItem>(item));
}

void InventoryItemGrid::RemoveAllGridObjects()
{
	this->ResetBoxStates();
	this->pGridObjects.clear();
	this->pItemList.clear();
}

bool InventoryItemGrid::AddNewItem(std::shared_ptr<InventoryItem> item, Coord pos)
{
	item->getGridObject()->setProperPosition(pos);
	return this->AddItem(std::shared_ptr<InventoryItem>(item));
}

bool InventoryItemGrid::AddNewItem(InventoryItemBundle* item, Coord pos) 
{
	for (auto it = item->begin(); it != item->end(); ++it)
	{
		this->AddItem(*it);
		//pItemList.push_back(*it);
		//(*it)->getGridObject()->setProperPosition(pos);
	}
	item->GetMainItem()->getGridObject()->setProperPosition(pos);
	return true;
}

//bool InventoryItemGrid::AddGridObject(std::shared_ptr<InventoryItem> object, const Coord & pos)
//{
//
//}

bool InventoryItemGrid::AddGridObject(std::shared_ptr<GridObject> object)
{
 	auto itemType = this->GetItemTypeFromObject(object);
	if (itemType == enItemType::Undefined)
		return false;

	return GridObjectViewer::AddGridObject(object);
}

std::list<std::shared_ptr<GridObjectContainer>> InventoryItemGrid::GetGridObjectContainerFromFrame()
{
	std::list<std::shared_ptr<GridObjectContainer>> result = std::list<std::shared_ptr<GridObjectContainer>>();
	for (auto it = this->pItemList.cbegin(); it != this->pItemList.cend(); it++)
	{
		result.push_back(std::shared_ptr<GridObjectItemContainer>(new GridObjectItemContainer((*it).second)));
	}
	return result;
}

bool InventoryItemGrid::IsItemAllowedForGrid(enItemType type)
{
	for (int i = 0; i < this->pAllowedItemTypes.size(); i++)
		if (this->pAllowedItemTypes[i] == type)
			return true;
	return false;
}

enItemType InventoryItemGrid::GetItemTypeFromObject(std::shared_ptr<GridObject> object)
{
	return GetItemTypeFromObject(object.get());
}

enItemType InventoryItemGrid::GetItemTypeFromObject(GridObject* object)
{
	auto item = GetItemFromGridObject(object);
	if (item == nullptr)
		return enItemType::Undefined;
	return item->getType();
}

std::shared_ptr<InventoryItem> InventoryItemGrid::GetItemFromGridObject(GridObject* object)
{
	for (auto it = this->pItemList.cbegin(); it != this->pItemList.cend(); it++)
	{
		if (it->first.get() == object)
			return it->second;
	}
	return nullptr;
}


std::weak_ptr<ITransferObject> InventoryItemGrid::GetSelectedGridObject()
{
	return this->GetItemFromGridObject(selectedGridObject.get());
}

void InventoryItemGrid::Render( SDL_Renderer* renderer )
{
	RenderGridBase( renderer );
	RenderGridObjects( renderer );
	RenderSelectedGridObject(renderer);
}
