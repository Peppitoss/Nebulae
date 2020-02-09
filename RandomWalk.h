#pragma once
#include<vector>
#include"Coord.h"
#include"CoordMap.h"

template class CoordMap<bool>;
template class CoordMapIterator<bool>;
template class CoordMapRow<bool>;
template class CoordMapItem<bool>;


class RandomWalk
{

public:

	RandomWalk();
	RandomWalk(const Coord& startCoordinate);


	Coord MinMaxYCoordinates;
	Coord MinMaxXCoordinates;

	//If 1 or larger, the next step will always be towards center of gravity
	//If 0 or less, next step will always be away from center of gravity
	double Compactness;

	//If 1, all directions point along axis Y, 
	//If 0, all directions point along axis X
	double ProbabilityForYDirection;

	//If 1 or larger, the next step will always point towards the direction that will grant the next node most neighbours
	//If 0 or less, next step will always point towards the direction that will grant the next node least neighbours
	double NeighbourFactor;

	Coord StartCoordinate;
	std::vector<Coord> FixedCoordinates;
	std::vector<Coord> EmptyCoordinates;
	int UniqueCoordinates;

	bool StepIsValid(const Coord&) const;

};

class RandomWalkGenerator
{

public:
	RandomWalkGenerator();

	std::vector<Coord> GenerateWalks();
	std::vector<RandomWalk> Iterators;

	static std::vector<Coord> GenerateWalk(const RandomWalk& walk);

private:

};