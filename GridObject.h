#pragma once
#include"ShipBuilderBox.h"
#include"GridObjectCollection.h"
#include"BoxList.h"
#include<Map>
#include"Link.h"
#include"PixelFigure.h"

static const spriteSize STANDARDOBJECTSIZE = spriteSize(8);

//using enObjectDiscreteRotation;
//using enObjectFlipped;

enum enGridObjectState
{
	goNormal = 1,
	goHide = 2,
	goNoSelect = 3
};

//enum enGridObjectType
//{
//	typeInternal = 1,
//	typeExternal = 2
//};


class ObjectIntersectResult;
class GridObject;

class ITransferObject
{
public:

	virtual std::shared_ptr<GridObject> getGridObject() = 0;
	//virtual Coord getRelativePosition() const = 0;

};

class GridObject : public ITransferObject
{

public:

	GridObject();
	GridObject( const BoxList& gridPos ); 
	GridObject( const BoxList& list, spriteSize ,const imagehandling::IMGToSurface*  );
	GridObject( CoordMap<GriddedBox>&, spriteSize ,const imagehandling::IMGToSurface* );
	//GridObject( const BoxList& gridPos, const GridObjectTexturePack& );
	GridObject( const imagehandling::IMGToSurface* );

	bool insertBoxInPosition( Coord pos, GridBox* );
	virtual void render( Coord pos, SDL_Renderer* renderer, int scaleFactor) const;

	Coord getRelativePosition() const { return this->pRelativePosition; };
	//Coord& getRelativePositionREF() { return this->pRelativePosition; }; //only use in caution!
	void setRelativePosition( Coord set);
	void setProperPosition( Coord set);// { setRelativePosition(set); setLastPlacedPosition(set); };

	Coord getLastPlacedPosition() { return pLastPlacedPosition; };
	void setLastPlacedPosition( Coord set) { this->pLastPlacedPosition = set; };

	std::shared_ptr<GridObject> getGridObject() { return std::shared_ptr<GridObject>(this); }

	bool isPositionInObject( Coord pos ) { return mapHasKey(pos-GridOrigo); };
	int boxSize() const { return this->pBoxSize.GetSize(); };

	Coord GetPositionInGridReferenceFrame() const;// { return this->getRelativePosition() + GridOrigo - ObjectOrigo; };
	void RunHotKeyEvent(SDL_Event& e);
	bool HotKeyIsSelected();
	string GetSelectedHotKey();
	void SetHotKeyForDisplay(std::string);

	Coord GridOrigo;
	Coord ObjectOrigo;

	int getWidth() const { return pObjectWidth; };
	int getHeight() const { return pObjectHeight; };

	virtual ObjectIntersectResult IntersectGridObject( GridObject* B );
	bool GridObjectIntersect(GridObject* B);
	bool GridObjectIntersect(const SDL_Rect& intersection);
	SDL_Rect GridObjectIntersection(GridObject* B);

	virtual void refreshBoxesState();
	void setBoxesInvalid();
	void setBoxesHighlighted();
	void SelectHotKeyLink(bool value);

	void setToolTipText( std::string set );
	bool hasTooltipLink() const { return pTooltip.link != nullptr; };

	bool hasHotKeyLink() const { return this->pHotKeyButton.link != nullptr; };
	bool showHotKeyLink() const;
	void GenerateHotKeyButton(char hotkey);

	void setStateHovered();// { isHoveredByMouse = true; };
	void disableHoveredState();// { isHoveredByMouse = false; };

	void setPlacementOrigoDependent( bool set ){ placementOrigoDependent = set; };
	bool isPlacementOrigoDependent() { return placementOrigoDependent; };

	void addGridObjectToBundle( GridObject* add );
	GridObject* PullGridObjectFromBundle();
	std::list<GridObject*> PullAllGridObjectFromBundle();

	bool isActive() const { return pActive; };
	void SetActive( bool set );

	bool hasBundle() const { return pBundled; };
	void setBundled( bool set ) { pBundled = set; };
	void resetBundleTextPosition( int scalingFactor );

	bool mapHasKey( Coord );
	int GetSortOrder() const { return SortOrder; };

