#pragma once
#include"PixelBox.h"
#include"Coord.h"
#include"GameAttributes.h"

class Neighbour
{

public:

	Neighbour();
	Neighbour( const Neighbour& );
	Neighbour( const Neighbour&& );

	Neighbour operator=(const Neighbour&);
	Neighbour operator=(Neighbour&&);

	Coord pos;
	EnumAdjacency properAdj;
	EnumAdjacency oppositeAdj;

	bool isPresent() { return *present; };

	bool* present; // no new, is used in a safe way, used as a reference (nullable reference)
	
	~Neighbour() {};
};

class GriddedBox
{

public:

	GriddedBox();
	GriddedBox(PixelBox setBox, Coord setCoord, bool setPlaced );

	GriddedBox( const GriddedBox & other );
	GriddedBox( const GriddedBox && other );

	GriddedBox& operator=(const GriddedBox& other);
	GriddedBox& operator=(const GriddedBox&& other);

	Neighbour getIteratorNeighbour(int i) const;
	Coord getIteratorNeighbourPos();

	PixelBox box;//std::unique_ptr<PixelBox> box;
	
	Coord Position() const { return this->pCoord; }
	double GetPercentageVitalHitPoints();

	game::Hull hull;
	//game::Hull initHull;

	void SetMaxHitPointValue( int value );

	bool isPlaced() const { return placed; };
	bool isEdgebox() const { return pEdgeBox; };

	void setPlaced(bool);
	void setEdgeBox(bool set) { pEdgeBox =  set; };

private:

	//int neighbourIterator;
	std::vector<Neighbour> pNeighbourIterator;
	bool placed; //Has the box been positioned in its proper place, or not? Is it currently a part of the ship?
	bool pEdgeBox; //is originally the edge of the figure
	Coord pCoord;
	
	GriddedBox& operator=( GriddedBox&& other);
};
