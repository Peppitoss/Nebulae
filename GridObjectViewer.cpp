#include"GridObjectViewer.h"
#include"GridObjectItem.h"
#include"GridBoxTextureCollection.h"
#include"SDLKeyboardSyntax.h"
#include"Debug.h"


GridObjectViewer::GridObjectViewer( spriteSize size, Coord pos, spriteSize boxesPerSide ) : Grid( size, pos,boxesPerSide ), pGridObjects()
{
	selectedGridObject.reset();
	pHoveredGridObject.reset();

	FLAG_GRIDOBJECT_SELECTED_FROM_SEPERATE_GRID = 0;
	FLAG_SELECTED_GRIDOBJECT_INVALID_PLACEMENT = 0;
}

void GridObjectViewer::Render( SDL_Renderer* renderer )
{ 
	RenderGridBase(renderer);
	RenderGridObjects(renderer);
}

void GridObjectViewer::RenderGridBase( SDL_Renderer* renderer )
{
	CoordMapIterator<GridBox> begin = this->boxList.Begin();
	CoordMapIterator<GridBox> end = this->boxList.End();

	for( auto it = begin; it != end; ++it )
	{
		DynamicGridBox* ptr = (DynamicGridBox*)(it.item().get());
		if( ((DynamicGridBox*)ptr)->getBoxState() == enBoxState::HIDE )
			continue;

		SDL_Texture* texture = ptr->GetTextureByState(renderer);
		Coord drawPosition = GetBoxDrawPosition(it.Coord());

		int textureSize = GridBox::BOXSIZE/pDescaleFactor;

		SDL_Rect renderQuad = { drawPosition.x, drawPosition.y, textureSize, textureSize};
		SDL_RenderCopy( renderer, texture, NULL, &renderQuad );
		Debug::SDLErrorCheck();

		if( it.item()->ShowIcon() )
			RenderGridBox( it.item()->getIconTexture(renderer), renderer, it.Coord() );
	}
}

void GridObjectViewer::RenderSelectedGridObject(SDL_Renderer* renderer)
{
	if (this->selectedGridObject)
		this->selectedGridObject->render(this->position, renderer, this->pDescaleFactor); // double rendering, some overhead, but will simply draw the selected on top
	else if (this->pHoveredGridObject.lock())
		this->pHoveredGridObject.lock()->render(this->position, renderer, this->pDescaleFactor);
}

DynamicGridBox* GridObjectViewer::GetNewGridBox()
{
	return new DynamicGridBox(GBTextureCollection::ImportGrid_Standard.get());
}

void GridObjectViewer::AddBox( int x, int y, enBoxState state)
{
	DynamicGridBox* newBox = GetNewGridBox();
	newBox->setHighlightTexture(  this->HighligthTexture().get() ); 
	newBox->setState( state );
	boxList.InsertBox( std::shared_ptr<GridBox>(newBox), Coord(x,y));
}


bool GridObjectViewer::AddGridObject( std::shared_ptr<GridObject> add, Coord position)
{
	if (AddGridObject(add))
	{
		add->setRelativePosition(position);
		return true;
	}
	return false;
}

bool GridObjectViewer::AddNewGridObject(std::shared_ptr<GridObject> add, Coord position)
{
	if (AddGridObject(add))
	{
		add->setProperPosition(position);
		return true;
	}
	return false;
}

bool GridObjectViewer::AddGridObject( std::shared_ptr<GridObject> add)
{
	if( this->ContainsGridObject( add ) )
		return false;

	if( add->isActive() ) //
	{
		pGridObjects.push_back( add );
		if(!add->isActive() && ObjectBoundaryIntersectWithGridObjects(add.get()))
			add->resetBundleTextPosition(this->getDescaleFactor());
		return true;
	}
	return false;
}

bool GridObjectViewer::ObjectBoundaryIntersectWithGridObjects( GridObject* A  )
{
	for( auto it = pGridObjects.cbegin(); it != pGridObjects.cend(); ++it )
	{
		GridObject* ptr = (*it).get();
		if( ptr != A )
			if( ObjectBoundaryIntersect(ptr,A) )
				return true;
	}
	return false;
}

