#include"ShipGrid.h"
#include"GridObjectItem.h"
#include"BorderGraphicHandler.h"
#include"ModuleInterfaceGlobalList.h"
#include"VesselGlobalList.h"
#include"GameManager.h"
#include"Debug.h"


//GridBoxSelectionResult::GridBoxSelectionResult()
//{
//	this->insideHp = -1;
//	this->outsideHp = -1;
//}
template class CoordMapIterator<GriddedBox>;

ShipGrid::ShipGrid( spriteSize size, Coord pos, spriteSize boxesPerSide ) : InventoryItemGrid( size, pos,boxesPerSide )
{
	pShowVesselOutside = false; //bug if initially true
	pGridSelectionType = enGridSelectType::Fight;	//is true for testing should be false at first
	breachHighlighted = false;
	pModelVessel.reset();// = nullptr;
	this->pVesselOuterGridList = CoordMap<GridBox>(boxesPerSide.GetSize());

}

void ShipGrid::SetGridShipEdge( const Coord& position )
{
	((ShipBuilderBox*)getBox( position ))->setType( enBoxType::WALL );
}

void ShipGrid::SetGridShipInterior( const Coord& position )
{
	((ShipBuilderBox*)getBox( position ))->setType( enBoxType::EMPTY );
}

void ShipGrid::SetBoxesPerSide( spriteSize boxesPerSide )
{
	Grid::SetBoxesPerSide( boxesPerSide );

	int boxesperside = boxesPerSide.GetSize();
	if( boxesperside != this->pVesselOuterGridList.IndexSize() || this->pVesselOuterGridList.IndexSize() == 0)
	{
		pVesselOuterGridList.ResetSize();
		pVesselOuterGridList = CoordMap<GridBox>(boxesPerSide.GetSize());
	}

}

bool ShipGrid::IsBoxDestroyed( const Coord & Coord) 
{ 
	return ((ShipBuilderBox*) this->boxList[ Coord ].get())->IsBoxDestroyed(); 
};

void ShipGrid::SetBoxDestroyed( const Coord & Coord) 
{ 
	((ShipBuilderBox*) this->boxList[ Coord ].get())->SetBoxDestroyed(); 
	this->boxList[ Coord ]->SetBoxRedFlag(true); 
};

void ShipGrid::SetShowHotKeys(bool set, enGridSelectType typeIfHotkeyDisabled)
{ 
	for (auto it = this->gridObjectBegin(); it != this->gridObjectEnd(); ++it)
	{
		if ((*it)->hasHotKeyLink())
			(*it)->RenderHotKeys(set);
	}
	if(set)
		this->pGridSelectionType = enGridSelectType::Hotkeys;
	else
		this->pGridSelectionType = typeIfHotkeyDisabled;
}

bool ShipGrid::IsShowHotkeys() const
{
	return this->pGridSelectionType == enGridSelectType::Hotkeys;
}

bool ShipGrid::IsBuilderMode() const
{
	return this->pGridSelectionType == enGridSelectType::Builder;
}
bool ShipGrid::IsFighterMode() const
{
	return this->pGridSelectionType == enGridSelectType::Fight;
}


int ShipGrid::GetHitPointFromHoveredGridObject()
{
	if (this->pHoveredGridObject.expired() == false)
	{
		return GetEntityFromGridObject(this->pHoveredGridObject.lock())->getHitPointLeft();
	}
	return 0;
}

int ShipGrid::GetHitPointFromChassis()
{
	if (IsGridPositionChassis(currentSelectedGridBox))
	{
		return this->pModelVessel.lock()->getHitPointLeft();
	}		
	return 0;
}

int ShipGrid::GetHitPointFromCurrentSelection()
{
	try
	{
		if (this->pHoveredGridObject.expired() == false)
		{
			std::shared_ptr<ModuleEntity> entity = GetEntityFromGridObject(this->pHoveredGridObject.lock());
			if( entity != nullptr)
				return entity->getHitPointLeft();
			return 0;
		}
		else
		{
			GetHitPointFromChassis();
		}
	}
	catch (int e)
	{
		return -1;
	}
}

bool ShipGrid::IsGridPositionChassis(const Coord& pos)
{
	return this->GetVesselFigure().lock()->GetBoxes().CoordIsFilled(pos);//this->pModelVessel.lock()->GetFigure().GetBoxes().CoordIsFilled(pos);
}

