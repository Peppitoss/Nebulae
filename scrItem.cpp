#include"scrItem.h"
#include"GameManager.h"
#include"ScreenButton.h"


ItemFrame::ItemFrame() : GridObjectFrame()
{
	
}

ItemFrame::ItemFrame( spriteSize size, Coord pos ) : GridObjectFrame( size, pos )
{
	this->Setup( size, pos );
}

void ItemFrame::Setup( spriteSize size, Coord pos )
{
	int screen_thickness = size.GetSize();
	int spacing = screen_thickness/10;
	int rect_h = STANDARD_BUTTON_HEIGHT;
	int rect_w = 95; //5*19
	int x_coord = pos.x + size.GetSize() - rect_w - 32;
	int y_coord = pos.y + size.GetSize() - 32-32;
	Coord offset = Coord(32,32);

	pGrid = std::unique_ptr<Grid>( new InventoryItemGrid( size, Coord(screenPosition.x+offset.x,screenPosition.y+offset.y), spriteSize( spriteSize::GetSize64() ))); //could be placed a bit different than the screen position, depending on screen layout
	((InventoryItemGrid*)pGrid.get())->generateGridRows( 12,8, enBoxState::STANDARD ); //size originially ( 12,8 )

	
	auto play = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord, y_coord), rect_w, rect_h, BtnText("Close")));
	play->OnWidgetClicked = [](Widget*) {GameManager::Screens.closeLootingScreen(); };
	this->Control()->AddWidget(play);

	//this->AddNewItem( new InventoryItem( enItemType::ScrapBox_1), Coord(2,2) ); //no delete on this item!! will leak leak leak, find an item handler (this is handled now??)
	//this->AddNewItem( new InventoryItem( enItemType::ScrapBox_1), Coord(4,2) );

	Screen::SetBackground("256x256GreyBack.png", EnumColorKey::kCYAN);
}

void ItemFrame::ResizeGrid(spriteSize size)
{
	auto allowedItemTypes = ((InventoryItemGrid*)pGrid.get())->GetAllowedItemTypes();

	Coord offset = Coord(32, 32);
	pGrid = std::unique_ptr<Grid>(new InventoryItemGrid(pGrid->getSpriteSize(), Coord(screenPosition.x + offset.x, screenPosition.y + offset.y), spriteSize().GetSize16())); //could be placed a bit different than the screen position, depending on screen layout
	((InventoryItemGrid*)pGrid.get())->generateGridRows(size.width(), size.height(), enBoxState::STANDARD); //size originially ( 12,8 )
	((InventoryItemGrid*)pGrid.get())->SetAllowedItemTypes(allowedItemTypes);
}

void ItemFrame::render( SDL_Renderer& renderer )
{
	GridFrame::render(renderer);
}

void ItemFrame::HandleActiveWidget( SDL_Event& e, Widget* widget )
{
	GridFrame::HandleActiveWidget(e,widget);
	std::string txt = widget->ID();
	
	if(txt == "Close" )
	{
		this->btnClose();
	}
}

//
//std::list<std::shared_ptr<GridObjectContainer>> ItemFrame::GetGridObjectContainerFromFrame()
//{
//	std::list<std::shared_ptr<GridObjectContainer>> result = std::list<std::shared_ptr<GridObjectContainer>>();
//	for( std::list<std::shared_ptr<InventoryItem>>::const_iterator it = this->GetGrid()->GetItemList().cbegin(); it != this->GetGrid()->GetItemList().cend(); it++ )
//	{
//		result.push_back( std::shared_ptr<GridObjectItemContainer>(new GridObjectItemContainer( (*it) ) ) );
//	}
//	return result;
//}

void ItemFrame::btnClose()
{
	GameManager::Screens.closeLootingScreen();
}

InventoryItemGrid* ItemFrame::GetGrid()
{
	return (InventoryItemGrid*)(pGrid.get()); 
}

bool ItemFrame::AddGridObject(std::shared_ptr<ITransferObject> object, const Coord & pos)
{
	if (std::shared_ptr<InventoryItem> derived = std::dynamic_pointer_cast<InventoryItem>(object))
	{
		return this->GetGrid()->AddNewItem(derived, pos);
	}
	return AddGridObject(object->getGridObject(), pos);
}