bool GridObjectViewer::ObjectBoundaryIntersect( GridObject* A, GridObject* B )
{
	return geometry::BoxIntersectRectangle_inclusive(A->GetPositionInGridReferenceFrame(),A->getWidth()/2,A->getHeight()/2,B->GetPositionInGridReferenceFrame(),B->getWidth()/2,B->getHeight()/2 );
}

ObjectIntersectResult GridObjectViewer::ObjectBoxesIntersect( GridObject* A, GridObject* B )
{
	return A->IntersectGridObject(B); //turns standard from this function
}

ObjectIntersectResult GridObjectViewer::GridObjectIntersectionResult(GridObject* object)
{
	return ObjectBoxesIntersectWithObjects(object);
}

ObjectIntersectResult GridObjectViewer::ObjectBoxesIntersectWithObjects( GridObject* selection)
{
	ObjectIntersectResult result = ObjectIntersectResult();
	selection->refreshBoxesState();

	for( auto it = pGridObjects.cbegin(); it != pGridObjects.cend(); ++it )
	{
		GridObject* ptr = (*it).get();
		if( ptr != selection )
		{
			ObjectIntersectResult res = ObjectBoxesIntersect(selection,ptr);

			if( res.bundled )
				return res;

			result.bundled = result.bundled || res.bundled;
			result.invalid = result.invalid || res.invalid;
			result.intersectObject = res.intersectObject;
			result.roomObject = res.roomObject;
		}
	}
	return result;
}

bool GridObjectViewer::AllowRenderGridObject(std::shared_ptr<GridObject> object)
{
	if (object->GetState() == enGridObjectState::goHide)
		return false;
	return true;
}

//bool GridObjectViewer::RenderGridObjectLast(std::shared_ptr<GridObject> object)
//{
//	return true;
//}

void GridObjectViewer::RenderGridObjects( SDL_Renderer* renderer)
{
	//std::vector<GridObject*> renderLast = std::vector<GridObject*>();

	for( auto it = pGridObjects.crbegin(); it != pGridObjects.crend(); ++it )
	{
		if(AllowRenderGridObject(*it) == false)
			continue;

		GridObject* currentGridObject = (*it).get();

		//if (RenderGridObjectLast((*it)))
		//	renderLast.push_back(currentGridObject);
		//else
			currentGridObject->render( this->position, renderer, this->pDescaleFactor);
	}

	//for( auto jt = renderLast.begin(); jt != renderLast.end(); ++jt )
	//{
	//	(*jt)->render( this->position, renderer, this->pDescaleFactor );
	//}
}

void GridObjectViewer::RenderGridObject( SDL_Renderer* renderer, GridObject* object )
{
	//if( object->boxSize() == this->pDescaleFactor )
	//	object->render( this->position, renderer, this->pDescaleFactor);
	//else
		object->render( this->position, renderer, this->pDescaleFactor);
}


void GridObjectViewer::RemoveSelectedGridObject()
{
	for( auto it = pGridObjects.begin(); it != pGridObjects.end(); )
	{
		if( this->selectedGridObject == (*it) )
		{
			it = pGridObjects.erase(it);
			break;
		}
		else
			++it;
	}
	this->selectedGridObject.reset();
}

void GridObjectViewer::RemoveGridObject( std::shared_ptr<GridObject> object)
{
	//could also use find function here, instead of iterating to find 
	//for( auto it = this->pGridObjects.begin(); it != this->pGridObjects.end(); ++it )
	//{
	//	if( object == (*it)->GetGridObject() )
	//	{
	//		this->pGridObjects.remove( *it );
	//		return;
	//	}

	//}
	this->pGridObjects.remove( object );
	if (this->selectedGridObject == object)
		this->selectedGridObject.reset();
}

void GridObjectViewer::RemoveAllGridObjects()
{
	this->ResetBoxStates();
	this->pGridObjects.clear();
}

void GridObjectViewer::handleEvent( SDL_Event& event )
{
	FLAG_SELECTED_GRIDOBJECT_INVALID_PLACEMENT = 0;

	Coord mousePos;
	SDL_GetMouseState( &mousePos.x, &mousePos.y );

	if( event.type == SDL_MOUSEMOTION ||  event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_KEYDOWN )
	{
		if( selectedGridObject )
		{
			handleGridObjectSelect( event, mousePos );
		}
		else
		{
 			handleGridMouseHover( event, mousePos );
		}
	}
	else
		return;
}

