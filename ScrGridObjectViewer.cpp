#include"ScrGridObjectViewer.h"
#include"ModuleInterfaceGlobalList.h"
#include"GameManager.h"

GridObjectViewerFrame::GridObjectViewerFrame() : GridObjectFrame()
{
}

GridObjectViewerFrame::GridObjectViewerFrame( spriteSize size, Coord pos ) : GridObjectFrame(size,pos)
{
	this->Setup( size, pos );
}

void GridObjectViewerFrame::init()
{
	nameOfCurrentShownEntity = "";
	pAcceptTransfers = true;
	pRegenerateObjects = false;
}

void GridObjectViewerFrame::Setup( spriteSize size, Coord pos )
{
	int screen_thickness = size.GetSize();
	int spacing = screen_thickness/10;
	int rect_h = STANDARD_BUTTON_HEIGHT;
	int rect_w = 95; //5*19
	int x_coord = pos.x + size.GetSize() - rect_w - 32;
	int y_coord = pos.y + size.GetSize() - 32-32;
	Coord offset = Coord(32,32);

	pGrid = std::unique_ptr<Grid>( new GridObjectViewer( size, Coord(screenPosition.x+offset.x,screenPosition.y+offset.y), spriteSize( spriteSize::GetSize32() )));
	((GridObjectViewer*)pGrid.get())->generateGridRows( 24,24, enBoxState::STANDARD );


	Screen::SetBackground("256x256GreyBack.png", EnumColorKey::kCYAN);

}

void GridObjectViewerFrame::RemoveSelectedGridObject()
{
	GridObjectFrame::RemoveSelectedGridObject();
	if( this->pRegenerateObjects )
	{
		this->GetGrid()->RemoveAllGridObjects();
		GameManager::loadModuleBuilder(this->nameOfCurrentShownEntity); 
	}
	//else if( !this->pAcceptTransfers )
	//{
	//	this->GetGrid()->RemoveAllGridObjects();
	//}
}

bool GridObjectViewerFrame::AddGridObject( std::shared_ptr<GridObject> object )
{
	if( !this->pAcceptTransfers )
	{
		if( this->pRegenerateObjects )
		{
			GameManager::loadModuleBuilder(this->nameOfCurrentShownEntity);
			return true;
		}
		return true;
	}
	else
	{
		return GridObjectFrame::AddGridObject( object );
	}
	return false;
}
bool GridObjectViewerFrame::AddGridObject( std::shared_ptr<GridObject> object, const Coord & pos )
{
	if( !this->pAcceptTransfers )
	{
		if( this->pRegenerateObjects )
		{
			GameManager::loadModuleBuilder(this->nameOfCurrentShownEntity);
			return true;
		}
		return true;
	}
	else
	{
		return GridObjectFrame::AddGridObject( object, pos );
	}
	return false;
}

void GridObjectViewerFrame::AddNewViewedGridObject( std::string globalListName )
{
	auto module = ModuleGlobalList::GetModule(globalListName);//, );
	module->GenerateModuleGridBox(this->GetGrid()->getSize() / this->GetGrid()->getDescaleFactor());
	if (module->getGridObject().get())
	{
		AddNewViewedGridObject(module);// , this->GetGrid()->getSize() / this->GetGrid()->getDescaleFactor()));
	}
	this->nameOfCurrentShownEntity = globalListName;
}

void GridObjectViewerFrame::AddNewViewedGridObject( ModuleEntity* object)
{
	if (object->getGridObject().get())
	{
		this->currentShownEntity = std::shared_ptr<ModuleEntity>(object);
		this->GetGrid()->RemoveAllGridObjects();
		this->GetGrid()->AddGridObject(object->getGridObject(), Coord(-16));
	}
}
void GridObjectViewerFrame::SetRegenerateObjects( bool set )
{
	this->pRegenerateObjects = set;
}
void GridObjectViewerFrame::SetAccepTransfers( bool set )
{
	this->pAcceptTransfers = set;
}

std::shared_ptr<GridObjectContainer> GridObjectViewerFrame::GetGridObjectContainerFromFrame()
{
	return  std::shared_ptr<GridObjectContainer>(  std::shared_ptr<GridObjectModuleContainer>( new GridObjectModuleContainer( this->currentShownEntity ) ) );
}
