#include"GridObjectRoom.h"
#include"GridBoxTextureCollection.h"
#include<algorithm>
//
//GridObjectRoom::GridObjectRoom( const BoxList& list, const GridObjectTexturePack& txtPack )
//{
//
//	pObjectWidth = list.getWidth();
//	pObjectHeight = list.getHeight();
//
//	int width_corrigation = 0;
//	if( pObjectWidth % 2 )
//		width_corrigation = -1;
//
//	this->pBoxSize = spriteSize(spriteSize::GetSize64());
//
//	for( std::list<Coord>::const_iterator it = list.begin(); it != list.end(); ++it )
//	{
//		if( !mapHasKey( (*it) ) )
//		{
//			ShipBuilderBox* newBox = new ShipBuilderBox( GBTextureCollection::ImportGrid_Standard.get(), txtPack.texture, txtPack.texture, txtPack.textureSize );
//			//((SpriteBorderGraphic*)newBox->getpStandardTxt.get())->UpdateTextureCrop(renderer,data);
//
//
//			if( list.isCorner( *it ) ||  list.isEdge( *it ) )
//			{
//				newBox->setType( enBoxType::WALL );
//			}
//			else
//			{
//				newBox->setType( enBoxType::EMPTY );
//			}
//
//			newBox->UpdateBorder( list.GetBorderFromCoord( *it, false) );
//			this->pBoxes[ (*it) ] = std::unique_ptr<GridBox>(newBox );
//		}
//	}
//	initialize();
//}
//
//void GridObjectRoom::initialize()
//{
//	GridObject::initialize();
//	objectsInRoom = std::list<GridObject*>();
//	SortOrder = -1;
//	GridOrigo = Coord(0,0);
//	ObjectOrigo = Coord(0,0);
//	//this->ObjectOrigo = Coord(std::max(pObjectWidth,pObjectHeight)/2);
//	//pType = enumGridType::GRIDROOM;
//
//}
//
//void GridObjectRoom::render( Coord pos, SDL_Renderer* renderer, int scaleFactor) const
//{
//	GridObject::render(pos,renderer,scaleFactor);
//
//	for( auto it = this->objectsInRoom.begin(); it != this->objectsInRoom.end(); ++it )
//	{
//		(*it)->render(pos, renderer, scaleFactor );
//	}
//}
//
//void GridObjectRoom::RemoveObjectFromRoom(GridObject* add)
//{
//	//std::list<GridObject*>::const_iterator findIter = std::find(objectsInRoom.begin(), objectsInRoom.end(), add);
//
//	//if( findIter != objectsInRoom.end() )
//	objectsInRoom.remove(add);
//	add->SetContained(false);
//}
//
//void GridObjectRoom::AddObjectToRoom(GridObject* add)
//{
//	std::list<GridObject*>::const_iterator findIter = std::find(objectsInRoom.begin(), objectsInRoom.end(), add);
//
//	if( findIter == objectsInRoom.end() )
//	{
//		objectsInRoom.push_back(add);
//		add->SetContained(true);
//	}
//}
//
//bool GridObjectRoom::isLocked()
//{
//	if( objectsInRoom.size() > 0 )
//		return true;
//	return false;
//}
//
//SDL_Texture* GridObjectRoom::GetTextureFromBox( GridBox* box, SDL_Renderer* renderer) const
//{
//	return ((ShipBuilderBox*)box)->GetTextureByState(renderer);
//}
//
//bool GridObjectRoom::PositionIsWall( Coord pos )
//{
//	if( !((ShipBuilderBox*)this->pBoxes[pos].get())->GetBoxType() == enBoxType::EMPTY )
//			return true;
//	return false;
//}
//
//bool GridObjectRoom::PositionIsFloor( Coord pos )
//{	
//		if( ((ShipBuilderBox*)this->pBoxes[pos].get())->GetBoxType() == enBoxType::EMPTY )
//			return true;
//	return false;
//}