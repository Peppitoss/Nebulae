#pragma once
#include "Graphics.h"
#include <Vector>

//Class is not in use

class ProceduralDesigner
{

public:

	enum enumIncrements
	{
		VERTICALDOWN = 0,
		VERTICALUP,
		HORIZONTALLEFT,
		HORIZONTALRIGHT,
		DASHLEFT,
		DASHRIGHT,
		DASHLEFTDOWN,
		DASHRIGHTDOWN,
	};

	ProceduralDesigner(ColorRGB color, int iterations, unsigned int resolution);
	ProceduralDesigner(CoordWithColor Coord, int iterations, unsigned int resolution);
	ProceduralDesigner( int iterations, unsigned int resolution);

    virtual ProceduralDesigner * clone () const = 0;  // Virtual constructor (copying) 
	//ProceduralDesigner& operator=(const ProceduralDesigner& other) { throw struntime_error("= operator on ProceduralDesigner not implemented"); };
	
	~ProceduralDesigner();
	
	virtual CoordWithColor iterate( bool branch ) = 0;
	virtual CoordWithColor iterate( enumIncrements enumincrements, bool branch ) = 0;

	int getIterations() { return pIterations; };

	std::vector<ProceduralDesigner*> branches;

	virtual void increment( enumIncrements,Coord &  ) = 0;

	unsigned int getResolution() { return pResolution; };
	void setResolution(	unsigned int );


protected:

	int pIterations;
	unsigned int pStep;
	CoordWithColor pCurrentCoord;
	unsigned int pResolution;

};

class ProceduralPlant : public ProceduralDesigner
{

public:

	ProceduralPlant( ColorRGB color, enumIncrements startDirection, int iterations, unsigned int resolution);
	ProceduralPlant( enumIncrements startDirection, int iterations, unsigned int resolution);
	ProceduralPlant( CoordWithColor Coord, enumIncrements startDirection, int iterations, unsigned int resolution );
	ProceduralPlant( int iterations, unsigned int resolution );

	ProceduralPlant(const ProceduralPlant & );

    ProceduralPlant * clone () const
    {
      return new ProceduralPlant(*this);
    }
	//ProceduralPlant& operator=(ProceduralPlant other) {throw std::runtime_error("Don't use use = operator on ProceduralPlant (code not implemented)");};

	virtual CoordWithColor iterate(bool branch);
	virtual CoordWithColor iterate(enumIncrements enumincrements,bool branch);
	virtual void increment( enumIncrements,Coord &  );

private:

	enumIncrements pStartDirection;
	

};