#include "Grid.h"
#include"GridBoxTextureCollection.h"
#include"DynamicGridBox.h"
#include"GridObject.h"
#include"Debug.h"

Grid::Grid( spriteSize size, Coord pos, spriteSize boxesPerSide )
{
	
		if( size.GetSize() < GridBox::BOXSIZE )
		pSize = spriteSize::GetSize64(); //GridBox::BOXSIZE;
	else
		pSize = size;

	this->SetBoxesPerSide( boxesPerSide );
	this->position = pos;
	this->pSize = size;
	this->pOrigo = Coord(0,0);
	this->pRows = -1;
	this->pColumns = -1;
	this->ResetSelectedGridBox();

}

GridBox* Grid::GetNewGridBox()
{
	return new GridBox(GBTextureCollection::ImportGrid_Standard.get());
}

//void Grid::generateGrid( int size )
//{
//
//	int rows = pDescaleFactor*size/(GridBox::BOXSIZE);
//	int columns = pDescaleFactor*size/(GridBox::BOXSIZE);
//
//	//generate top edge
//
//	for( int i = 0; i<rows; i++ )
//	{
//		for( int j = 0; j<columns; j++ )
//		{
//			//GridBox* newBox =  //add textures
//			boxList[ Coord(j,i) ] = std::unique_ptr<GridBox>( new GridBox( GBTextureCollection::ImportGrid_Standard.get() ));
//		}
//	}
//}

void Grid::generateGridRows( int x, int y )
{
	generateGridRows(x,y,enBoxState::STANDARD );
}

void Grid::generateGridRows( int x, int y, enBoxState initState )
{
	pRows = y;
	pColumns = x;
	//this->SetOrigo( Coord( std::max(rows,columns)/2  ));
	//generate top edge

	for( int i = 0; i<pRows; i++ )
	{
		for( int j = 0; j<pColumns; j++ )
		{
			AddBox(j,i,initState);
		}
	}
}

void Grid::generateGrid( int xSize, int ySize, enBoxState initState )
{

	if( this->boxList.GetInsertedBoxes() != 0 )	//demand that teh boxlist is empty before refilling it, use ResetSize(), or change boxesperside which can trigger ResetSize()
		return;

	pRows = pDescaleFactor*ySize/(GridBox::BOXSIZE);
	pColumns = pDescaleFactor*xSize/(GridBox::BOXSIZE);
	//generate top edge

	FillGrid(initState);
}

void Grid::FillGrid(enBoxState state)
{
	for( int i = 0; i<pRows; i++ )
	{
		for( int j = 0; j<pColumns; j++ )
		{
			AddBox(j,i,state);
		}
	}
}

void Grid::generateGrid( int size, enBoxState initState )
{
	generateGrid(size,size,initState);
}

void Grid::generateGrid( int size )
{
	Grid::generateGrid(size, enBoxState::STANDARD );
}


void Grid::Render( SDL_Renderer* renderer )
{
	RenderGridBase( renderer );
}

void Grid::SetCurrentSelectedGridBox( const Coord& Coord)
{
	this->currentSelectedGridBox = Coord;
}


void Grid::AddBox( int x, int y )
{
	AddBox(x,y, enBoxState::STANDARD );
}

void Grid::setBoxState( const Coord& pos, enBoxState set) 
{ 
	this->boxList[pos]->setState( set ); 
}

void Grid::AddBox( int x, int y, enBoxState state)
{
	auto newBox = GetNewGridBox();
	newBox->setState( state );
	boxList.InsertBox( std::shared_ptr<GridBox>(newBox), Coord(x,y));
}


void Grid::RenderGridBase( SDL_Renderer* renderer)
{
	CoordMapIterator<GridBox> begin = this->boxList.Begin();
	CoordMapIterator<GridBox> end = this->boxList.End();

	for( auto it = begin; it != end; ++it )
	{
		SDL_Texture* texture = it.item()->getStandardTexture(renderer);
		RenderGridBox( texture, renderer, it.Coord() );
		if( it.item()->ShowIcon() )
			RenderGridBox( it.item()->getIconTexture(renderer), renderer, it.Coord() );
	}
}

void Grid::RenderGridBox( SDL_Texture* texture, SDL_Renderer* renderer, const Coord& pos)
{
	Coord drawPosition = GetBoxDrawPosition(pos);

	int textureSize = GridBox::BOXSIZE/this->getDescaleFactor();

	SDL_Rect renderQuad = { drawPosition.x, drawPosition.y, textureSize, textureSize};
	SDL_RenderCopy( renderer, texture, NULL, &renderQuad );
	Debug::SDLErrorCheck();

}

void Grid::handleGridMouseHover( SDL_Event& e, const Coord& )
{
	//not implemented yet, see GridObjectViewer
}