//void ShipGrid::Resize( spriteSize size, spriteSize boxesPerSide)
//{
//	Grid::Resize( size, boxesPerSide );
//	pVesselOuterGridList.ResetSize();
//	pVesselOuterGridList = CoordMap<GridBox>(boxesPerSide.getIntValue());
//	this->pVesselOuterHiddenList.clear();
//}

void ShipGrid::AddBox( int x, int y, enBoxState state )
{
	if( boxList.CoordIsFilled(Coord(x,y)) )
	{
		boxList.GetItem(Coord(x,y)).Item->setState(state);
		return;
	}

	ShipBuilderBox* newBox = new ShipBuilderBox( GBTextureCollection::ImportGrid_Standard.get(), GridObjectCollection::GO_std_floorpack.texture, GridObjectCollection::GO_std_wallpack.texture, GridObjectCollection::GO_std_wallpack.textureSize ); 
	newBox->setHighlightTexture( GBTextureCollection::ImportGrid_Highligt.get() ); 
	newBox->setState( state );
	boxList.InsertBox(std::shared_ptr<GridBox>(newBox), Coord(x,y)); 
}

//void Grid::AddBox( int x, int y, enBoxState state)
//{
//	ShipBuilderBox* newBox = new ShipBuilderBox(); 
//	newBox->setHighlightTexture(  GBTextureCollection::ImportGrid_Highligt.get() ); 
//	newBox->setState( state );
//	boxList[ Coord(x,y) ] = std::unique_ptr<GridBox>(newBox);
//}


void ShipGrid::RenderGridBase( SDL_Renderer* renderer)
{

}

void ShipGrid::RenderGridBaseOutside( SDL_Renderer* renderer )
{
	for( auto it = this->pVesselOuterGridList.Begin(); it != this->pVesselOuterGridList.End(); ++it) 
	{
		RenderGridBaseOutside(renderer, it.Coord() );
	}
}


void ShipGrid::Render( SDL_Renderer* renderer )
{
	if( this->GetModelVessel() != nullptr )
	{
		if(!pShowVesselOutside)
		{
			RenderGridBaseInside( renderer );
			RenderGridObjects( renderer );
			RenderSelectedGridObject(renderer);
		}
		else
		{
			RenderGridObjects( renderer );
			RenderGridBaseOutside(renderer);
			RenderSelectedGridObject( renderer );
		}
	}
}

bool ShipGrid::AllowRenderGridObject(std::shared_ptr<GridObject> object)
{
	if (GridObjectViewer::AllowRenderGridObject(object) == false)
		return false;

	enItemType itemType = this->GetItemTypeFromObject(object);
	if (pShowVesselOutside)
	{
		if (itemType == enItemType::ExternalModuleItem)
			return true;
	}
	else if (itemType == enItemType::InternalModuleItem || itemType == enItemType::ProjectileItem)
		return true;
			
	return false;
}

//bool ShipGrid::RenderGridObjectLast(std::shared_ptr<GridObject> object)
//{
//	enItemType itemType = this->GetItemTypeFromObject(object);
//	if (pShowVesselOutside && itemType == enItemType::ExternalModuleItem)
//		return true;
//	return false;
//}



void ShipGrid::RenderGridBaseOutside( SDL_Renderer* renderer, const Coord& Coord )
{
	GridBox* ptr = this->pVesselOuterGridList[Coord].get();
	/*if( ((DynamicGridBox*)ptr)->getBoxState() == enBoxState::HIDE )
	{
		if( !this->DoesPointContainGridObject( Coord ) )
			RenderGridBaseInside( renderer, Coord );
	}
	else
	{*/
		SDL_Texture* texture = ((DynamicGridBox*)ptr)->GetTextureByState(renderer);
		Grid::RenderGridBox( texture, renderer, Coord );
	//}
}

