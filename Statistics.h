#pragma once
#include"Graphics.h"
#include"Shapes.h"
#include"RandomNumberGenerator.h"

namespace statistics
{
	int Roll_100();
	Coord getRandomCoordInCircle( const Shapes::Circle& center, const std::uniform_int_distribution<int>& distance );
	Coord getRandomCoordInCircle( const Shapes::Circle& center, const std::exponential_distribution<double>& distance );
	Coord getRandomCoordInCircle( const Shapes::Circle& center, std::normal_distribution<double>& distance );

	primitives::vector GetRandomDirection();


	int GetIntInRange(int min, int max);
	int GetIntInRange(int min, int max, bool negativeIntsPossible );
	std::vector<int> GetIntInRangeList(int min, int max, int amount);

	ColorRGB GetRandomColorFromEnum();


	
}


