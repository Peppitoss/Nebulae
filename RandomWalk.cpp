#include"RandomWalk.h"


RandomWalkGenerator::RandomWalkGenerator()
{
	Iterators = std::vector<RandomWalk>();
}

std::vector<Coord> RandomWalkGenerator::GenerateWalks()
{
	std::vector<Coord> result = std::vector<Coord>();
	for (int i = 0; i < Iterators.size(); i++)
	{
		auto walk = GenerateWalk(Iterators[i]);
		result.insert(result.end(), walk.begin(), walk.end());
	}
	return result;
}

std::vector<Coord> RandomWalkGenerator::GenerateWalk(const RandomWalk& walk)
{
	std::vector<Coord> result = std::vector<Coord>();

	int size = 999;

	CoordMap<bool> walkData = CoordMap<bool>(size);
	
	walkData.SetBoxFilled(walk.StartCoordinate);
	Coord currentStep = walk.StartCoordinate;

	while (walkData.GetInsertedBoxes() < walk.UniqueCoordinates)
	{
		Coord nextStep = currentStep + RandomDirection(1);
		if (walk.StepIsValid(nextStep))
		{
			walkData.SetBoxFilled(nextStep);
			result.push_back(nextStep);
			currentStep = nextStep;
		}
	}
	
	Coord smallestValue = Coord(size+1);
	for (int i = 0; i < result.size(); i++)
	{
		if (result[i].x < smallestValue.x)
			smallestValue.x = result[i].x;

		if (result[i].y < smallestValue.y)
			smallestValue.y = result[i].y;
	}

	for (int i = 0; i < result.size(); i++)
	{
		result[i] -= smallestValue;
	}

	return result;

}


RandomWalk::RandomWalk() : RandomWalk(Coord(445))
{

}

RandomWalk::RandomWalk(const Coord& startCoordinate)
{
	MinMaxYCoordinates = Coord(-999, 999); //included
	MinMaxXCoordinates = Coord(-999, 999); //included

	Compactness = 0.5;							//not included
	ProbabilityForYDirection = 0.5;				//not included
	NeighbourFactor = 0.5;						//not included
	
	StartCoordinate = startCoordinate;			//included
	FixedCoordinates = std::vector<Coord>();	//not included
	EmptyCoordinates = std::vector<Coord>();	//included
	UniqueCoordinates = 10;						//included
}

bool RandomWalk::StepIsValid(const Coord& pos) const
{
	if (pos.x <= MinMaxXCoordinates.x && pos.x >= MinMaxXCoordinates.y)
		return false;

	if (pos.y <= MinMaxYCoordinates.x && pos.y >= MinMaxYCoordinates.y)
		return false;

	if (std::find(EmptyCoordinates.begin(), EmptyCoordinates.end(), pos) != EmptyCoordinates.end())
		return false;

	return true;
	
}