#pragma once
#include"GridObjectViewer.h"
#include"GridObjectItemContainer.h"

class InventoryItemGrid : public GridObjectViewer
{
public:

	InventoryItemGrid( spriteSize size, Coord position, spriteSize boxesPerSide  ); 

	void Render( SDL_Renderer* ) override;

	std::weak_ptr<ITransferObject> GetSelectedGridObject() override;
	const std::map<std::shared_ptr<GridObject>, std::shared_ptr<InventoryItem>>& GetItemList() { return this->pItemList; };

	void RemoveAllGridObjects() override;
	
	//bool AddGridObject(std::shared_ptr<InventoryItem> object, const Coord & pos);
	bool AddGridObject(std::shared_ptr<GridObject>) override;
	bool AddNewItem(std::shared_ptr<InventoryItem> item); //return false, if all place is occupied
	bool AddNewItem(std::shared_ptr<InventoryItem> item, Coord pos);
	bool AddNewItem(InventoryItemBundle* item, Coord pos);
	std::list<std::shared_ptr<GridObjectContainer>> GetGridObjectContainerFromFrame();

	void SetAllowedItemType(enItemType type);
	void SetAllowedItemTypes(std::vector<enItemType> types);
	std::vector<enItemType> GetAllowedItemTypes() { return this->pAllowedItemTypes; }

protected:

	DynamicGridBox* GetNewGridBox() override;
	std::shared_ptr<imagehandling::IMGToSurface> HighligthTexture() override { { return GBTextureCollection::ImportGrid_ShopSelected; } }

	enItemType GetItemTypeFromObject(std::shared_ptr<GridObject>);
	enItemType GetItemTypeFromObject(GridObject*);

	bool IsItemAllowedForGrid(GridObject* object);
	std::shared_ptr<InventoryItem> GetItemFromGridObject(GridObject*);

	void RenderItems(SDL_Renderer*);
	ObjectIntersectResult GridObjectIntersectionResult(GridObject* A) override;
	//ObjectIntersectResult ObjectBoxesIntersectWithObjects( GridObject* object ) ;
	bool ColourObjectBasedOnGridIntersection(GridObject* object);	//returns true if object not within grid
	ObjectIntersectResult ObjectBoxesIntersect(GridObject* A, GridObject* B) override;

	virtual bool IsItemAllowedForGrid(enItemType type);

private:

	bool AddItem(std::shared_ptr<InventoryItem>);
	std::map<std::shared_ptr<GridObject>, std::shared_ptr<InventoryItem>> pItemList; //container for safe ptr delete
	std::vector<enItemType> pAllowedItemTypes;

};
