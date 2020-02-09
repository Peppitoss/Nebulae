#include"scrGridObject.h"

using namespace std::placeholders;

GridObjectFrame::GridObjectFrame() : GridFrame()
{
}

GridObjectFrame::GridObjectFrame( spriteSize size, Coord pos ) : GridFrame(size, pos )
{
	this->Control()->OnCloseScreen = std::bind(&GridObjectFrame::OnCloseScreen, this);
}

void GridObjectFrame::renderGridObjects( SDL_Renderer& renderer )
{
	GetGrid()->RenderGridObjects( &renderer );
}

void GridObjectFrame::renderGridObject( SDL_Renderer& renderer, GridObject* object )
{
	GetGrid()->RenderGridObject( &renderer, object );
}


GridObjectViewer* GridObjectFrame::GetGrid() 
{
	return (GridObjectViewer*)(pGrid.get()); 
}

void GridObjectFrame::HandleEvent( SDL_Event& e)
{
	Screen::HandleEvent(e);
	GetGrid()->handleEvent(e);
}

bool GridObjectFrame::GridSatisfiesUndoTransfer()
{
	return GetGrid()->FLAG_SELECTED_GRIDOBJECT_INVALID_PLACEMENT*GetGrid()->FLAG_GRIDOBJECT_SELECTED_FROM_SEPERATE_GRID;
}

std::weak_ptr<ITransferObject> GridObjectFrame::selectedObject()
{ 
	return this->GetGrid()->GetSelectedGridObject();
}

void GridObjectFrame::setSelectedObject( std::shared_ptr<GridObject> set ) 
{
	GetGrid()->setNewSelectedObject(set);
};

bool GridObjectFrame::AddGridObject( std::shared_ptr<GridObject> object, const Coord & pos )
{
	return this->GetGrid()->AddGridObject(object,pos);
}

bool GridObjectFrame::AddGridObject( std::shared_ptr<GridObject> object )
{
	return this->GetGrid()->AddGridObject(object);
}

bool GridObjectFrame::AddGridObject(std::shared_ptr<ITransferObject> object)
{
	return AddGridObject(object->getGridObject());
}
bool GridObjectFrame::AddGridObject(std::shared_ptr<ITransferObject> object, const Coord & pos)
{
	return AddGridObject(object->getGridObject(),pos);
}

bool GridObjectFrame::AddNewGridObject(std::shared_ptr<ITransferObject> object, const Coord & pos)
{
	return this->GetGrid()->AddNewGridObject(object->getGridObject(), pos);
}

void GridObjectFrame::RemoveGridObject( std::shared_ptr<GridObject> object )
{
	this->GetGrid()->RemoveGridObject( object );
}

void GridObjectFrame::RemoveGridObject(std::shared_ptr<ITransferObject> object)
{
	RemoveGridObject(object->getGridObject());
}

Coord GridObjectFrame::GetGridOrigo()
{
	return this->GetGrid()->Origo();
}

void GridObjectFrame::OnCloseScreen()
{
	this->GetGrid()->ResetSelectedGridObject();
}

Coord GridObjectFrame::GetBoxPositionFromMousePosition()
{
	Coord mousePos;
	SDL_GetMouseState(&mousePos.x, &mousePos.y);
	return this->GetBoxPositionFromMousePosition(mousePos);
}

Coord GridObjectFrame::GetBoxPositionFromMousePosition( const Coord mousepos )
{
	return this->GetGrid()->boxPositionFromMousePosition( mousepos );
}

void GridObjectFrame::RemoveSelectedGridObject()
{
	GetGrid()->RemoveSelectedGridObject();
}

void GridObjectFrame::render( SDL_Renderer& renderer )
{
	renderBackground(renderer);
	this->GetGrid()->Render(&renderer);
	renderGridObjects(renderer);
}

void GridObjectFrame::Setup(spriteSize size, spriteSize gridBoxPerSide)
{
	pGrid = std::unique_ptr<Grid>(new GridObjectViewer( size, screenPosition,gridBoxPerSide )); //could be placed a bit different than the screen position, depending on screen layout
	pGrid->generateGrid( size.GetSize() );
}