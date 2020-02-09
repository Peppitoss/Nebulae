#pragma once
#include"scrGrid.h"
#include"GridObjectViewer.h"

//class GridFrame;

class GridObjectFrame : public GridFrame
{

public:

	GridObjectFrame();
	GridObjectFrame( spriteSize, Coord pos );
	void render( SDL_Renderer& ) override;
	virtual void renderGridObjects( SDL_Renderer& );
	virtual void renderGridObject( SDL_Renderer& , GridObject* );

	virtual void HandleEvent( SDL_Event& e);

	bool GridSatisfiesUndoTransfer();

	virtual std::weak_ptr<ITransferObject> selectedObject();
	void setSelectedObject( std::shared_ptr<GridObject> set );
	virtual void RemoveSelectedGridObject();
	virtual bool AddGridObject( std::shared_ptr<GridObject> object );
	virtual bool AddGridObject( std::shared_ptr<GridObject> object, const Coord & pos );
	virtual bool AddGridObject(std::shared_ptr<ITransferObject> object);
	virtual bool AddGridObject(std::shared_ptr<ITransferObject> object, const Coord & pos);
	bool AddNewGridObject(std::shared_ptr<ITransferObject> object, const Coord & pos);
	virtual void RemoveGridObject( std::shared_ptr<GridObject> object );
	virtual void RemoveGridObject(std::shared_ptr<ITransferObject> object);
	Coord GetBoxPositionFromMousePosition();
	Coord GetBoxPositionFromMousePosition( const Coord mousepos );
	Coord GetGridOrigo();



	void SetFlagGridObject_SeperateGrid( int value ) { this->GetGrid()->FLAG_GRIDOBJECT_SELECTED_FROM_SEPERATE_GRID = value; };
	// //should ideally not be public!!

	GridObjectViewer* GetGrid();

protected:

	void Setup(spriteSize size, spriteSize gridBoxPerSide) override;
	void OnCloseScreen();

private:

	GridObjectFrame( const GridObjectFrame& );
	GridObjectFrame( GridObjectFrame&& );
	GridObjectFrame& operator=( const GridObjectFrame& );
	GridObjectFrame& operator=(GridObjectFrame&& );
	
};
