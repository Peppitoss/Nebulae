#pragma once
#include<map>
#include"GridBox.h"
#include"CoordMap.h"

template class CoordMap<GridBox>;

class Grid //make abstract
{
	
public:

	Grid( spriteSize size, Coord position, spriteSize boxesPerSide  );


	virtual void RenderGridBase( SDL_Renderer* );
	virtual void RenderGridBox( SDL_Texture* , SDL_Renderer* , const Coord& boxPos );

	virtual void Render( SDL_Renderer* );
	virtual void generateGrid( int size );
	virtual void generateGrid( int x, int y, enBoxState initState );
	virtual	void generateGridRows( int x, int y );
	virtual void generateGridRows( int x, int y, enBoxState initState );
	virtual void generateGrid( int size, enBoxState initState );

	virtual GridBox* GetNewGridBox();

	virtual void FillGrid(enBoxState state);
	void AddBox( int x, int y );
	virtual void AddBox( int x, int y, enBoxState );
	virtual void setBoxState( const Coord& pos, enBoxState set);
	Coord boxPositionFromMousePosition( const Coord& );

	spriteSize getSpriteSize() { return this->pSize; };
	int getSize() const { return this->pSize.GetSize(); };
	int getGridboxThickness() { return pDescaleFactor*getSize()/(GridBox::BOXSIZE); };
	int getDescaleFactor() { return this->pDescaleFactor; };
	Coord GetPosition() { return position; };
	Coord GetBoxDrawPosition( const Coord& boxPos );

	virtual void SetBoxesPerSide( spriteSize boxesPerSide ) ;
	void GenerateGridTexture( const imagehandling::IMGToSurface* surface);

	int GetBoxesPerSize() const;
	//int GetColumns() const { return this->pColumns; };
	
	Coord Origo() { return this->pOrigo; };
	void SetOrigo( Coord set ) { this->pOrigo = set; };

	Coord RowColumnDimensions();

protected:

	Coord currentSelectedGridBox;
	Coord position;
	int pDescaleFactor; // one grid has size 64pixels, determines the amount of gridboxes in the grid by scaling their size viewed on the screen.
	spriteSize pSize; //THIS SIZE IS only 

	virtual GridBox* getBox( const Coord& );
	CoordMap<GridBox> boxList;
	//std::map<Coord,std::unique_ptr<GridBox>> boxList;

	virtual void SetCurrentSelectedGridBox( const Coord& );
	virtual void handleGridBoxSelect( SDL_Event& e,  const Coord& boxPos  );
	virtual void handleGridMouseHover( SDL_Event& e, const Coord& );
	virtual bool CoordHasBox( const Coord& pos);

	bool currentSelectedBoxIsValid();
	//virtual void Resize( spriteSize size, spriteSize boxesPerSide);

	Coord selectedGridPos;

	bool mouseInsideGrid( const Coord& );
	bool isBoxHidden( Coord pos );

	virtual void GridBoxDownEvent();

	void SetRows( int set ) { this->pRows = set; };
	void SetColumns( int set ) { this->pColumns = set; };

	void ResetBoxStates();
	virtual void ResetBoxState(GridBox* box);
	void ResetSelectedGridBox();

private:

	Coord pOrigo;
	int pRows;
	int pColumns;

	Grid( const Grid& );
	Grid( Grid&& );
	Grid& operator=(const Grid& );
	Grid& operator=( Grid&& );

};