void Grid::handleGridBoxSelect( SDL_Event& event, const Coord& boxPos )
{
	
	if (!CoordHasBox(boxPos))
	{
		ResetSelectedGridBox();
		return;
	}

	if (isBoxHidden(boxPos))
	{
		SetCurrentSelectedGridBox(boxPos);
		return;
	}

	switch( event.type )
		{
		case SDL_MOUSEMOTION:
			if( currentSelectedGridBox != boxPos )
			{
				if(CoordHasBox(boxPos))
					this->getBox(boxPos)->setState(enBoxState::HIGHLIGHTED);
				ResetSelectedGridBox();
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			GridBoxDownEvent();
			break;

		case SDL_MOUSEBUTTONUP:
			break;
		}
		SetCurrentSelectedGridBox(boxPos);
}

void Grid::GridBoxDownEvent()
{
}

bool Grid::currentSelectedBoxIsValid() 
{ 
	return currentSelectedGridBox.x >= 0 && currentSelectedGridBox.y >= 0 && boxList.CoordIsFilled(currentSelectedGridBox);
}

Coord Grid::GetBoxDrawPosition( const Coord& boxPos )
{
	return Coord(this->position.x + boxPos.x*GridBox::BOXSIZE/this->getDescaleFactor() , this->position.y + boxPos.y*GridBox::BOXSIZE/this->getDescaleFactor());
}


bool Grid::mouseInsideGrid( const Coord& pos )
{
	return geometry::pointWithinSquare_inclusive( pos, this->position, this->getSize() );
}

Coord Grid::boxPositionFromMousePosition( const Coord& pos )
{
	int boxSizes = GridBox::BOXSIZE/pDescaleFactor; 
	Coord result = (Coord(pos)-this->position)/boxSizes;
	return result;
}


void Grid::SetBoxesPerSide( spriteSize boxesPerSide ) 
{
	int boxesperside = boxesPerSide.GetSize();

	if( boxesperside > pSize.GetSize() )
		boxesperside = pSize.GetSize();

	if( boxesperside*boxesperside != this->boxList.IndexSize() || this->boxList.IndexSize() == 0)
	{
		this->boxList.ResetSize();
		this->boxList = CoordMap<GridBox>(boxesPerSide.GetSize());
	}

	pDescaleFactor = std::fmax(1,(GridBox::BOXSIZE)*boxesPerSide.GetSize()/this->getSize());
	this->SetOrigo(Coord(boxesPerSide.GetSize())/2);
};

int Grid::GetBoxesPerSize() const
{
	return pDescaleFactor*this->getSize()/(GridBox::BOXSIZE);
}

bool Grid::CoordHasBox(const Coord& Coord)
{
	return boxList.CoordIsFilled(Coord);
}

GridBox* Grid::getBox( const Coord& pos)
{
	return boxList[ pos ].get();
}

bool Grid::isBoxHidden( Coord pos )
{
	if (!CoordHasBox(pos) || getBox(pos)->getBoxState() == enBoxState::HIDE)
		return true;
	return false;
}

void Grid::GenerateGridTexture( const imagehandling::IMGToSurface* surface )
{
	BoxList list = BoxList(pColumns,pRows);
	int sizeInt = STANDARDOBJECTSIZE.GetSize();//this->pBoxSize.getIntValue();
	this->boxList.Clear();

	std::vector<CoordWithColor> canvas = std::vector<CoordWithColor>();
	canvas.reserve(sizeInt*sizeInt);
	for( int i = 0; i < sizeInt*sizeInt; i++ )
		canvas.push_back( CoordWithColor() );

	for( std::list<Coord>::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		Coord relativePosition = list.getLowestPosition()*-1 + (*it);

		for( int x = 0; x<sizeInt;x++)
		{
			for( int y = 0; y<sizeInt;y++)
			{
				canvas[y+x*sizeInt] = CoordWithColor(x,y,surface->get_pixel32RGB(x+relativePosition.x*sizeInt,y + relativePosition.y*sizeInt)); //x+y*sizeInt is correct?
			}
		}

		if( !this->CoordHasBox( (*it) ) )
		{
			imagehandling::IMGToSurface* box = new imagehandling::IMGToSurface(canvas,STANDARDOBJECTSIZE);
			this->boxList.InsertBox( std::unique_ptr<GridBox>(new DynamicGridBox( box )), (*it) );
		}
	}
}

void Grid::ResetBoxStates()
{
	for (auto box = this->boxList.Begin(); box != this->boxList.End(); ++box)
	{
		ResetBoxState(box.item().get());
	}
}

void Grid::ResetBoxState(GridBox* box)
{
	box->setState(enBoxState::STANDARD);
	box->SetBoxRedFlag(false);
}

void Grid::ResetSelectedGridBox()
{
	if (currentSelectedBoxIsValid())
	{
		ResetBoxState(getBox(currentSelectedGridBox));
		this->SetCurrentSelectedGridBox( Coord(-1,-1) );}
}

Coord Grid::RowColumnDimensions()
{
	return Coord( pColumns, pRows );
}