void ShipGrid::RenderGridBaseInside( SDL_Renderer* renderer )
{
	CoordMapIterator<GriddedBox> begin = this->GetVesselFigure().lock()->GetBoxes().Begin();
	CoordMapIterator<GriddedBox> end   = this->GetVesselFigure().lock()->GetBoxes().End();
	 
	for( auto it = begin; it != end; ++it) 
	{
		RenderGridBaseInside( renderer, it.Coord() );
	}

	auto vector = this->GetModelVessel()->GetExternalModulesList();

	//render external boxes
	for( auto it = this->gridObjectBegin(); it != this->gridObjectEnd(); ++it )
	{
		if( this->GetItemTypeFromObject((*it)) != enItemType::ExternalModuleItem )
			continue;

		auto begin = (*it)->begin();//->GetBoxes().Begin();
		auto end   = (*it)->end();//GetModuleGraphics()->GetFigure()->GetBoxes().End();

		for( auto jt = begin; jt != end; ++jt )
		{
			Coord boxPosition = (*jt).first + (*it)->GetPositionInGridReferenceFrame();
			RenderGridBaseInside( renderer, boxPosition );
		}
	}
}

void ShipGrid::RenderGridBaseInside( SDL_Renderer* renderer, const Coord& coord)
{
	GridBox* ptr = boxList[coord].get();
	if( ((DynamicGridBox*)ptr)->getBoxState() == enBoxState::HIDE )
		return;

	SDL_Texture* texture = ((ShipBuilderBox*)ptr)->GetTextureByState(renderer);
	Coord drawPosition = GetBoxDrawPosition(coord);

	ShipBuilderBox* box = ((ShipBuilderBox*)this->getBox(coord));
	if (box->GetBoxType() == enBoxType::WALL)
		int d = 3;

	int textureSize = 8;//GridBox::BOXSIZE/pDescaleFactor;

	SDL_Rect renderQuad = { drawPosition.x, drawPosition.y, textureSize, textureSize};
	SDL_RenderCopy( renderer, texture, NULL, &renderQuad );
	Debug::SDLErrorCheck();
}

void ShipGrid::setBoxState( const Coord& pos, enBoxState set, bool outside)
{
	if( outside )
	{
		this->pVesselOuterGridList[pos]->setState( set );
		if( set == enBoxState::HIDE ) //&& this->boxList[pos]->getBoxState() != enBoxState::HIDE )
		{
			this->pVesselOuterHiddenList.insert( pos );
		}
	}
	else
	{
		GridObjectViewer::setBoxState(pos,set);
	}
}

void ShipGrid::SetGridBoxesToHide()
{
	for( auto it = this->pVesselOuterGridList.Begin(); it != this->pVesselOuterGridList.End(); ++it )
		it.item()->setState(enBoxState::HIDE);
	for( auto it = this->boxList.Begin(); it != this->boxList.End(); ++it )
		it.item()->setState( enBoxState::HIDE );

	this->pVesselOuterHiddenList.clear();
}

void ShipGrid::generateGrid( int size )
{
	this->pShowVesselOutside = false;
	Grid::generateGrid( size );
}

