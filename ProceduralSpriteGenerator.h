#pragma once
#include <SDL.h>
#include "ProceduralDesigner.h"
#include "PixelFigure.h"

//class not in use
class ProceduralSpriteGenerator
{

public:

	ProceduralSpriteGenerator( int x, int y, unsigned int resolution );
	ProceduralSpriteGenerator( const ProceduralDesigner* designer, int x, int y, unsigned int resolution );
	ProceduralSpriteGenerator(  const ProceduralSpriteGenerator &  );

	~ProceduralSpriteGenerator() { delete pDesigner; };

	friend void swap(ProceduralSpriteGenerator& first, ProceduralSpriteGenerator& second) //undo
    {
        using std::swap; 

		swap(first.pSizeX, second.pSizeX);
		swap(first.pSizeY, second.pSizeY); 
        swap(first.pResolution, second.pResolution); 
        swap(first.pDesigner, second.pDesigner);
		swap(first.pCenter, second.pCenter); 

    }

	Coord getCenter() { return pCenter; };

	ProceduralSpriteGenerator& operator=(ProceduralSpriteGenerator other);

	PixelFigure psrlGenerateFigure();

private:

	Coord pCenter;
	ProceduralDesigner* pDesigner;
	int pSizeX;
	int pSizeY;
	unsigned int pResolution;

	void addDesigner( const ProceduralDesigner* );

};



