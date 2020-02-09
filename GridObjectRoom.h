#pragma once
#include"GridObject.h"


//class GridObjectRoom : public GridObject
//{
//public:
//	GridObjectRoom( const BoxList& gridPos, const GridObjectTexturePack& );
//
//	bool PositionIsWall( Coord );
//	bool PositionIsFloor( Coord );
//
//	void render( Coord pos, SDL_Renderer* renderer, int scaleFactor) const override;
//
//	void RemoveObjectFromRoom(GridObject* );
//	void AddObjectToRoom(GridObject* add);
//	bool isLocked();
//
//protected:
//
//	void initialize() override;
//
//	SDL_Texture* GetTextureFromBox( GridBox* box, SDL_Renderer* ) const override;
//
//private:
//
//	std::list<GridObject*> objectsInRoom; // pointers used for reference only (no new or delete!)
//
//};
