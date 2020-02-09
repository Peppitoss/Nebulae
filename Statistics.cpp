#include"Statistics.h"


namespace statistics
{

	Coord getRandomCoordInCircle( const Shapes::Circle& circ, const std::uniform_int_distribution<int>& distribution )
	{
		int angle = rand()%(2*3141);
		double dAngle = (double)angle/1000;
		double distance = std::fmax(circ.GetRadius(),(double)(distribution( RANDGENERATOR() )));

		Coord result = geometry::GetPointFromCircle( circ.GetCenterPosition(), distance, dAngle );
		return result;
	}

	Coord getRandomCoordInCircle( const Shapes::Circle& circ, const std::exponential_distribution<double>& distribution )
	{
		int angle = rand()%(2*3141);
		double dAngle = (double)angle/1000;
		double distance = std::fmax(circ.GetRadius(),distribution( RANDGENERATOR() ));

		Coord result = geometry::GetPointFromCircle( circ.GetCenterPosition(), distance, dAngle );
		return result;
	}

	Coord getRandomCoordInCircle( const Shapes::Circle& circ, std::normal_distribution<double>& distribution )
	{
		int angle = rand()%(2*3141);
		double dAngle = (double)angle/1000;
		double distance = std::fmin(circ.GetRadius(),distribution( RANDGENERATOR() ));

		Coord result = geometry::GetPointFromCircle( circ.GetCenterPosition(), distance, dAngle );
		return result;
	}

	int Roll_100()
	{
		/*std::uniform_int_distribution<int> dist_100(0, 100);
		return dist_100(rengine);*/
		return GetIntInRange(0, 100);
	}

	int GetIntInRange(int min, int max)
	{
		std::uniform_int_distribution<int> dis(min, max);
		return dis(RANDGENERATOR());
	}

	//double GetDoubleInRange(double min, double max)
	//{
	//	std::uniform_int_distribution<double> dis(min, max);
	//	return dis(RANDGENERATOR());
	//}

	int GetIntInRange(int min, int max, bool negativeIntsPossible)
	{
		if (negativeIntsPossible == false)
			return GetIntInRange(min, max);
		else
		{
			int result = GetIntInRange(min, max);
			if (rand() % 2)
				result *= -1;
			return result;
		}
	}

	std::vector<int> GetIntInRangeList(int min, int max, int amount)
	{
		std::vector<int> result = std::vector<int>();
		result.reserve(amount);
		for (int i = 0; i < amount; i++)
			result.push_back(GetIntInRange(min, max));
		return result;
	}

	primitives::vector GetRandomDirection()
	{
		int angle = GetIntInRange(0, 360);
		return geometry::angleToVector(angle);
	}

	ColorRGB GetRandomColorFromEnum()
	{
		int enumvalue = GetIntInRange(0, 8);
		return colorFromEnum( EnumColor(enumvalue) );
	}


}