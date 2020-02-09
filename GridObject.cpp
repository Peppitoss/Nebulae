#include"GridObject.h"
#include"HotKeySelection.h"
#include"StringBuilder.h"
#include"Debug.h"

GridObject::GridObject()
{
	pObjectWidth = 0;
	pObjectHeight = 0;
	this->pBoxSize = spriteSize(spriteSize(0));
	initialize();
}

GridObject::GridObject( const BoxList& gridPos )
{
	pObjectWidth = gridPos.getWidth();
	pObjectHeight = gridPos.getHeight();

	this->pBoxSize = spriteSize(spriteSize::GetSize64() );
	for( std::list<Coord>::const_iterator it = gridPos.begin(); it != gridPos.end(); ++it )
	{
		if( !mapHasKey( (*it) ) )
			this->pBoxes[ (*it) ] = std::unique_ptr<GridBox>( new DynamicGridBox( GridObjectCollection::ImportGrid_Used) );
	}
	initialize();
}

GridObject::GridObject( CoordMap<GriddedBox>& map, spriteSize Boxsize,const imagehandling::IMGToSurface* surface )
{
	map.OptimizeIteratorConstraints();
	int sizeInt = Boxsize.GetSize();
	for( auto it = map.Begin(); it != map.End(); ++it )
	{
			std::vector<CoordWithColor> canvas = std::vector<CoordWithColor>();	//optimize so you don't need to remake list ever iteration
			canvas.reserve(sizeInt*sizeInt);
			Coord relativePosition = it.Coord() - map.GetMinCoordValues();

			for( int x = 0; x<sizeInt;x++)
			{
				for( int y = 0; y<sizeInt;y++)
				{
					canvas.push_back( CoordWithColor(x,y,surface->get_pixel32RGB(x+relativePosition.x*sizeInt,y + relativePosition.y*sizeInt)));
				}
			}

			Coord pBoxPosition = it.Coord() - map.GetOptimiziedDimension()/2;

			if( !mapHasKey( pBoxPosition ) )
			{
				imagehandling::IMGToSurface* box = new imagehandling::IMGToSurface(canvas,Boxsize);
				//box->ReplaceSurface();
				this->pBoxes[ pBoxPosition ] = std::unique_ptr<GridBox>(new DynamicGridBox( box ));
			}

	}

	pObjectWidth = map.GetOptimiziedDimension().x;
	pObjectHeight = map.GetOptimiziedDimension().y;
	initialize();
}

GridObject::GridObject( const BoxList& list, spriteSize Boxsize,const imagehandling::IMGToSurface* surface ) //not used, see function above
{
	initialize();

	this->pBoxSize = Boxsize;
	int sizeInt = this->pBoxSize.GetSize();

	if(Boxsize.GetDimension() == STANDARDOBJECTSIZE.GetDimension() ) //good, no image resizing required size should always be 8
	{
		for( std::list<Coord>::const_iterator it = list.begin(); it != list.end(); ++it )
		{
			std::vector<CoordWithColor> canvas = std::vector<CoordWithColor>();	//optimize so you don't need to remake list ever iteration
			canvas.reserve(sizeInt*sizeInt);
			Coord relativePosition = list.getLowestPosition()*-1 + (*it);

			for( int x = 0; x<sizeInt;x++)
			{
				for( int y = 0; y<sizeInt;y++)
				{
					canvas.push_back( CoordWithColor(x,y,surface->get_pixel32RGB(x+relativePosition.x*sizeInt,y + relativePosition.y*sizeInt)));
				}
			}

			if( !mapHasKey( (*it) ) )
			{
				imagehandling::IMGToSurface* box = new imagehandling::IMGToSurface(canvas,Boxsize);
				//box->ReplaceSurface();
				this->pBoxes[ (*it) ] = std::unique_ptr<GridBox>(new DynamicGridBox( box ));
			}
		}
	}
	pObjectWidth = list.getWidth();
	pObjectHeight = list.getHeight();
	initialize();
	
}

void GridObject::refreshBoxesState()
{
	for( auto it = pBoxes.begin(); it != pBoxes.end(); ++it )
	{
		(*it).second.get()->setState( enBoxState::STANDARD );
	}
}

void GridObject::setBoxesInvalid()
{
	for( auto it = pBoxes.begin(); it != pBoxes.end(); ++it )
	{
		(*it).second.get()->setState( enBoxState::INVALID );
	}
}

