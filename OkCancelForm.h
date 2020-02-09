#pragma once
#include"Screen.h"

class OkCancelForm : public Screen
{
public:

	OkCancelForm();
	OkCancelForm( Coord dimensions, Coord pos);

protected:

		virtual void Setup( Coord dimensions, Coord pos );

		virtual void close_click(Widget*) = 0;
		virtual void ok_click( Widget*) = 0;

};