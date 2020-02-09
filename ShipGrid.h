#pragma once
#include"InventoryItemGrid.h"
#include"Vessel.h"
#include<set>

enum enGridSelectType
{
	Builder = 1,
	Hotkeys = 2,
	Fight = 3
};

struct GridBoxSelectionResult
{
	GridBoxSelectionResult();
	int outsideHp;
	int insideHp;
};

class ShipGrid : public InventoryItemGrid
{
public:

	ShipGrid( spriteSize size, Coord position, spriteSize boxesPerSide  ); 

	void generateGrid( int size ) override;
	void generateGrid( int xSize, int ySize, enBoxState initState ) override;
	void AddBox( int x, int y, enBoxState state ) override;
	void SetGridShipEdge( const Coord& position );
	void SetGridShipInterior( const Coord& position );
	virtual void RenderGridBase( SDL_Renderer* ) override;
	
	void RenderGridBaseInside( SDL_Renderer* renderer );
	void RenderGridBaseInside( SDL_Renderer* renderer, const Coord& );
	void RenderGridBaseOutside( SDL_Renderer* renderer );
	void RenderGridBaseOutside( SDL_Renderer* renderer, const Coord& );
	void Render( SDL_Renderer* renderer ) override;
	void setBoxState( const Coord& pos, enBoxState set,bool outside);


	ObjectIntersectResult GridObjectIntersectionResult(GridObject* A) override;
	//ObjectIntersectResult ObjectBoxesIntersectWithItem(GridObjectItem* object ); what is this?
	ObjectIntersectResult ObjectBoxesIntersectWithShip(GridObject* object, enItemType objectItem);

	//bool defineGridObjectState( GridObject* object ) override;
	void SetGridBoxesToHide();
	void handleEvent(SDL_Event& event);

	void SetVesselOuterGridColor( const Coord& , const ColorRGB& );
	void SetVesselInnerGridColor( const Coord& , const ColorRGB& );
	void RefreshVesselOutsideBoxColor( const Coord& , const ColorRGB& );
	void SetShowVesselOutside(bool set);
	bool GetShowVesselOutside() { return pShowVesselOutside; }
	void SwitchShowVesselOutside();

	//bool addGridObject( std::shared_ptr<GridObject> , Coord position ) override;
	//bool addGridObject( std::shared_ptr<GridObject> )  override;
	bool AddGridObject( std::shared_ptr<GridObject> ) override;

	void RefreshOuterVesselBorders( const Coord& , const PixelBox& );
	void RefreshInnerVesselBorders( const Coord& , const PixelBox&, enObjectDiscreteRotation rotation, enObjectFlipped flip );
	void RefreshInnerVesselBox( const Coord& );

	//GridBoxSelectionResult SelectionResult;
	void SetModelVessel( std::weak_ptr<Vessel> vessel);
	Vessel* GetModelVessel() { return this->pModelVessel.lock().get(); };
	std::weak_ptr<PixelFigure> GetVesselFigure() { return pModelFigure; };

	void RefreshFigureBorders( const PixelFigure& figure, const Coord& relativePosition);
	void RefreshFigureBorders( const PixelFigure& figure, const Coord& relativePosition, enObjectFlipped flip, enObjectDiscreteRotation drotate );

	void SetBoxesPerSide( spriteSize boxesPerSide ) override;

	bool IsBoxDestroyed( const Coord & Coord);// { return ((ShipBuilderBox*) this->boxList[ Coord ].get())->IsBoxDestroyed(); };
	void SetBoxDestroyed( const Coord & Coord);// { ((ShipBuilderBox*) this->boxList[ Coord ].get())->SetBoxDestroyed(); this->boxList[ Coord ]->SetBoxFlag(true); };

	int GetHitPointFromHoveredGridObject();
	int GetHitPointFromChassis();
	int GetHitPointFromCurrentSelection();

	void SetShowHotKeys(bool set, enGridSelectType typeIfHotkeyDisabled);
	bool IsShowHotkeys() const;

	bool IsBuilderMode() const;
	bool IsFighterMode() const;

	void DrawVesselUsingGridObjects();

	void SetGridDisplayMode(enGridSelectType set) { this->pGridSelectionType = set; };
	enGridSelectType GetDisplayMode() { return this->pGridSelectionType; }

protected:

	void SetCurrentSelectedGridBox( const Coord& ) override;
	bool breachHighlighted;

	void RotateGBClockwise() override;
	void Flip() override;
	void RotateGBAntiClockwise() override;

	void handleGridObjectHoveredMouseMotion(SDL_Event& e, std::weak_ptr<GridObject>, const Coord&) override;
	void handleGridObjectHoveredMouseDown(SDL_Event& e, std::weak_ptr<GridObject> hoveredObj, const Coord& mousepos) override;
	//void Resize( spriteSize size, spriteSize boxesPerSide) override;
	bool AllowRenderGridObject(std::shared_ptr<GridObject>) override;

	bool isBoxFillable( const Coord& pos, enItemType itemtype ); //can place gridobject in box
	bool pShowVesselOutside; //make enum here?
	enGridSelectType pGridSelectionType;
	bool drawGridObjectValidity( GridObject* object );
	CoordMap<GridBox> pVesselOuterGridList; //use coordmapper
	std::set<Coord> pVesselOuterHiddenList;
	//std::unordered_set<Coord> pVesselOuterHiddenList;
	GridBox* getBox( const Coord& pos) override;
	//GridBox* GetExternalBox( const Coord& pos);
	bool CoordHasBox( const Coord& pos) override;
	void HighlightHiddenSet();
	void DisableHighlightHiddenSet();

	void ShowVesselOutsideGridObjects();
	void ShowVesselInsideGridObject();
	//void HandleGridObjectAdd( std::shared_ptr<GridObject> object );

	std::weak_ptr<Vessel> pModelVessel; //reference only, don't use delete
	std::weak_ptr<PixelFigure> pModelFigure;
	std::shared_ptr<ModuleEntity> GetEntityFromGridObject( std::shared_ptr<GridObject> );
	void ResetBoxState(GridBox* box) override;

	bool IsItemAllowedForGrid(enItemType type) override;
	//void ClearGridLists();
	//std::list<std::shared_ptr<GridObjectContainer>> GridObjectModels;

private:

	void handleEventHotkeys(SDL_Event& event);
	void handleEventFighter(SDL_Event& event);

	bool IsGridPositionChassis(const Coord& pos);
	void AddGridBoxesFromShipTemplate();


};