void GridObjectViewer::handleGridMouseHover( SDL_Event& e, const Coord& mousePos  )
{
	std::weak_ptr<GridObject> newObject = newSelectedObjectFromMousePosition( mousePos );

	if( newObject.expired() && pHoveredGridObject.expired() )
	{
		handleGridBoxSelect( e, boxPositionFromMousePosition(mousePos) );
	}
	else if( newObject.expired() && !pHoveredGridObject.expired() )
	{
		pHoveredGridObject.lock()->disableHoveredState();
		pHoveredGridObject.reset();
		//ResetSelectedGridBox();
	}
	else if( !newObject.expired() )
	{
		if(pHoveredGridObject.expired() || pHoveredGridObject.lock() != newObject.lock())
		{
			//if( GridObjectRoom* RoomObject = dynamic_cast< GridObjectRoom* >( newObject.lock().get() ) )
			//{
			//	//simple fix so that objects inside a room can be hovered instead
			//	if(RoomObject->isLocked())
			//		return;
			//}
			AddNewGridObjectHovered( newObject.lock() );
		}
			
		handleGridObjectHovered( e, pHoveredGridObject, mousePos);
		ResetSelectedGridBox();
	}
}

void GridObjectViewer::AddNewGridObjectHovered( std::shared_ptr<GridObject> newObject )
{
	if( !pHoveredGridObject.expired() )
		pHoveredGridObject.lock()->disableHoveredState();
	pHoveredGridObject = newObject;
	pHoveredGridObject.lock()->setStateHovered();
}

void GridObjectViewer::handleGridObjectHovered( SDL_Event& event, std::weak_ptr<GridObject> hoveredObj, const Coord& mousepos )
{
	switch( event.type )
		{
		case SDL_MOUSEMOTION:
			handleGridObjectHoveredMouseMotion(event, hoveredObj, mousepos);
			break;

		case SDL_MOUSEBUTTONDOWN:
			handleGridObjectHoveredMouseDown(event, hoveredObj, mousepos);
			break;
		case SDL_MOUSEBUTTONUP:
			break;//mousepos
		}
}

void GridObjectViewer::handleGridObjectHoveredMouseMotion(SDL_Event& e, std::weak_ptr<GridObject>, const Coord&)
{

}

void GridObjectViewer::handleGridObjectHoveredMouseDown(SDL_Event& e, std::weak_ptr<GridObject> hoveredObj, const Coord& mousepos)
{
	selectedGridPos = boxPositionFromMousePosition(mousepos);
	setNewSelectedObject(hoveredObj.lock(), (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)));
	pHoveredGridObject.lock()->disableHoveredState();
	pHoveredGridObject.reset();
	FLAG_GRIDOBJECT_SELECTED_FROM_SEPERATE_GRID = 0;
}



