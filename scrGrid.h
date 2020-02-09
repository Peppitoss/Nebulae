#pragma once
#include "Screen.h"
#include "Grid.h"

//class GameManager;

class GridFrame : public Screen 
{


public:

	GridFrame() {};
	GridFrame( spriteSize, Coord pos ); 

	virtual void render( SDL_Renderer& );
	virtual void HandleEvent( SDL_Event& e); //is this in use practically?

	Grid* GetGrid() { return pGrid.get(); };
	//GameManager* getParent() { return pParent; };
	virtual void HandleActiveWidget( SDL_Event& e, Widget* widget );

	//virtual ~GridFrame(void); //default

protected:

	std::unique_ptr<Grid> pGrid;
	//GameManager* pParent; //is global no delete
	void renderBackground( SDL_Renderer& );
	
	virtual void Setup(spriteSize size, spriteSize gridBoxPerSide);

private:

	
	GridFrame( const GridFrame& );
	GridFrame( GridFrame&& );
	GridFrame& operator=( const GridFrame& );
	GridFrame& operator=(GridFrame&& );

};