void ShipGrid::generateGrid( int xSize, int ySize, enBoxState initState )
{
	
	if( this->boxList.GetInsertedBoxes() != 0 )	//demand that the boxlist is empty before refilling it, use ResetSize(), or change boxesperside which can trigger ResetSize()
	{
		SetGridBoxesToHide();
	}
	else
	{
		this->pVesselOuterHiddenList.clear();
	}

	SetRows( pDescaleFactor*ySize/(GridBox::BOXSIZE) );
	SetColumns( pDescaleFactor*xSize/(GridBox::BOXSIZE) );

	AddGridBoxesFromShipTemplate();
	this->ResetSelectedGridBox();

}
void ShipGrid::AddGridBoxesFromShipTemplate()
{

	if(this->pModelVessel.lock().get() == false || this->GetVesselFigure().lock().get() == false)
		return;

	enBoxState internalBoxState = enBoxState::STANDARD;
	//if(this->pShowVesselOutside)
	//	internalBoxState = enBoxState::HIDE;

	CoordMapIterator<GriddedBox> begin = this->GetVesselFigure().lock()->GetBoxes().Begin();
	CoordMapIterator<GriddedBox> end = this->GetVesselFigure().lock()->GetBoxes().End();

	for( auto it = begin; it != end; ++it)
	{
		this->AddBox( it.Coord().x, it.Coord().y, internalBoxState );
		this->setBoxState( it.Coord(), internalBoxState, false );
		this->SetVesselOuterGridColor( it.Coord(), this->pModelVessel.lock()->getGraphics()->getGraphicalObject()->getMainObject()->getColorFromPoint( it.Coord() ) );
	}
	RefreshFigureBorders(*this->GetVesselFigure().lock().get(), Coord(0));

	//add boxes from modules if external
	for( auto it = this->gridObjectBegin(); it != this->gridObjectEnd(); ++it )
	{
		if (this->GetItemTypeFromObject((*it)) != enItemType::ExternalModuleItem)
			continue;

		auto begin = (*it)->begin();//->GetBoxes().Begin();
		auto end   = (*it)->end();//GetModuleGraphics()->GetFigure()->GetBoxes().End();

		for( auto jt = begin; jt != end; ++jt )
		{
			Coord boxPosition = (*jt).first + (*it)->GetPositionInGridReferenceFrame();
			this->AddBox( boxPosition.x, boxPosition.y, enBoxState::STANDARD );
			this->setBoxState( boxPosition, enBoxState::STANDARD, false );
			//this->SetVesselOuterGridColor( jt.Coord(), this->pModelVessel.lock()->getGraphics()->getGraphicalObject()->getMainObject()->getColorFromPoint( jt.Coord() ) );
		}

		ModuleInterface mInterface = ModuleGlobalList::GetInterface((*it)->GetIDName()); //this->GetEntityFromGridObject(*it)->GetIDName()
		if( mInterface.GetFigureUsage() == enFigureUsage::useNoMainFigure )
			continue;
		
      		RefreshFigureBorders(*mInterface.GetFigure(),
				mInterface.GetFigure()->figureCenter().negate() + (*it)->GetPositionInGridReferenceFrame(), //(mInterface.GetFigureOrSurfaceSize().largestdimension() / 2)
			(*it)->GetFlipped(), (*it)->GetDiscreteRotation() );
		
		//RefreshFigureBorders( *this->GetEntityFromGridObject(*it)->GetModuleGraphics()->GetFigure().get(), 
		//	Coord(-this->GetEntityFromGridObject(*it)->GetModuleGraphics()->getSize()/2) + (*it)->GetPositionInGridReferenceFrame(),
		//	(*it)->GetFlipped(), (*it)->GetDiscreteRotation() );
	}
}

//void ShipGrid::ClearGridLists()
//{
//	this->pVesselOuterGridList.Clear();
//	this->boxList.Clear();
//	this->pVesselOuterHiddenList.clear();
//}

void ShipGrid::SetVesselInnerGridColor( const Coord& Coord, const ColorRGB& col)
{
	ShipBuilderBox* box = ((ShipBuilderBox*)this->getBox(Coord));
	box->SetColorModulation( col );
}

void ShipGrid::SetVesselOuterGridColor( const Coord& Coord, const ColorRGB& col )
{
	ShipBuilderBox* newBox = new ShipBuilderBox( &imagehandling::whiteParticle, GridObjectCollection::GO_std_whitepack.texture, GridObjectCollection::GO_std_whitepack.texture, GridObjectCollection::GO_std_whitepack.textureSize );
	newBox->setType( enBoxType::OUTER );
	newBox->SetColorModulation( col );
	this->pVesselOuterGridList.InsertBox( std::unique_ptr<GridBox>(newBox), Coord );
}

void ShipGrid::RefreshVesselOutsideBoxColor( const Coord& Coord, const ColorRGB& col)
{
	//if(this->pVesselOuterGridList.CoordIsFilled(Coord))
		this->pVesselOuterGridList[Coord]->RefreshColor(col);
}

void ShipGrid::SetModelVessel( std::weak_ptr<Vessel> vessel) 
{ 
	pModelVessel = vessel;
	pModelFigure = ModuleGlobalList::GetInterface(vessel.lock()->GetChassisName()).GetFigure();
	this->SetOrigo( Coord( this->pModelVessel.lock()->getSize()/2 ));
};

void ShipGrid::RefreshOuterVesselBorders( const Coord& Coord, const PixelBox& box)
{
	ShipBuilderBox* sBox = ((ShipBuilderBox*)this->pVesselOuterGridList[Coord].get());
	sBox->UpdateBorder(BorderData( box, false ) ); 
	sBox->Refresh();
}

void ShipGrid::RefreshInnerVesselBox( const Coord& Coord )
{
	((ShipBuilderBox*)this->getBox( Coord ))->Refresh();
}

