#pragma once
//#include"GameManager.h"
#include"scrGridObject.h"
#include"InventoryItemGrid.h"
#include"InventoryItem.h"
#include"GridObjectItemContainer.h"

class ItemFrame : public GridObjectFrame
{

public:

	ItemFrame();
	ItemFrame( spriteSize, Coord pos );

	void ResizeGrid(spriteSize);
	virtual void render( SDL_Renderer& );
	//virtual void HandleEvent( SDL_Event& e);

	virtual void btnClose();
	void HandleActiveWidget( SDL_Event& e, Widget* widget );


	InventoryItemGrid* GetGrid();
	bool AddGridObject(std::shared_ptr<ITransferObject> object, const Coord & pos) override;

protected:

	void Setup( spriteSize, Coord pos );
	//std::list<std::shared_ptr<InventoryItem>> itemList; //container for safe ptr delete
};