void GridObject::setBoxesHighlighted()
{
	for (auto it = pBoxes.begin(); it != pBoxes.end(); ++it)
	{
		(*it).second.get()->setState(enBoxState::HIGHLIGHTED);
	}
}

void GridObject::resetBundleTextPosition(int scalingFactor )
{
	Coord relativePosition = (Coord(this->getWidth(),this->getHeight()))*GridBox::BOXSIZE/scalingFactor;
	pBundleCountText.setRelativePosition( relativePosition );
	pBundleCountText.setAttachmentposition( Coord(0,0) );
}

void GridObject::setRelativePosition( Coord set )
{ 
	this->pRelativePosition = set; 
}

void GridObject::setProperPosition( Coord set) 
{ 
	setRelativePosition(set);
	setLastPlacedPosition(set);
};

GridObject::GridObject(const imagehandling::IMGToSurface* surface )
{
	if( surface->GetSpriteSize().IsMax() )
		return;

	this->pBoxSize = spriteSize(STANDARDOBJECTSIZE);
	int sizeInt = this->pBoxSize.GetSize();

	auto boxesPerDimension = surface->GetSpriteSize().GetDimension()/this->pBoxSize.GetSize();

	pObjectWidth = boxesPerDimension.x;
	pObjectHeight = boxesPerDimension.y;

	for( int box_x = 0; box_x< pObjectWidth; box_x++ )
	{
		for( int box_y = 0; box_y<pObjectHeight;box_y++ )
		{
			std::vector<CoordWithColor> canvas = std::vector<CoordWithColor>();	//optimize, so you don't need to remake list all the time!
			canvas.reserve(sizeInt);

			for( int x = 0; x<sizeInt;x++)
			{
				for( int y = 0; y<sizeInt;y++)
				{
					canvas.push_back( CoordWithColor(x,y,surface->get_pixel32RGB(x+box_x*sizeInt,y + box_y*sizeInt)));
				}
			}

			if( !mapHasKey( Coord(box_x,box_y) ) )
			{
				imagehandling::IMGToSurface* box = new imagehandling::IMGToSurface(canvas,this->pBoxSize);
				this->pBoxes[ Coord(box_x,box_y) ] = std::unique_ptr<GridBox>(new DynamicGridBox( box ));
			}
		}
	}
	initialize();
}

void GridObject::initialize()
{
	pDescription = "";
	GridOrigo = Coord(0,0);
	ObjectOrigo = Coord(0,0);
	placementOrigoDependent = false;
	isHoveredByMouse = false;
	SortOrder = 0;
	pState = enGridObjectState::goNormal;
	//pGridObjectType = enGridObjectType::typeInternal;
	pFlipped = enObjectFlipped::goNotFlipped;
	pRotation = enObjectDiscreteRotation::goNoRotation;
	//setScaling(1);

	this->pRenderHotKeys = false;
	this->pBundled = false;
	this->pBundleCountText.generateLink(ObjectOrigo, this->GetPositionInGridReferenceFrame() + ObjectOrigo, "X", enLinkType::LINKSTANDARD);
	pGridObjectBundle = std::list<GridObject*>();
	pIDName = "";
	tempStackedGridObject = NULL;
	pCanRotate = false;
	//this->GenerateHotKeyButton('k');
	//this->setBundleTextPosition();
}

int GridObject::GetCenter_X()
{
	int xSmallest = 6000;
	int xBiggest = -6000;

	for (auto it = this->begin(); it != this->end(); ++it)
	{
		if ((*it).first.x < xSmallest)
			xSmallest = (*it).first.x;
		if ((*it).first.x > xBiggest)
			xBiggest = (*it).first.x;
	}

	return (xSmallest + xBiggest) / 2;
}

bool GridObject::mapHasKey( Coord Coord)
{
	auto it = pBoxes.find(Coord);
	if(it != pBoxes.end())
	{
		//element found
		return true; 
	}
	return false;
}

void GridObject::SetNewGridOrigo(const Coord& origo)
{
	this->GridOrigo = Coord(origo.x + this->ObjectOrigo.x, origo.y + this->ObjectOrigo.y);
}


void GridObject::SetNewGridOrigo( int gridSize )
{
	this->GridOrigo = Coord(gridSize/2+this->ObjectOrigo.x,gridSize/2+this->ObjectOrigo.y);
}

//Coord GridObject::GetMaxCoordValues()
//{
//	int maxy = 0;
//	int maxx = 0;
//	for( auto it = this->begin(); it != this->end(); ++it )
//	{
//		if(it->first.x > maxx )
//			maxx = it->first.x;
//		if(it->first.y > maxy )
//			maxy = it->first.y;
//	}
//	return Coord(maxx, maxy);
//}

