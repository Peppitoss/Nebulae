#include<iostream>
#include"ProceduralSpriteGenerator.h"

ProceduralSpriteGenerator::ProceduralSpriteGenerator(int x, int y, unsigned int resolution)
{
	if( resolution > 0 )
		pResolution = resolution;
	else 
		pResolution = 1;

	pDesigner = NULL;
	pSizeX = x;
	pSizeY = y;
	pCenter.x = pSizeX/2;
	pCenter.y = pSizeY/2;
}

ProceduralSpriteGenerator::ProceduralSpriteGenerator( const ProceduralDesigner* designer, int x, int y, unsigned int resolution )
{
	if( resolution > 0 )
		pResolution = resolution;
	else 
		pResolution = 1;
	addDesigner( designer );
	pSizeX = x;
	pSizeY = y;
	pCenter.x = pSizeX/2;
	pCenter.y = pSizeY/2;
}

ProceduralSpriteGenerator::ProceduralSpriteGenerator(  const ProceduralSpriteGenerator & other )
{
	pResolution = other.pResolution;
	delete pDesigner;
	addDesigner( other.pDesigner );
	pSizeX = other.pSizeX;
	pSizeY = other.pSizeY;
	pCenter.x = other.pCenter.x;
	pCenter.y = other.pCenter.y;
	

}

PixelFigure ProceduralSpriteGenerator::psrlGenerateFigure()
{

	if( pDesigner == NULL ) 
	{
		throw std::runtime_error( "pDesigner is zero in ProceduralSpriteGenerator!");
		return PixelFigure();
	}

	PixelFigure result(pSizeX,pSizeY,pResolution);

	while( pDesigner->getIterations() > 0 ) 
	{
		CoordWithColor dotPosition = pDesigner->iterate(true);
		try
		{
			result.addBox(dotPosition + pCenter );
		}
		catch( std::runtime_error &e )
		{
			std::cerr << e.what();
		}
	}
	result.UpdateNeighbourhood();
	return result;

}

ProceduralSpriteGenerator& ProceduralSpriteGenerator::operator=(ProceduralSpriteGenerator other)
{
	swap(*this,other);
	return *this;
}

void ProceduralSpriteGenerator::addDesigner( const ProceduralDesigner* designer )
{
	pDesigner = designer->clone();
	pDesigner->setResolution( pResolution );
}
