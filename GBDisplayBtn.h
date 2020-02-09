#pragma once
#include"SelectLink.h"
#include"GridObject.h"


class GBDisplayBtn
{
public:

	GBDisplayBtn( const char* text, TextSize, Coord position, int h, int w, std::shared_ptr<GridObject> );

private:

	SelectLink pBtn;
	std::shared_ptr<GridObject> pGridObjectDisplay;

};