void GridObject::render( Coord pos, SDL_Renderer* renderer, int scaleFactor) const //, spriteSize vesselSize )
{
	if( !isActive() )
		return;

	for( auto it = pBoxes.begin(); it != pBoxes.end(); ++it )
	{
		if( (*it).second.get()->getBoxState() == enBoxState::HIDE )
			continue;

		SDL_Texture* texture = GetTextureFromBox((*it).second.get(),renderer);
		Coord drawPosition( pos.x + ((*it).first.x + this->GetPositionInGridReferenceFrame().x)*GridBox::BOXSIZE/scaleFactor , pos.y + ((*it).first.y + this->GetPositionInGridReferenceFrame().y )*GridBox::BOXSIZE/scaleFactor );
		int textureSize = GridBox::BOXSIZE/scaleFactor;

		SDL_Rect renderQuad = { drawPosition.x, drawPosition.y, textureSize, textureSize};
		SDL_RenderCopy( renderer, texture, NULL, &renderQuad );
		Debug::SDLErrorCheck();
	}

	Coord toolTipDrawPosition = pos + (this->GetPositionInGridReferenceFrame() - Coord(1,1) - this->ObjectOrigo)*GridBox::BOXSIZE/scaleFactor;
	//Coord bundleDrawPosition = (Coord(this->getWidth(),this->getHeight()))*this->boxSize()*GridBox::BOXSIZE/scaleFactor;

	if (this->showHotKeyLink() && this->hasHotKeyLink())
	{
		if (this->isHoveredByMouse)
			this->pHotKeyButton.link->sethovered();
		else if (this->pHotKeyButton.link->GetState() == WIDGET_STATE::WS_ON)
			this->pHotKeyButton.link->enable();
		this->pHotKeyButton.link->Render(*renderer, toolTipDrawPosition); //
	}
	if (this->hasTooltipLink() && this->isHoveredByMouse)
	{
		pTooltip.link->Render(*renderer, toolTipDrawPosition);
	}


	if( hasBundle() && pBundleCountText.link)
		pBundleCountText.link->Render(*renderer,toolTipDrawPosition);
}

void GridObject::RunHotKeyEvent(SDL_Event& e)
{
	if (this->hasHotKeyLink())
		this->pHotKeyButton.link->HandleEvent(e);
}

bool GridObject::HotKeyIsSelected()
{
	if (this->hasHotKeyLink())
		return this->pHotKeyButton.link->GetState() == WIDGET_STATE::WS_ON;
	return false;
}

string GridObject::GetSelectedHotKey()
{
	if (this->hasHotKeyLink())
		return ((HotKeySelection*)this->pHotKeyButton.link.get())->GetText();
}

void GridObject::SetHotKeyForDisplay(std::string set)
{
	if (this->hasHotKeyLink())
		((HotKeySelection*)this->pHotKeyButton.link.get())->SetHotKey(set);
}

void GridObject::SelectHotKeyLink(bool value)
{
	if (this->hasHotKeyLink())
		this->pHotKeyButton.link->SetWidgetClicked();
}

SDL_Texture* GridObject::GetTextureFromBox( GridBox* box, SDL_Renderer* renderer ) const
{
	return ((DynamicGridBox*)box)->GetTextureByState(renderer);
}

void GridObject::setToolTipText( std::string set )
{
	Coord relativePos(5,5);
	pTooltip.generateLink( relativePos, this->GetPositionInGridReferenceFrame() + relativePos, set, enLinkType::LINKSTANDARD);
	pTooltip.link->SetWidgetClicked();
}

bool GridObject::showHotKeyLink() const
{
	return this->RenderHotKeys();
}

void GridObject::setStateHovered() 
{	
	setBoxesHighlighted();
	isHoveredByMouse = true; 
}

void GridObject::disableHoveredState()
{
	refreshBoxesState();
	isHoveredByMouse = false;
}

void GridObject::GenerateHotKeyButton(char hotkey)
{
	Coord relativePos(0, 0);
	this->pHotKeyButton.generateLink(relativePos, this->getRelativePosition(), &hotkey, enLinkType::LINKHOTKEYSELECT);
}

Coord GridObject::GetPositionInGridReferenceFrame() const
{ 
	return this->getRelativePosition() -ObjectOrigo + GridOrigo; //Coord(32, 32);
};