void GridObjectViewer::handleGridObjectSelect( SDL_Event& event, const Coord& mousepos )
{
	Coord changeInPosition(0,0);
	bool inside = mouseInsideGrid(mousepos);
	ObjectIntersectResult intersectResult = this->GridObjectIntersectionResult(this->selectedGridObject.get());	//placed on top for red drawing of invalid boxes

	switch( event.type )
		{
		case SDL_MOUSEMOTION:
			changeInPosition = (this->selectedGridPos - boxPositionFromMousePosition(mousepos))*-1;
			this->selectedGridObject->setRelativePosition( this->selectedGridObject->getRelativePosition() + changeInPosition ); //this->selectedGridObject->getRelativePosition() + changeInPosition
			this->selectedGridPos += changeInPosition;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(!inside)
				intersectResult.invalid = true;

			if(intersectResult.bundled)
			{
				this->selectedGridObject->setTempStackedGridObject( intersectResult.intersectObject );
				this->selectedGridObject->MakeBundleWithStackedObject();
			}
			else if(intersectResult.invalid)
			{
				FLAG_SELECTED_GRIDOBJECT_INVALID_PLACEMENT = 1;
				if(this->selectedGridObject->hasAwaitingStackObject() )
					this->selectedGridObject->MakeBundleWithStackedObject();
				else
				{
					this->selectedGridObject->setProperPosition( this->selectedGridObject->getLastPlacedPosition() );
				}
				this->selectedGridObject->refreshBoxesState();
			}
			else
			{
				this->selectedGridObject->nullifyTempStackedGridObject();
				this->selectedGridObject->setLastPlacedPosition(this->selectedGridObject->getRelativePosition());
			}
			if( !this->FLAG_SELECTED_GRIDOBJECT_INVALID_PLACEMENT )
				this->AddNewGridObjectHovered( selectedGridObject );
			this->selectedGridObject.reset();
			break;

		case SDL_MOUSEBUTTONUP:

			//this->selectedGridObject.reset();
			break;

		case SDL_KEYDOWN:
			SDL_Scancode key = event.key.keysym.scancode;// KeyReader::GetKey(event);
			if( SDLKeyBible::IsArrowKey(key) && this->selectedGridObject.get()->CanRotate() )
			{
				if(key == SDL_SCANCODE_UP)
				{
					Flip();
				}
				else if(key == SDL_SCANCODE_DOWN)
				{
					Flip();
				}
				else if(key == SDL_SCANCODE_LEFT)
				{
					RotateGBAntiClockwise();
				}
				else if(key == SDL_SCANCODE_RIGHT)
				{
					RotateGBClockwise();
				}
			}
			break;
		}
}

void GridObjectViewer::RotateGBClockwise()
{
	this->selectedGridObject.get()->Rotate90Degrees(true);
}
void GridObjectViewer::Flip()
{
	this->selectedGridObject.get()->FlipAlongX();
}
void GridObjectViewer::RotateGBAntiClockwise()
{
	this->selectedGridObject.get()->Rotate90Degrees(false);
}

void GridObjectViewer::setNewSelectedObject( std::shared_ptr<GridObject> set )
{
	setNewSelectedObject(set,true);
}

void GridObjectViewer::ResetSelectedGridObject()
{
	this->selectedGridObject.reset();
	this->pHoveredGridObject.reset();
}

bool GridObjectViewer::ContainsGridObject( std::shared_ptr<GridObject> object )
{
	for( auto it = this->pGridObjects.begin(); it != this->pGridObjects.end(); ++it )
	{
		if( object == (*it) )
			return true;
	}
	return false;
}

void GridObjectViewer::setNewSelectedObject( std::shared_ptr<GridObject> set, bool singleObjectSelect )
{
	if( set->isActive() )
	{
		GridObject* selected = set.get();
		if(selected->GetState() == enGridObjectState::goNoSelect || selected->GetState() == enGridObjectState::goHide)
			return;

		if( singleObjectSelect && set->hasBundle() )
			selected = set->PullGridObjectFromBundle();

		this->selectedGridObject = set;
		this->selectedGridPos = selected->GetPositionInGridReferenceFrame();
	}
}

bool GridObjectViewer::DoesPointContainGridObject( Coord gridPos )
{
	Coord objectCoordPos = gridPos;//(mousepos - this->position)*this->pDescaleFactor/GridBox::BOXSIZE;

	for( auto it = this->pGridObjects.begin(); it!= this->pGridObjects.end(); ++it )
	{
		GridObject* ptr = (*it).get();
		if( ptr->isPositionInObject( objectCoordPos - ptr->getRelativePosition() + ptr->ObjectOrigo ) )
			return true;
	}
	return false;
}

std::weak_ptr<GridObject> GridObjectViewer::newSelectedObjectFromMousePosition( Coord mousepos )
{
	Coord objectCoordPos = (mousepos - this->position)*this->pDescaleFactor/GridBox::BOXSIZE;

	for( auto it = this->pGridObjects.begin(); it!= this->pGridObjects.end(); ++it )
	{
		GridObject* ptr = (*it).get();
		if(ptr->GetState() == enGridObjectState::goNoSelect || ptr->GetState() == enGridObjectState::goHide )
			continue;

		if( ptr->isPositionInObject( objectCoordPos - ptr->getRelativePosition() + ptr->ObjectOrigo ) )
			return (*it);
	}
	return std::shared_ptr<GridObject>(nullptr);
}


