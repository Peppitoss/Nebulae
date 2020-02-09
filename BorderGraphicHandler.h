#pragma once
#include"Graphics.h"
#include"BorderData.h"



class BorderGraphicHandler
{

public:

	static SDL_Rect BorderGraphicHandler::GenerateCropRect( const BorderData& data, int cropSize );

private :

	static Coord GetSurfaceDiagonalCutPosition(const BorderData&);
	static Coord ThreeDiagonalCornersGetCoord(const BorderData&);
	static Coord TwoDiagonalCornersGetCoord(const BorderData&);
	static Coord OneDiagonalCornersGetCoord(const BorderData&);


	static Coord GetSurfaceCutPosition( const BorderData& );
	static Coord ThreeCornersGetCoord( const BorderData& );
	static Coord TwoCornersGetCoord( const BorderData& );
	static Coord OneCornersGetCoord( const BorderData& );


};