ObjectIntersectResult GridObject::IntersectGridObject( GridObject* B )
{
	ObjectIntersectResult result = ObjectIntersectResult();

	auto intersection = GridObjectIntersection(B);
	if(GridObjectIntersect(intersection) == false)
		return result;

	Coord relativePositionToB = this->GetPositionInGridReferenceFrame()  - B->GetPositionInGridReferenceFrame();

	for( auto it = this->pBoxes.begin() ; it != this->pBoxes.end(); ++it )
	{
		if( geometry::pointWithinRectangle_inclusive( this->ObjectOrigo+(*it).first, Coord(intersection.x, intersection.y), intersection.w,intersection.h ) && B->mapHasKey( relativePositionToB + (*it).first))
		{
			(*it).second.get()->setState(enBoxState::INVALID);
			result.invalid = true;
			result.intersectObject = B;
		}
		else if( (*it).second.get()->getBoxState() != enBoxState::INVALID )
			(*it).second.get()->setState(enBoxState::STANDARD);
	}

	return result;
}

SDL_Rect GridObject::GridObjectIntersection(GridObject* B)
{
	return geometry::GetRectangleIntersect(this->GetPositionInGridReferenceFrame() - this->ObjectOrigo, this->getWidth(), this->getHeight(), B->GetPositionInGridReferenceFrame() - B->ObjectOrigo, B->getWidth(), B->getHeight());
}

bool GridObject::GridObjectIntersect(GridObject* B)
{
	return GridObjectIntersect(GridObjectIntersection(B));
}

bool GridObject::GridObjectIntersect(const SDL_Rect& intersection)
{
	if (intersection.w <= 0 || intersection.h <= 0)
		return false;
	return true;
}

void GridObject::MakeBundleWithStackedObject()
{
	if(this->tempStackedGridObject != NULL)
	{
		this->tempStackedGridObject->addGridObjectToBundle( this );
	}
}


GridObject* GridObject::PullGridObjectFromBundle()
{
	if( this->pGridObjectBundle.size()>0 )
	{
		GridObject* result = *this->pGridObjectBundle.begin();
		result->SetActive(true);
		result->setProperPosition( this->getRelativePosition() + this->GridOrigo - result->GridOrigo );
		this->pGridObjectBundle.pop_front();
		this->pBundleCountText.ChangeText( IntToString(pGridObjectBundle.size()+1) );
		if( this->pGridObjectBundle.empty() )
			this->setBundled(false);

		return result;
	}
	else
		return NULL;
}

std::list<GridObject*> GridObject::PullAllGridObjectFromBundle()
{
	std::list<GridObject*> result = std::list<GridObject*>();

	for( int i = 0; i < this->pGridObjectBundle.size(); i++ )
	{
		result.push_back( this->PullGridObjectFromBundle() );
	}
	return result;
}

void GridObject::addGridObjectToBundle( GridObject* add )
{
		
	if( add->hasBundle() )
	{
		std::list<GridObject*> bundle = add->PullAllGridObjectFromBundle();
		for( std::list<GridObject*>::const_iterator it = bundle.begin(); it != bundle.end(); ++it )
		{
			this->addGridObjectToBundle(*it);
		}
	}

	add->SetActive(false);
	add->setTempStackedGridObject(this);
	add->setRelativePosition(Coord(-100,-100));	//in order not to disturb selection of objects, -100 is arbitrary

	this->pGridObjectBundle.push_front(add);
	this->setBundled(true);
	this->pBundleCountText.ChangeText( IntToString(pGridObjectBundle.size()+1) );
}
void GridObject::SetActive( bool set )
{
	if( set == false && this->pGridObjectBundle.size()==0 )
	{
		this->pActive = false;
	}
	else
		this->pActive = true;
}

void GridObject::SetBoxRed( const Coord& pos )
{
	if( mapHasKey(pos) )
		pBoxes[pos]->SetBoxRedFlag(true);
}

void GridObject::SetBoxHvite( const Coord& pos )
{
	if( mapHasKey(pos) )
		pBoxes[pos]->SetBoxRedFlag(false);
}

void GridObject::SetBoxBlackLevel( const Coord& pos, double percent )
{
	int color = 255*percent;
	this->pBoxes[this->ObjectOrigo*-1 + pos].get()->Modulate( ColorRGB(color,color,color) );
}

void GridObject::SetBoxTransparent( const Coord& pos )
{
	this->pBoxes[this->ObjectOrigo*-1 + pos].get()->setState( enBoxState::HIDE );
}

