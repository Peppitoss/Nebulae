#pragma once
#include "Wrapper.h"
struct Data
{
	SURFACE_TYPE _type;
	COLOUR _colour;
	SDL_Surface* _surface;

	Data(SDL_Surface s, COLOUR c, SURFACE_TYPE t) : _colour(c),_type(t),_surface(s)
	{ 

	}

	bool const operator==(const Data& sd) const
	{
		bool status = false;
		if (sd._surface->w == this->_surface->w && sd._surface->h == this->_surface->h && sd._colour == this->_colour && sd._type == this->_type)
		{
			status = true;
		}
		return status;
	}

	virtual ~Data(void)
	{
		SDL_FreeSurface(_surface);
	}
};

