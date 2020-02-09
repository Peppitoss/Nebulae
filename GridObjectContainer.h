#pragma once
#include"GridObject.h"

enum enGOContainerType
{
	Pure = 0, //only a grid object
	Module = 1,
	Item = 2
};

class GridObjectContainer
{
public:

	GridObjectContainer();
	GridObjectContainer( std::shared_ptr<GridObject> object );
	GridObjectContainer( GridObjectContainer& );
	GridObjectContainer( GridObjectContainer&& );
	
	GridObjectContainer& operator=(const GridObjectContainer& other);
	GridObjectContainer& operator=(const GridObjectContainer&& other);

	virtual void AddGridObject( std::shared_ptr<GridObject> object );
	std::shared_ptr<GridObject> GetGridObject();

	enGOContainerType GetType() { return pType; };

protected:

	enGOContainerType pType;
	std::shared_ptr<GridObject> pGObject;
	//int ID;

	void Initialize();

};