void GridObject::GridObjectHvite()
{
	for( auto it = pBoxes.begin(); it != pBoxes.end(); ++it )
	{
		(*it).second.get()->SetBoxRedFlag(false);
	}
}
void GridObject::SetBoxRed( const BoxList& list ) //assumes list has smallest Coord of 0
{
	for( std::list<Coord>::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		SetBoxRed(this->ObjectOrigo*-1+(*it) );
	}
}

enGridObjectState GridObject::GetState()
{
	return this->pState;
}

bool GridObject::CanRotate() const
{ 
	return this->pCanRotate; 
};
void GridObject::SetCanRotate( bool TrueIsRotate )
{ 
	this->pCanRotate = TrueIsRotate;
};
void GridObject::Rotate90Degrees( bool clockwise )
{
	std::map<Coord,std::unique_ptr<GridBox>> pNewContainer;
	for( auto it = pBoxes.begin(); it != pBoxes.end(); ++it )
	{
		if(clockwise)
		{
			pNewContainer[ (*it).first.rotateClockWise() ] = std::move( (*it).second );
			
		}
		else
		{
			pNewContainer[ (*it).first.rotateAntiClockWise() ] = std::move( (*it).second );
		}
	}
	if(clockwise)
		IncrementDiscreteRotation( this->pRotation );
	else
		DecrementDiscreteRotation( this->pRotation );
	this->pBoxes.swap(pNewContainer);	
}

void GridObject::Transpose()
{
	std::map<Coord,std::unique_ptr<GridBox>> pNewContainer;
	for( auto it = pBoxes.begin(); it != pBoxes.end(); ++it )
	{
		pNewContainer[ (*it).first.xyswap() ] = std::move( (*it).second );
	}
	this->pBoxes.swap(pNewContainer);	
}

void GridObject::FlipAlongX()
{
	int centerX = GetCenter_X();
	std::map<Coord,std::unique_ptr<GridBox>> pNewContainer;
	for( auto it = pBoxes.begin(); it != pBoxes.end(); ++it )
	{
		//if (this->getWidth() % 2 == 0)
		//	pNewContainer[ (*it).first.FlipAlongXline(centerX).xadd(-1) ] = std::move( (*it).second );
		//else
			pNewContainer[(*it).first.FlipAlongXline(centerX)] = std::move((*it).second);
	}

	this->pBoxes.swap(pNewContainer);
	ToggleFlipped(this->pFlipped);
}

//void GridObject::FlipAlongY()
//{
//	std::map<Coord,std::unique_ptr<GridBox>> pNewContainer;
//	for( auto it = pBoxes.begin(); it != pBoxes.end(); ++it )
//	{
//		pNewContainer[ (*it).first.FlipAlongYline(0) ] = std::move( (*it).second );
//	}
//	this->pBoxes.swap(pNewContainer);	
//}

//enGridObjectType GridObject::GetGridObjectType()
//{
//	return this->pGridObjectType;
//}
//
//void GridObject::SetGridObjectType( enGridObjectType set )
//{ 
//	this->pGridObjectType = set; 
//}

void GridObject::SetFlipped(enObjectFlipped flipped)
{ 
	if( flipped != pFlipped )
	{
		this->FlipAlongX();
		pFlipped = flipped;
	}
};

void GridObject::SetRotation(enObjectDiscreteRotation rotation) 
{ 
	int difference = rotation - this->pRotation;
	bool clockwise = difference >= 0;

	for( int i = 0; i < abs(difference); i++ )
		Rotate90Degrees(clockwise);
};

int GridObject::GridBoxAmount() const
{ 
	return this->pBoxes.size(); 
};


ObjectIntersectResult::ObjectIntersectResult()
{
	this->invalid = false;
	this->bundled = false;
	this->intersectObject = NULL;
	this->roomObject = NULL;
}

ObjectIntersectResult::ObjectIntersectResult( bool invalid, bool stack, GridObject* object ,GridObject* RoomObject)
{
	this->invalid = invalid;
	this->bundled = stack;
	this->intersectObject = object;
	this->roomObject = RoomObject;
}

bool compare_gridObjects_ptr(const std::shared_ptr<GridObject>& first, const std::shared_ptr<GridObject>& second)
{
	return compare_gridObjects(first.get(),second.get());
}

bool compare_gridObjects(const GridObject* first, const GridObject* second)
{
	if( first->GetSortOrder() == second->GetSortOrder() )
		return first->GetSortOrder()>second->GetSortOrder();
	else
		return first->GetSortOrder() > second->GetSortOrder();
}


