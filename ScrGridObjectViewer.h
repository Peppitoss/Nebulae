#pragma once
#include"scrGridObject.h"
#include"ModuleEntity.h"
#include"GridObjectModuleContainer.h"


class GridObjectViewerFrame : public GridObjectFrame
{

public:

	GridObjectViewerFrame();
	GridObjectViewerFrame( spriteSize, Coord pos ); 

	//virtual void render( SDL_Renderer& );

	//void SetViewedGridObject( std::shared_ptr<GridObject> viewedObject );
	bool AddGridObject( std::shared_ptr<GridObject> object ) override;
	bool AddGridObject( std::shared_ptr<GridObject> object, const Coord & pos ) override;
	void RemoveSelectedGridObject() override;
	void AddNewViewedGridObject( std::string globalListName );
	void AddNewViewedGridObject( ModuleEntity* );

	void SetRegenerateObjects( bool set );
	void SetAccepTransfers( bool set );

	bool AcceptTransfers() { return this->pAcceptTransfers; };
	bool RegenerateObjects() { return this->pRegenerateObjects; };

	std::shared_ptr<GridObjectContainer> GetGridObjectContainerFromFrame(); //obsolete??

protected:

	std::shared_ptr<ModuleEntity> currentShownEntity;
	std::string nameOfCurrentShownEntity;
	void Setup( spriteSize, Coord pos );

	//std::shared_ptr<GridObject> pCurrentViewedObject;

private:

	bool pAcceptTransfers;
	bool pRegenerateObjects;
	void init();
};