void ShipGrid::RefreshInnerVesselBorders( const Coord& Coord, const PixelBox& box, enObjectDiscreteRotation rotation, enObjectFlipped flip)
{
	ShipBuilderBox* sBox = ((ShipBuilderBox*)this->boxList[Coord].get());
	BorderData borderData;
	if(sBox->GetBoxType() == enBoxType::EMPTY )
		borderData = BorderData( box, false );
	else if(sBox->GetBoxType() == enBoxType::WALL )
		borderData = BorderData( box, true );

	borderData.Flip = flip;
	borderData.Rotation = rotation;
	borderData.FlipData(flip);
	borderData.RotateData();
	sBox->UpdateBorder(borderData);
	sBox->Refresh();
}

//void ShipGrid::RefreshFigureBorders( std::shared_ptr<GridObject> gridobject, const Coord& relativePosition, enObjectFlipped flip, enObjectDiscreteRotation drotate )	//should use gridobject, to exclude the need of having modules
//{
//	auto begin = gridobject.get()->begin();
//	auto end   = gridobject->end();
//	Coord fcenter = gridobject->GridOrigo;
//
//	for( auto it = begin; it != end; it++)
//	{
//		if( it.item()->box.isEdge() )
//		{
//			this->SetGridShipEdge(0, it.Coord().flipRotation(flip,fcenter.x).discreteRotation(drotate,fcenter.x) + relativePosition );
//		}
//		
//		this->RefreshInnerVesselBorders(it.Coord().flipRotation(flip,fcenter.x).discreteRotation(drotate,fcenter.x) + relativePosition, it.item()->box );
//	}
//}

void ShipGrid::RefreshFigureBorders( const PixelFigure& figure, const Coord& relativePosition)
{
	RefreshFigureBorders(figure, relativePosition, enObjectFlipped::goNotFlipped, enObjectDiscreteRotation::goNoRotation );
}

void ShipGrid::RefreshFigureBorders( const PixelFigure& figure, const Coord& relativePosition, enObjectFlipped flip, enObjectDiscreteRotation drotate )	//should use gridobject, to exclude the need of having modules
{
	CoordMapIterator<GriddedBox> begin = figure.begin();
	CoordMapIterator<GriddedBox> end   = figure.end();
	Coord fcenter = figure.figureCenter();

	for( auto it = begin; it != end; ++it)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	{
		if( it.item()->box.isEdge() )
		{
			this->SetGridShipEdge(it.Coord().flipRotation(flip, fcenter.x).discreteRotation(drotate, fcenter.x) + relativePosition); //
		}
		
		this->RefreshInnerVesselBorders(it.Coord().flipRotation(flip,fcenter.x).discreteRotation(drotate,fcenter.x) + relativePosition, it.item()->box, drotate, flip);
	}
}

GridBox* ShipGrid::getBox( const Coord& pos)
{
	//Are outer grid vessel boxes necessary?????

	//if( pShowVesselOutside )
	//{
	//	if( this->pVesselOuterGridList[pos].get()->getBoxState() == enBoxState::HIDE ) //should not be in here!!!
	//	{
	//		HighlightHiddenSet();  //should not be in here!!!
	//		return boxList[ pos ].get();
	//	}
	//	
	//	DisableHighlightHiddenSet(); //should not be in here!!!
	//	return this->pVesselOuterGridList[pos].get();
	//}
	//else
		return boxList[ pos ].get();
}

void ShipGrid::HighlightHiddenSet()
{
	if( breachHighlighted )
		return;

	breachHighlighted = true;
	for( std::set<Coord>::iterator it = pVesselOuterHiddenList.begin(); it != pVesselOuterHiddenList.end(); ++it )
	{
		this->boxList[*it].get()->SetBoxRedFlag( true );
		this->boxList[*it].get()->setState(enBoxState::INVALID);
		// render grid objects in ship red??
	}
}

void ShipGrid::DisableHighlightHiddenSet()
{	
	if(!breachHighlighted)
		return;

	breachHighlighted = false;
	for( std::set<Coord>::iterator it = pVesselOuterHiddenList.begin(); it != pVesselOuterHiddenList.end(); ++it )
	{
		//this->boxList[*it].get()->SetBoxFlag( false );
		if( this->IsBoxDestroyed( *it ) )
			this->boxList[*it].get()->setState(enBoxState::HIDE);
		else
		{
			this->boxList[*it].get()->SetBoxRedFlag( false );
			this->boxList[*it].get()->setState(enBoxState::STANDARD);
		}
		//Modulate( ColorRGB(255,255,255) );
	}
}