	void setTempStackedGridObject( GridObject* set ) { tempStackedGridObject = set; };
	bool hasAwaitingStackObject() { return tempStackedGridObject != NULL; };
	void MakeBundleWithStackedObject();
	void nullifyTempStackedGridObject() {tempStackedGridObject = NULL; };

	Coord TransformCoordToListBoxReference( const Coord& );

	void SetBoxRed( const Coord& pos );
	void SetBoxRed( const BoxList& box );
	void SetBoxHvite( const Coord& pos );
	void SetBoxBlackLevel( const Coord& pos, double percent );
	void SetBoxTransparent( const Coord& pos );
	void GridObjectHvite();
	
	bool IsContained() { return this->pIsContainedFlag; };
	void SetContained( bool set ) { this->pIsContainedFlag = set; };

	string GetIDName() { return this->pIDName; };
	void SetIDName(std::string set) { pIDName = set; };
	int GridBoxAmount() const;

	//int GetID() { return this->ID; };
	//void SetID( int set ) { ID = set; };

	enObjectFlipped GetFlipped() const { return this->pFlipped; };;
	enObjectDiscreteRotation GetDiscreteRotation() const { return this->pRotation; };

	bool CanRotate() const;
	void SetCanRotate( bool TrueIsRotate );
	void Rotate90Degrees( bool clockwise );
	void FlipAlongX();
	void Transpose();
	
	enGridObjectState GetState();
	void SetState( enGridObjectState set ) { this->pState = set; };

	//enGridObjectType GetGridObjectType();
	//void SetGridObjectType( enGridObjectType set );
	void SetFlipped(enObjectFlipped flipped);
	void SetRotation(enObjectDiscreteRotation rotation);

	void SetNewGridOrigo( int gridSize );
	void SetNewGridOrigo(const Coord& origo);

	bool RenderHotKeys() const { return this->pRenderHotKeys; };
	void RenderHotKeys(bool set) { this->pRenderHotKeys = set; };
	
	std::map<Coord,std::unique_ptr<GridBox>>::const_iterator const begin() const { return pBoxes.begin(); }
    std::map<Coord,std::unique_ptr<GridBox>>::const_iterator const end() const { return pBoxes.end(); }
protected:

	enGridObjectState pState;
	//enGridObjectType pGridObjectType;
	enObjectFlipped pFlipped;
	enObjectDiscreteRotation pRotation;

	//void IncrementDiscreteRotation();
	//void DecrementDiscreteRotation();
	//void ToggleFlipped();

	int pObjectWidth;
	int pObjectHeight;

	int SortOrder;
	//int pScale;

	bool placementOrigoDependent;
	bool isHoveredByMouse;
	bool pBundled;
	bool pIsContainedFlag; //flag that determines whether the grid object is contained by another object
	std::string pDescription;
	spriteSize pBoxSize;
	
	Coord pRelativePosition;	//position when fixed on ground + position seen while selected with mouse
	Coord pLastPlacedPosition;	//position when fixed on ground 
	std::map<Coord,std::unique_ptr<GridBox>> pBoxes;
	std::list<GridObject*> pGridObjectBundle;  //used when gridobjects are stacked onto each other
	
	GridObject* tempStackedGridObject; //a helper variable : the object that This gridobject can become stacked to (to form bundle with)

	virtual SDL_Texture* GetTextureFromBox( GridBox* box, SDL_Renderer* ) const;

	LinkAttachment pTooltip;
	LinkAttachment pBundleCountText;
	LinkAttachment pHotKeyButton;

	//void addGridObjectToBundle( GridObject* add );
	//enumGridType pType;

	bool pActive;
	std::string pIDName;
	virtual void initialize();

private:

	bool pRenderHotKeys;
	int GetCenter_X();
	//Coord GetMaxCoordValues();
	bool pCanRotate;

};



class ObjectIntersectResult
{
public:
	bool invalid;
	bool bundled;
	GridObject* intersectObject; //not smart pointer for possible speed performance
	GridObject* roomObject;

	ObjectIntersectResult();
	ObjectIntersectResult(bool invalid,bool bundled, GridObject*, GridObject*);

};

bool compare_gridObjects(const GridObject* first, const GridObject* second);
bool compare_gridObjects_ptr(const std::shared_ptr<GridObject>& first, const std::shared_ptr<GridObject>& second);