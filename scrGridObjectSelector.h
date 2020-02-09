#pragma once
#include"scrList.h"
#include<string>


class GridObjectSelector  : public ListFrame
{
public:

	GridObjectSelector();
	GridObjectSelector( Coord dimensions, Coord );

	std::string GetSelectedGridObjectName();

protected:

	void HandleActiveWidget( SDL_Event& e, Widget* widget ) override;
	//void SetupListAndBars( int topScrollBarPos, int bottomScrollBarPos ) override;

	void Setup( Coord dimensions, Coord pos );
	void AddList(const Coord& pos) override;

private:

	void init();
	std::string SelectedGridObjectName;
};