void ShipGrid::SetShowVesselOutside(bool set)
{ 
	//if (this->pShowVesselOutside == set)
	//	return;

	this->pShowVesselOutside = set;
	if (this->pShowVesselOutside)
	{
		ShowVesselOutsideGridObjects();
	}
	else
	{
		ShowVesselInsideGridObject();
	}
	DisableHighlightHiddenSet(); //was previously only in SwitchShowVesselOutside
}

void ShipGrid::SwitchShowVesselOutside() 
{ 
	//this->pShowVesselOutside = !this->pShowVesselOutside;
	SetShowVesselOutside(!this->pShowVesselOutside);
};

void ShipGrid::handleEvent(SDL_Event& event)
{
	switch (this->pGridSelectionType)
	{
	case(enGridSelectType::Hotkeys):
		handleEventHotkeys(event);
		break;
	case(enGridSelectType::Fight):
		handleEventFighter(event);
		break;
	case(enGridSelectType::Builder):
		GridObjectViewer::handleEvent(event);
		break;
	default:
		GridObjectViewer::handleEvent(event);
	}
}

void ShipGrid::handleEventFighter(SDL_Event& event)
{
	bool selectedGridObject = this->selectedGridObject.get();
	std::shared_ptr<AmmoContainer> derived;
	if (selectedGridObject)
	{
		if (event.button.button == SDL_BUTTON_MIDDLE || event.button.button == SDL_BUTTON_RIGHT)
			derived = std::dynamic_pointer_cast<AmmoContainer>(GetEntityFromGridObject(this->selectedGridObject));
		
		if (derived != nullptr)
		{
			GameManager::Screens.openAmmoContainer();
		}
		else
		{
			GameManager::Screens.closeAmmoScreen();
		}
		this->selectedGridObject.reset();
	}
	else
		GridObjectViewer::handleEvent(event);

}

void ShipGrid::handleEventHotkeys(SDL_Event& event)
{
	bool selectedGridObject = this->selectedGridObject.get();
	if (selectedGridObject)
	{
		this->selectedGridObject->RunHotKeyEvent(event);
		if (this->selectedGridObject->HotKeyIsSelected() == false)
		{
			this->selectedGridObject.reset();
			//return;
		}
		else
		{
			switch (event.type)
			{
			case(SDL_MOUSEBUTTONDOWN):
				this->selectedGridObject->SelectHotKeyLink(false);
				this->selectedGridObject.reset();
				break;
			}
		}
	}
	else
		GridObjectViewer::handleEvent(event);
}

void ShipGrid::handleGridObjectHoveredMouseMotion(SDL_Event& e, std::weak_ptr<GridObject> hoveredObj, const Coord&)
{
	//if (pGridSetHotKeys == true)
	//{
	//	//hoveredObj.lock()->pHotKeyButton.link->sethovered();
	//}
}

void ShipGrid::handleGridObjectHoveredMouseDown(SDL_Event& e, std::weak_ptr<GridObject> hoveredObj, const Coord& mousepos)
{
	//left mouse button = normal, right = open container

	if (IsShowHotkeys() == false)
	{
		if (e.button.button == SDL_BUTTON_MIDDLE || e.button.button == SDL_BUTTON_RIGHT)	//
		{
			if (auto derived = std::dynamic_pointer_cast<AmmoContainer>(GetEntityFromGridObject(hoveredObj.lock())))
				GameManager::Screens.openAmmoContainer();
		}
		else
			GridObjectViewer::handleGridObjectHoveredMouseDown(e, hoveredObj, mousepos);
	}	
	else
	{
		hoveredObj.lock()->SelectHotKeyLink(true);
		setNewSelectedObject(hoveredObj.lock(), (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)));
		pHoveredGridObject.lock()->disableHoveredState();
		pHoveredGridObject.reset();
	}
}

