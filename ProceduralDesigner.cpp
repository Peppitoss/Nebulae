#include"ProceduralDesigner.h"


ProceduralDesigner::ProceduralDesigner(ColorRGB color, int iterations, unsigned int resolution)
{
	pIterations = iterations;
	pStep = 0;
	pCurrentCoord.x = 0;
	pCurrentCoord.y = 0;
	pCurrentCoord.Object = color;

	if( resolution > 0 )
		pResolution = resolution;
	else
		pResolution = 1;
}

ProceduralDesigner::ProceduralDesigner(CoordWithColor Coord, int iterations, unsigned int resolution)
{
	pIterations = iterations;
	pStep = 0;
	pCurrentCoord = Coord;

	if( resolution > 0 )
		pResolution = resolution;
	else
		pResolution = 1;
}

ProceduralDesigner::ProceduralDesigner(int iterations, unsigned int resolution)
{
	pIterations = iterations;
	pStep = 0;
	pCurrentCoord.x = 0;
	pCurrentCoord.y = 0;
	pCurrentCoord.Object = ColorRGB();

	if( resolution > 0 )
		pResolution = resolution;
	else
		pResolution = 1;
}

void ProceduralDesigner::setResolution( unsigned int set )
{
	if( set > 0 )
		pResolution = set;
	else 
		pResolution = 1;
}

ProceduralDesigner::~ProceduralDesigner()
{
	branches.clear();
}



ProceduralPlant::ProceduralPlant(ColorRGB color,enumIncrements startDirection, int iterations, unsigned int resolution) : ProceduralDesigner(color,iterations,resolution) 
{
	pStartDirection = startDirection;
}
ProceduralPlant::ProceduralPlant(enumIncrements startDirection, int iterations, unsigned int resolution) : ProceduralDesigner(iterations,resolution) 
{
	pStartDirection = startDirection;
}
ProceduralPlant::ProceduralPlant(int iterations, unsigned int resolution ) : ProceduralDesigner(iterations,resolution)
{
	pStartDirection = VERTICALUP;
}

ProceduralPlant::ProceduralPlant( CoordWithColor Coord,enumIncrements startDirection, int iterations, unsigned int resolution ) : ProceduralDesigner(Coord,iterations,resolution) 
{
	pStartDirection = startDirection;
}

ProceduralPlant::ProceduralPlant(const ProceduralPlant & other) : ProceduralDesigner( other.pCurrentCoord, other.pIterations,other.pResolution  )
{
	pStartDirection = other.pStartDirection;
	pStep = other.pStep;
}

CoordWithColor ProceduralPlant::iterate( bool branch )
{
	return iterate( pStartDirection, branch );
}

CoordWithColor ProceduralPlant::iterate( enumIncrements enumincrements, bool branch )
{

	CoordWithColor result = this->pCurrentCoord;

	if( branches.size() > 0 )
	{
		int i = branches.size()-1;
		if( branches[i]->getIterations() >= 0 )
		{
			//if( enumincrements == HORIZONTALLEFT )
			//	result = branches[i]->iterate(VERTICALDOWN,true);
			//else if( enumincrements == HORIZONTALLEFT )
			//	result = branches[i]->iterate(DASHLEFT);
			//else
			//	result = branches[i]->iterate(DASHRIGHT);
			result = branches[i]->iterate(false);
		}
		else
		{
			branches.erase(branches.end()-1);
			return this->iterate(enumincrements,branch);
		}
	}
	else if( pIterations >= 0 )
	{
		pStep++;
		pIterations--;

		if( branch && (pStep % 40 ) == 19 )
		{
			CoordWithColor newbranch = this->pCurrentCoord;
			//increment( HORIZONTALLEFT, newbranch );
			newbranch.Object.newColor( MAGENTA );
			branches.push_back( new ProceduralPlant( newbranch, HORIZONTALLEFT, pIterations, pResolution ) );

			newbranch = this->pCurrentCoord;
			//increment( HORIZONTALRIGHT, newbranch );
			newbranch.Object.newColor( MAGENTA );
			branches.push_back( new ProceduralPlant( newbranch, HORIZONTALRIGHT, pIterations, pResolution ) );
		}
		
		increment( enumincrements, result );
		this->pCurrentCoord = result;
	}

	return result;

}


void ProceduralPlant::increment( enumIncrements enumincrements, Coord & result )
{

	switch( enumincrements ) 
	{
	case( VERTICALDOWN ) :
		result.y += 1*pResolution;
		break;
	case( VERTICALUP ) :
		result.y -= 1*pResolution;
		break;
	case( HORIZONTALLEFT ) :
		result.x -= 1*pResolution;
		break;
	case( HORIZONTALRIGHT ) :
		result.x += 1*pResolution;
		break;
	case( DASHLEFT ) :
		result.y -= 1*pResolution;
		result.x -= 1*pResolution;
		break;
	case( DASHRIGHT ) :
		result.y += 1*pResolution;
		result.x -= 1*pResolution;
		break;
	case( DASHLEFTDOWN ) :
		result.y -= 1*pResolution;
		result.x += 1*pResolution;
		break;
	case( DASHRIGHTDOWN ) :
		result.y += 1*pResolution;
		result.x += 1*pResolution;
		break;
	}

}