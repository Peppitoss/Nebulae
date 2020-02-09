#pragma once
#include"Grid.h"
#include"GridObject.h"

class GridObjectViewer : public Grid
{
public:
	GridObjectViewer( spriteSize size, Coord position, spriteSize boxesPerSide  ); 

	virtual void Render( SDL_Renderer* renderer ) override;
	void RenderGridBase( SDL_Renderer* ) override;
	virtual void RenderGridObjects( SDL_Renderer*);
	virtual void RenderGridObject( SDL_Renderer*, GridObject* );
	void RenderSelectedGridObject(SDL_Renderer*);

	virtual void handleEvent( SDL_Event& e );


	void RemoveSelectedGridObject();
	void RemoveGridObject( std::shared_ptr<GridObject> );
	virtual void RemoveAllGridObjects();

	virtual std::weak_ptr<ITransferObject> GetSelectedGridObject() { return selectedGridObject; };
	std::weak_ptr<GridObject> GetHoveredGridObject() { return pHoveredGridObject; };

	void AddBox( int x, int y, enBoxState state) override;
	bool AddNewGridObject(std::shared_ptr<GridObject>, Coord position);
	bool AddGridObject( std::shared_ptr<GridObject> , Coord position );
	virtual bool AddGridObject( std::shared_ptr<GridObject> ); 

	void setNewSelectedObject( std::shared_ptr<GridObject> set );
	void setNewSelectedObject( std::shared_ptr<GridObject>, bool SingleObjectSelect );

	bool DoesPointContainGridObject( Coord mousepos );
	bool ContainsGridObject( std::shared_ptr<GridObject> object );
	bool ContainsAnyGridObject() const { return this->pGridObjects.size() > 0; }
	int GridObjectsAmount() const { return this->pGridObjects.size(); }

	std::list<std::shared_ptr<GridObject>>::const_iterator const gridObjectBegin() const { return pGridObjects.cbegin(); }
    std::list<std::shared_ptr<GridObject>>::const_iterator const gridObjectEnd() const { return pGridObjects.cend(); }
	
	void ResetSelectedGridObject();
	// ----- hackish code:
	// ----- FLAGS are used as a mean by exterior managers or classes, (i.e srcShipBuilder) to communicate or affect the grid
	// ----- The flags are not used within the class to make internal changes 
	int FLAG_GRIDOBJECT_SELECTED_FROM_SEPERATE_GRID;
	int FLAG_SELECTED_GRIDOBJECT_INVALID_PLACEMENT;

protected:


	virtual void handleGridObjectHovered( SDL_Event& e, std::weak_ptr<GridObject>, const Coord&   );
	virtual void handleGridObjectSelect( SDL_Event& e,  const Coord& mousepos );
	void handleGridMouseHover( SDL_Event& e, const Coord&   ) override;

	virtual void handleGridObjectHoveredMouseMotion(SDL_Event& e, std::weak_ptr<GridObject>, const Coord&);
	virtual void handleGridObjectHoveredMouseDown(SDL_Event& e, std::weak_ptr<GridObject>, const Coord&);

	void AddNewGridObjectHovered( std::shared_ptr<GridObject> object );
	//virtual bool RenderGridObjectLast(std::shared_ptr<GridObject>);
	virtual bool AllowRenderGridObject(std::shared_ptr<GridObject>);

	std::shared_ptr<GridObject> selectedGridObject;
	std::weak_ptr<GridObject> pHoveredGridObject;

	virtual void RotateGBClockwise();
	virtual void Flip();
	virtual void RotateGBAntiClockwise();

	void setSelectedGridObject( GridObject* set );	
	
	
	std::weak_ptr<GridObject> newSelectedObjectFromMousePosition( Coord mousepos );
	std::list<std::shared_ptr<GridObject>> pGridObjects;

	bool ObjectBoundaryIntersect( GridObject* A, GridObject* B );
	bool ObjectBoundaryIntersectWithGridObjects( GridObject* A  );

	virtual DynamicGridBox* GetNewGridBox();
	virtual std::shared_ptr<imagehandling::IMGToSurface> HighligthTexture() { return GBTextureCollection::ImportGrid_Highligt; }

	virtual ObjectIntersectResult GridObjectIntersectionResult(GridObject* A);

	virtual ObjectIntersectResult ObjectBoxesIntersect( GridObject* A, GridObject* B );
	ObjectIntersectResult ObjectBoxesIntersectWithObjects( GridObject* A);

	//bool isBoxFloor( const Coord& );

private:

	GridObjectViewer( const GridObjectViewer& );
	GridObjectViewer( GridObjectViewer&& );
	GridObjectViewer operator=(const GridObjectViewer& );
	GridObjectViewer operator=( GridObjectViewer&& );
};