void ShipGrid::RotateGBClockwise()
{
	GridObjectViewer::RotateGBClockwise();
	std::shared_ptr<ModuleEntity> module = this->GetEntityFromGridObject( this->selectedGridObject );
	if(module.get())
		module->RotateClockwise(geometry::PI_div2);
}
//void ShipGrid::FlipGBDown()
//{
//	GridObjectViewer::FlipGBDown();
//}
void ShipGrid::RotateGBAntiClockwise()
{
	GridObjectViewer::RotateGBAntiClockwise();
	std::shared_ptr<ModuleEntity> module = this->GetEntityFromGridObject( this->selectedGridObject );
	if(module.get())
		module->RotateClockwise( -geometry::PI_div2 );
}
void ShipGrid::Flip()
{
	GridObjectViewer::Flip();
	std::shared_ptr<ModuleEntity> module = this->GetEntityFromGridObject( this->selectedGridObject );
	if(module.get())
		module->FlipGraphics();
}	

bool ShipGrid::AddGridObject( std::shared_ptr<GridObject> object)
{
	if( InventoryItemGrid::AddGridObject(object) == true )
	{
		auto itemType = this->GetItemTypeFromObject(object);
		this->SetShowVesselOutside(itemType == enItemType::ExternalModuleItem);

		//if(pShowVesselOutside == false && this->GetItemTypeFromObject(object) == enItemType::ExternalModuleItem )
		//	object->SetState( enGridObjectState::goHide );
		return true;
	}
	return false;
}

std::shared_ptr<ModuleEntity> ShipGrid::GetEntityFromGridObject( std::shared_ptr<GridObject> gridobject)
{
	auto shipModel = this->GetModelVessel();
	if (shipModel == nullptr)
		return nullptr;

	/*auto externalList = this->GetModelVessel()->GetExternalModulesList();
	for( auto it = externalList.begin(); it != externalList.end(); ++it )
	{
		if( (*it)->getGridObject() == gridobject )
			return (*it);
	}

	auto internalList = this->GetModelVessel()->GetInternalModulesList();
	for (auto it = internalList.begin(); it != internalList.end(); ++it)
	{
		if ((*it)->getGridObject() == gridobject)
			return (*it);
	}*/
	auto result = this->GetItemFromGridObject(gridobject.get());
	if (result != nullptr && (result->getType() == enItemType::InternalModuleItem || result->getType() == enItemType::ExternalModuleItem))
		return std::dynamic_pointer_cast<ModuleEntity>(result);

	return nullptr;
}

bool ShipGrid::CoordHasBox( const Coord& Coord)
{
	if( pShowVesselOutside )
	{
		return pVesselOuterGridList.CoordIsFilled(Coord);
	}
	else
		return Grid::CoordHasBox(Coord);
}

void ShipGrid::ResetBoxState(GridBox* box)
{
	Grid::ResetBoxState(box);
	ShipBuilderBox* builderbox = (ShipBuilderBox*)box;
	builderbox->setType(enBoxType::EMPTY);
	builderbox->UpdateBorder(BorderData());
}

bool ShipGrid::isBoxFillable( const Coord& pos, enItemType itemtype) //can place gridobject in box
{
	bool result = true;
	if( isBoxHidden(pos) )
		result = false;
	else
	{
		ShipBuilderBox* box = ((ShipBuilderBox*)getBox( pos ));

		if(box->GetBoxType() == enBoxType::WALL )
			result = false;
	}
	if(itemtype == enItemType::ExternalModuleItem )
		return !result;
	return result;
}

bool ShipGrid::IsItemAllowedForGrid(enItemType type)
{
	return type == enItemType::ExternalModuleItem ||
		type == enItemType::InternalModuleItem;
}

ObjectIntersectResult ShipGrid::GridObjectIntersectionResult(GridObject* A)
{
	ObjectIntersectResult result;
	enItemType itemType = this->GetItemTypeFromObject(A);

	if (this->IsItemAllowedForGrid(itemType) == false)
	{
		result.invalid = true;
		A->setBoxesInvalid();
		return result;
	}

	result = GridObjectViewer::ObjectBoxesIntersectWithObjects(this->selectedGridObject.get());

	if (result.bundled || result.invalid)
		return result;

	//return ObjectBoxesIntersectWithItem(A);

	return ObjectBoxesIntersectWithShip(A, itemType);
}

//ObjectIntersectResult ShipGrid::ObjectBoxesIntersectWithItem(GridObjectItem* ItemObject)
//{
//	ObjectIntersectResult result;
//
//	for (auto it = ItemObject->begin(); it != ItemObject->end(); ++it)
//	{
//		if ((*it).second->getBoxState() == enBoxState::UNKNOWN)
//		{
//			Coord boxFramePosition = ItemObject->GetPositionInGridReferenceFrame() + (*it).first; //ItemObject->getRelativePosition() - ItemObject->ObjectOrigo + (*it).first;
//			if (isBoxFillable(boxFramePosition, ItemObject->GetGridObjectType()))
//			{
//				(*it).second->setState(enBoxState::INVALID);
//				result.invalid = true;
//			}
//			else if ((*it).second->getBoxState() != enBoxState::INVALID)
//				(*it).second->setState(enBoxState::STANDARD);
//		}
//	}
//	return result;
//}

ObjectIntersectResult ShipGrid::ObjectBoxesIntersectWithShip(GridObject* object, enItemType objectItem)
{
	ObjectIntersectResult result;
	for (auto it = object->begin(); it != object->end(); ++it)
	{
		Coord boxFramePosition = object->GetPositionInGridReferenceFrame() + (*it).first;
		if (isBoxFillable(boxFramePosition, objectItem) != (*it).second->IsBoxRedFlagged() && (*it).second->getBoxState() != enBoxState::INVALID)
		{
			(*it).second->setState(enBoxState::STANDARD);
		}
		else
		{
			(*it).second->setState(enBoxState::INVALID);
			result.invalid = true;
		}
	}
	return result;
}
//
//ObjectIntersectResult ShipGrid::ObjectBoxesIntersectWithObjects( GridObject* object )
//{
//
//
//
//		
//	}
//	else
//		for( auto it = object->begin(); it != object->end(); ++it )
//		{
//			Coord boxFramePosition = object->GetPositionInGridReferenceFrame() + (*it).first; //->getRelativePosition() - object->ObjectOrigo ;
//			if( !isBoxFillable(boxFramePosition, object->GetGridObjectType()) && (*it).second->IsBoxRedFlagged() )
//			{
//				if( (*it).second->getBoxState() != enBoxState::INVALID )
//					(*it).second->setState( enBoxState::STANDARD);
//			}
//			else if( !isBoxFillable(boxFramePosition, object->GetGridObjectType()) && !(*it).second->IsBoxRedFlagged() )
//			{
//				(*it).second->setState( enBoxState::INVALID);
//				result.invalid = true;
//			}
//			else if( isBoxFillable(boxFramePosition, object->GetGridObjectType()) && !(*it).second->IsBoxRedFlagged() )
//			{
//				if( (*it).second->getBoxState() != enBoxState::INVALID )
//					(*it).second->setState( enBoxState::STANDARD);
//			}
//			else
//			{
//				(*it).second->setState( enBoxState::INVALID);
//				result.invalid = true;
//			}
//
//		}
//	return result;
//}

void ShipGrid::ShowVesselOutsideGridObjects()
{
	for (auto it = this->gridObjectBegin(); it != this->gridObjectEnd(); ++it)
	{
		if (this->GetItemTypeFromObject(*it) == enItemType::InternalModuleItem)
			(*it)->SetState(enGridObjectState::goNoSelect);
		else
			(*it)->SetState(enGridObjectState::goNormal);
	}
}
void ShipGrid::ShowVesselInsideGridObject()
{
	for (auto it = this->gridObjectBegin(); it != this->gridObjectEnd(); ++it)
	{
		if (this->GetItemTypeFromObject(*it) == enItemType::ExternalModuleItem)
			(*it)->SetState(enGridObjectState::goHide);
		else
			(*it)->SetState(enGridObjectState::goNormal);
	}

	try
	{
		this->generateGrid(this->getSize());
	}
	catch (int e)
	{
		this->SwitchShowVesselOutside();	//show ship outside 
	}
}

void ShipGrid::DrawVesselUsingGridObjects()
{
	this->AddGridBoxesFromShipTemplate();
	this->ResetSelectedGridBox();
	SetShowVesselOutside(this->pShowVesselOutside);
}

void ShipGrid::SetCurrentSelectedGridBox( const Coord& Coord)
{
	this->currentSelectedGridBox = Coord;
}
