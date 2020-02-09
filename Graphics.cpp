#include <sstream>
#include "Graphics.h"
//#include <SDL_image.h>
#include"Globals.h"
#include <cmath>
#include <random>
#include <sstream>
#include <algorithm>
#include"RandomNumberGenerator.h"
#include"StringBuilder.h"
#include<SDL_ttf.h>
#include"Wrapper.h"


namespace primitives {




	

	bool RectAEqualsToB(const SDL_Rect& A, const SDL_Rect& B)
	{
		if (A.x != B.x)
			return false;
		if (A.y != B.y)
			return false;
		if (A.w != B.w)
			return false;
		if (A.h != B.h)
			return false;
		return true;
	}



	double limitDouble( double value, double limit )
	{
		if( value > limit )
			value = limit;
		else if( value < -limit )
			value = -limit;
		return value;
	}

	//template <typename  T> bool RemoveItemFromVector(std::vector<T>& v, T item)
	//{
	//	typename std::vector<T>::iterator it = std::find(v.begin(), v.end(), item);
	//	if (it != v.end())
	//	{
	//		using std::swap;
	//		swap(*it, v.back());
	//		v.pop_back();
	//		return true;
	//	}
	//	return false;
	//}

	//template <typename  T> bool RemoveItemFromVector(std::vector<T>& v, int index)
	//{

	//	typename std::vector<T>::iterator it = v.at(v.begin() + index);
	//	if (it != v.end())
	//	{
	//		using std::swap;
	//		swap(*it, v.back());
	//		v.pop_back();
	//		return true;
	//	}
	//	return false;
	//}


	SDL_Rect vector::ToSDLRect(const vector& size) const
	{
		return ToSDLRect(size.x, size.y);
	}

	SDL_Rect vector::ToSDLRect(double w, double h) const
	{
		SDL_Rect result = { this->x, this->y, w, h };
		return result;
	}

		bool vector::operator<(const vector& other)  const
	{
		if( x < other.x )
		{
			return true;
		}
		else if( x == other.x )
			return y < other.y;
		else
			return false;
	}



	bool vector::operator>(const vector& other) const
	{
		if( x > other.x )
		{
			return true;
		}
		else if( x == other.x )
			return y > other.y;
		else
			return false;
	}

	bool vector::xORyLargerThan( double val )
	{
		return ( x>val || y>val );
	}

	bool vector::xORySmallerThan( double val )
	{
		return ( x<val || y<val );
	}

	bool vector::xANDyLargerThan( double val )
	{
		return ( x>val && y>val );
	}

	bool vector::xANDySmallerThan( double val )
	{
		return ( x<val && y<val );
	}

	void vector::xANDyLimit( double limit )
	{
		x = limitDouble(x,limit);
		y = limitDouble(y,limit);
	}

	double vector::ABS()
	{
		return sqrt( this->DotProduct() );
	}

	double vector::DotProduct()
	{
		return this->x*this->x + this->y*this->y;
	}

	vector vector::Normalize()
	{
		double length = ABS();
		this->x /= length;
		this->y /= length;
		return *this;
	}

	vector vector::Normalize(vector input )
	{
		vector result = input;
		double length = input.ABS();
		result.x /= length;
		result.y /= length;
		return result;
	}

	vector vector::ScaleByFactor(vector input, double factor)
	{
		vector result = input.ScaleByFactor(factor);
		return result;
	}
	
	vector vector::Scale( double length )
	{
		this->Normalize();
		this->x *= length;
		this->y *= length;
		return *this;
	}

	vector vector::ScaleByFactor( double factor )
	{
		this->x *= factor;
		this->y *= factor;
		return *this;
	}

	void vector::Rotate(double heading)
	{
		auto rotated = geometry::rotateVector(*this, heading);
		this->x = rotated.x;
		this->y = rotated.y;
			
	}

	bool waypoint::positionWithinWaypoint( Coord pos ) const
	{
		if( geometry::realDistance( pos, this->pos ) < this->circleRadius )
			return true;
		return false;
	}

	bool positionWithinWaypoint( primitives::waypoint wp, Coord pos )
	{
		if( geometry::realDistance( pos, wp.pos ) < wp.circleRadius )
			return true;
		return false;
	}


	void PIDSettings::Reset()
	{
		this->P = 0;
		this->I = 0;
		this->D = 0;
		this->Pheading = 0;
		this->Iheading = 0;
		this->Dheading = 0;
	}

	PIDSettings::PIDSettings()
	{
		this->Reset();
	}

	PIDSettings::PIDSettings(double P, double I, double D, double ph, double ih, double dh)
	{
		this->P = P;
		this->I = I;
		this->D = D;
		this->Pheading = ph;
		this->Iheading = ih;
		this->Dheading = dh;
	}

	PIDSettings::PIDSettings(double P, double I, double D)
	{
		this->P = P;
		this->I = I;
		this->D = D;
		this->Pheading = 0;
		this->Iheading = 0;
		this->Dheading = 0;
	}

	std::string GetLargestString(const std::vector<std::string>& allStrings)
	{
		std::string result = "";
		int width = 0;
		for (int i = 0; i < allStrings.size(); i++)
		{
			int currentWidth = GetTextDimensions(allStrings[i], 12, "Roboto-Regular.ttf").x;	//"Roboto-Regular.ttf" is standardLinkFont
			if (currentWidth > width)
			{
				result = allStrings[i];
				width = currentWidth;
			}
		}
		return result;
	}

	Coord GetTextDimensions(std::string text, int size, std::string font)
	{
		SDL_Rect dimension = sdlWrapper.GetTextSize(size, font, text, TTF_STYLE_BOLD);
		return Coord(dimension.w, dimension.h);
	}

	std::vector<std::string> SplitNewLine(const std::string &s, int size, std::string font, int maxWidth)
	{
		std::vector<std::string> result = std::vector<std::string>();
		if (s == "")
		{
			result.push_back("");
			return result;
		}

		int StartWidth = GetTextDimensions(s, size, font).x;
		if (StartWidth < std::max(StartWidth, maxWidth-10) )
		{
			result.push_back(s);
			return result;
		}

		std::string firstLine = "";
		std::string secondLine = "";

		std::vector<std::string> allStrings = Split(s, ' ');
		int accumulatedWidth = 0;
		for (int i = 0; i < allStrings.size(); i++)
		{
			accumulatedWidth += GetTextDimensions(allStrings[i], size, font).x;
			if (accumulatedWidth < std::max(0, maxWidth-10))
				firstLine += (allStrings[i] + " ");
			else
				secondLine += (allStrings[i] + " ");
		}

		//if (accumulatedWidth == 0)
		//	return result;

		result.push_back(firstLine);
		if (secondLine == s)	//this should not occur, but is added to prevent infinite loop
		{
			result.push_back(secondLine);
			return result;
		}
			

		std::vector<std::string> newSplit = SplitNewLine(secondLine, size, font, maxWidth);
		result.insert(result.end(), newSplit.begin(), newSplit.end());
		return result;
	}

	std::vector<std::string> Split(const std::string &s, char delim)
	{
		std::vector<std::string> result = std::vector<std::string>();
		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (getline(ss, item, delim))
		{
			result.push_back(item);
		}
		return result;
	}

	std::vector<std::string> SplitNewLine(const std::string &s)
	{
		std::vector<std::string> result = std::vector<std::string>();
		if (s == "")
		{
			result.push_back("");
			return result;
		}


		std::stringstream ss;
		ss.str(s);
		std::string item;
		while (getline(ss, item))
		{
			result.push_back(item);
		}
		return result;
	}

	void NormalDistribution( int average_x, int average_y, int standardDev_x, int standardDev_y, int size, std::list<Coord>& listOut, std::default_random_engine& engine )
	{
		std::normal_distribution<double> distribution_x(average_x,standardDev_x);
		std::normal_distribution<double> distribution_y(average_y,standardDev_y);

		int number_y;
		int number_x;
	
		for( int i = 0; i < size; i++ )
		{
			number_y = distribution_y(engine);
			number_x = distribution_x(engine);
			listOut.push_back( Coord(number_x,number_y) );
		}
	}

	void NormalDistribution( double average_x, double average_y, double standardDev_x, double standardDev_y, int size, std::list<primitives::vector>& listOut, std::default_random_engine& engine )
	{
		std::normal_distribution<double> distribution_x(average_x,standardDev_x);
		std::normal_distribution<double> distribution_y(average_y,standardDev_y);

		double number_y;
		double number_x;
	
		for( int i = 0; i < size; i++ )
		{
			number_y = distribution_y(engine);
			number_x = distribution_x(engine);
			listOut.push_back( primitives::vector(number_x,number_y) );
		}
	}

	void NormalDistribution( double average_x, double average_y, double standardDev_x, double standardDev_y, primitives::vector& Out, std::default_random_engine& engine )
	{
		std::normal_distribution<double> distribution_x(average_x,standardDev_x);
		std::normal_distribution<double> distribution_y(average_y,standardDev_y);

		Out.y = distribution_y(engine);
		Out.x = distribution_x(engine);
	}

	void NormalDistribution( int average, int standardDeviation, int size, std::list<int>& listOut, std::default_random_engine& engine )
	{
		std::normal_distribution<double> distribution(average,standardDeviation);

		int number;
	
		for( int i = 0; i < size; i++ )
		{
			number = distribution(engine);
			listOut.push_back( number );
		}
	}
	


	SDL_Rect SDLRect(int x, int y, int w, int h)
	{
		SDL_Rect result;
		result.x = x;
		result.y = y;
		result.w = w;
		result.h = h;
		return result;
	}

	TargetState::TargetState()
	{
		this->Position = Coord(0, 0);
		this->Velocity = primitives::vector(0, 0);
		this->Heading = 0;
	}

	TargetState::TargetState(Coord pos, primitives::vector vel, double heading)
	{
		this->Position = pos;
		this->Velocity = vel;
		this->Heading = heading;
	}

	TargetState::TargetState(const TargetState& other)
	{
		this->Position = other.Position;
		this->Velocity = other.Velocity;
		this->Heading = other.Heading;
	}

	TargetState::TargetState(const TargetState && other)
	{
		this->Position = std::move(other.Position);
		this->Velocity = std::move(other.Velocity);
		this->Heading = std::move(other.Heading);
	}

	


	
	//Circle::Circle()
	//{
	//	this->center = Coord(0,0);
	//	this->pubRadius = 1;
	//}
	//Circle::Circle(Coord center, double radius )
	//{
	//	this->center = center;
	//	this->pubRadius = radius;
	//}

			
}

namespace geometry {



	bool pointWithinBoundary_inclusive( int point, int lowLimit, int highLimit )
	{
		return( point >= lowLimit && point <= highLimit );
	}

	bool pointWithinBoundary_exclusive( int point, int lowLimit, int highLimit )
	{
		return( point > lowLimit && point < highLimit );
	}

	bool pointWithinSquare_inclusive( const Coord& point, const Coord& squareTopLeft, int squareThickness )
	{
		return pointWithinRectangle_inclusive( point, squareTopLeft, squareThickness, squareThickness );
	}

	bool pointWithinSquare_exclusive( const Coord& point, const Coord& squareTopLeft, int squareThickness )
	{
		return pointWithinRectangle_exclusive( point, squareTopLeft, squareThickness, squareThickness );
	}

	bool pointWithinRectangle_inclusive( const Coord& point, const Coord& squareTopLeft, const Coord& squareBottomright)
	{
		return pointWithinRectangle_inclusive(point, squareTopLeft, squareBottomright.x - squareTopLeft.x + 1, squareBottomright.y - squareTopLeft.y );
	}

	bool pointWithinRectangle_inclusive( const Coord& point, const Coord& squareTopLeft, int x, int y )
	{
		if (y < 0)
			return pointWithinRectangle_inclusive(point.yflip(), Coord(squareTopLeft.x, -squareTopLeft.y), x, -y);

		if( x < 0 ) 
			return pointWithinRectangle_inclusive(point.xflip(), Coord(-squareTopLeft.x, squareTopLeft.y), -x, y);

		if( pointWithinBoundary_inclusive( point.x, squareTopLeft.x, squareTopLeft.x+x-1 ) )
			if( pointWithinBoundary_inclusive( point.y, squareTopLeft.y, squareTopLeft.y+y-1 ) )
				return true;
		return false;
	}

	bool pointWithinRectangle_exclusive( const Coord& point, const Coord& squareTopLeft, int x, int y )
	{
		if( x == 0 || y == 0 ) 
		{
			throw std::runtime_error("error, thickness of rectangle is zero!");
			return false;
		}

		if( pointWithinBoundary_exclusive( point.x, squareTopLeft.x, squareTopLeft.x+x-1 ) )
			if( pointWithinBoundary_exclusive( point.y, squareTopLeft.y, squareTopLeft.y+y-1 ) )
				return true;
		return false;
	}

	bool BoxIntersectRectangle_inclusive( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopLeft, int rect_x, int rect_y ) //this one works properly
	{
		if( box_x == 0 || box_y == 0 || rect_x == 0 || rect_y == 0 ) 
		{
			throw std::runtime_error("error, thickness of rectangle or box is zero!");
			return false;
		}

		bool xIntersect = false;

		if( boxTopRight.x < squareTopLeft.x )
		{
			if( pointWithinBoundary_inclusive(squareTopLeft.x, boxTopRight.x, boxTopRight.x+box_x-1 ) )
				xIntersect = true;
		}
		else
		{
			if( pointWithinBoundary_inclusive( boxTopRight.x, squareTopLeft.x, squareTopLeft.x+rect_x-1 ) )
				xIntersect = true;
		}

		if(xIntersect == false)
			return false;

		if( boxTopRight.y < squareTopLeft.y )
		{
			if( pointWithinBoundary_inclusive(squareTopLeft.y, boxTopRight.y, boxTopRight.y+box_y-1 ) )
				return true;
		}
		else
		{
			if( pointWithinBoundary_inclusive( boxTopRight.y, squareTopLeft.y, squareTopLeft.y+rect_y-1 ) )
				return true;
		}
		return false;
	}

	int Sign( double val)
	{
		if( val < 0 )
			return -1;
		return 1;
	}

	SDL_Rect GetRectangleIntersect( const Coord& A, int A_x, int A_y, const Coord& B, int B_x, int B_y ) //gets the overlapping recangle intersection when rectangle A and B overlap
	{

		SDL_Rect result;
		result.h = 0;
		result.w = 0;
		
		if( !BoxIntersectRectangle_inclusive( A, A_x, A_y, B, B_x, B_y ) )
			return result;

		Coord relativePosition = B - A;
		Coord squareTopRight;
		if( relativePosition.x >= 0 )
		{
			result.x = relativePosition.x;
			result.w = A_x - relativePosition.x;
		}
		else
		{
			result.x = 0;
			result.w = A_x - relativePosition.x;
		}

		if( relativePosition.y >= 0 )
		{
			result.y = relativePosition.y;
			result.h = A_y - relativePosition.y;
		}
		else
		{
			result.y = 0;
			result.h = A_y - relativePosition.y;
		}
		return result;

	}

	bool BoxWithinRectangle_inclusive( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopRight, int rect_x, int rect_y ) //not tested
	{
		if( box_x == 0 || box_y == 0 || rect_x == 0 || rect_y == 0 )  
		{
			throw std::runtime_error("error, thickness of rectangle or box is zero!");
			return false;
		}
		//not tested - but looks to be correct
		if( pointWithinBoundary_inclusive( boxTopRight.x+box_x-1, squareTopRight.x, squareTopRight.x+rect_x-1 ) && pointWithinBoundary_inclusive( boxTopRight.x, squareTopRight.x, squareTopRight.x+rect_x-1 ) )
			if( pointWithinBoundary_inclusive( boxTopRight.y + box_y -1, squareTopRight.y, squareTopRight.y+rect_y-1 )  && pointWithinBoundary_inclusive( boxTopRight.y, squareTopRight.y, squareTopRight.y+rect_y-1 ))
				return true;
		return false;
	}

	bool BoxIntersectSquare_inclusive( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopRight, int squareThickness )
	{
		return BoxIntersectRectangle_inclusive( boxTopRight, box_x, box_y, squareTopRight, squareThickness,squareThickness );
	}

	bool BoxWithinSquare_inclusive( const Coord& boxTopRight, int box_x, int box_y, const Coord& squareTopRight, int squareThickness )
	{
		return BoxWithinRectangle_inclusive( boxTopRight, box_x, box_y, squareTopRight, squareThickness,squareThickness );
	}

	double angleBetweenTwoPoints( const Coord& A, const Coord& B) //range between [0, 2pi]
	{
		Coord relativePosition = B-A;
		//relativePosition.y *= -1 ;

		double result = atan2(relativePosition.y, relativePosition.x );

		if( result < 0 )
			result += 2*PI;

		return result;
	}

	

	double lineFunction( const Coord& intersection, double rate, double x ) //not tested
	{
		return intersection.y + rate*x;
	}

	Coord pointAlongLine( const Coord& linePoint, double angle, double distanceAlongLine )
	{
		primitives::vector lineVector = rotateVector(primitives::vector(1,0),angle);
		primitives::vector pointOnLine = primitives::vector(linePoint.x,linePoint.y) + lineVector*distanceAlongLine;
		Coord result = Coord(pointOnLine.x,pointOnLine.y);
		return result;
	}

	double rateFromAngle( double angle ) //not tested, ... aand what does this do? (line rate?)
	{
		if( angle == PI/2 || angle == 3*PI/2 )
			angle -= 0.01;

		if( ( angle > 0 && angle < PI/2 ) || angle < PI && angle < 3*PI/2 )
			return tan(angle);
		else
			return -tan(angle);
	}

	bool isInFirstQuadrant( double rad )
	{
		rad = radLimit2PI(abs(rad));
		return ( rad >= 0 && rad < PI_div2 );
	}
	bool isInSecondQuadrant( double rad )
	{
		rad = radLimit2PI(abs(rad));
		return ( rad >= PI_div2 && rad < PI );
	}
	bool isInThirdQuadrant( double rad )
	{
		rad = radLimit2PI(abs(rad));
		return ( rad >= PI && rad < 3*PI_div2 );
	}
	bool isInFourthQuadrant( double rad )
	{
		rad = radLimit2PI(abs(rad));
		return ( rad >= 3*PI_div2 );
	}

	double SetHeadingToQaudrant(double heading, int quadrant)
	{
		double rad = radLimit2PI(abs(heading));
		if (rad >= 0 && rad <= PI_div2)
		{
			if (quadrant == 1)
				return rad;
			else if (quadrant == 2)
			{
				return rad -= PI_div2;
			}
			else if (quadrant == 3)
			{
				return rad -= PI;
			}
			else if (quadrant == 4)
			{
				return rad -= ( PI + PI_div2 );
			}
		}

		if (rad >= PI_div2 && rad <= PI)
		{
			if (quadrant == 2)
				return rad;
			else if (quadrant == 1)
			{
				return rad += PI_div2;
			}
			else if (quadrant == 3)
			{
				return rad -= PI_div2;
			}
			else if (quadrant == 4)
			{
				return rad -= PI;
			}
		}

		if (rad >= PI && rad <= 3 * PI_div2)
		{
			if (quadrant == 3)
				return rad;
			else if (quadrant == 1)
			{
				return rad += PI;
			}
			else if (quadrant == 2)
			{
				return rad += PI_div2;
			}
			else if (quadrant == 4)
			{
				return rad -= PI_div2;
			}
		}

		if (rad >= 3 * PI_div2)
		{
			if (quadrant == 4)
				return rad;
			else if (quadrant == 1)
			{
				return rad += (PI + PI_div2);
			}
			else if (quadrant == 2)
			{
				return rad += PI;
			}
			else if (quadrant == 3)
			{
				return rad += PI_div2;
			}
		}
		throw new std::exception("error, heading doesn't fit in any quadrant");

	}

	primitives::vector OrthogonalVector( primitives::vector inVector )
	{
		return geometry::rotateVector(inVector,PI_div2);
	}

	double ProjectVectorOntoVector( primitives::vector inVector, primitives::vector projection ) // not a proper projection, but returns the amount of vector in that is projected. (I.e orthogoal vectors give 0 out )
	{
		return  inVector.ABS()*std::cos(geometry::shortestAngleDifference(geometry::vectorHeading(inVector),geometry::vectorHeading(projection)));
	}

	Coord pixelDistance( Coord A, Coord B)
	{
		return A-B;
	}

	double realDistance(Coord A)
	{
		return realDistance(Coord(0), A);
	}

	double realDistance( Coord A, Coord B)
	{
		return std::sqrt(realDistanceQuadratic(A,B));
	}

	double realDistanceQuadratic( Coord A, Coord B)
	{
		Coord pxDistance = pixelDistance(A,B);

		double result = pxDistance.x* pxDistance.x + pxDistance.y* pxDistance.y;
		return result;
	}

	bool PointWithinRange( Coord reference, Coord point, double range )
	{
		Coord pxDistance = pixelDistance(reference,point);
		double dA = pxDistance.x*pxDistance.x +pxDistance.y*pxDistance.y;

		if( dA <= range*range )
			return true;
		return false;
	}

	double realDistanceToYAxis( double X, Coord B) // bit wierd function..
	{
		return X-B.x;
	}

	bool pixelAreNeighbours( Coord A, Coord B )
	{
		Coord distance = pixelDistance(A,B);

		if( abs(distance.x) == 1 )
		{
			if( abs(distance.y) <= 1 )
				return true;
		}
		else if(  abs(distance.x) == 0 )
		{
			if( abs(distance.y) == 1 )
				return true;
		}
		return false;
	}

	Coord rotateCoord( Coord input, double heading )
	{
		Coord result;
		result.x = input.x*std::cos( heading ) - input.y*std::sin( heading );
		result.y = input.x*std::sin( heading ) + input.y*std::cos( heading );

		return result;
	}

	primitives::vector rotateVector( primitives::vector input, double heading )
	{
		primitives::vector result;
		result.x = input.x*std::cos( heading ) - input.y*std::sin( heading );
		result.y = input.x*std::sin( heading ) + input.y*std::cos( heading );

		return result;
	}

	double vectorHeading( primitives::vector A )
	{
		return atan2(A.y,A.x);
	}

	double vectorHeading( Coord A )
	{
		return vectorHeading( primitives::vector( A.x, A.y ) );
	}

	primitives::vector angleToVector( double angle )
	{
		return primitives::vector( cos(angle), sin(angle) );
	}

	double shortestAngleDifference( double TargetAngle, double SourceAngle )
	{
		/*double min1 = std::min(SourceAngle-TargetAngle, SourceAngle-TargetAngle+2*PI);
		return std::min(min1, SourceAngle-TargetAngle-2*PI);*/  
		//return atan2(sin(TargetAngle-SourceAngle), cos(TargetAngle-SourceAngle));
		double a = TargetAngle - SourceAngle;
		if( a > PI )
			a -= 2*PI;
		else if( a < -PI )
			a += 2*PI;
		return a;
		//double result = TargetAngle - SourceAngle;
		//if( result > PI/2 )
		//	return (result -= PI);
		//if( result < -PI/2 )
		//	return (result += PI);
		//return result;

	}

	double radToDegrees( double rad ) { return rad/PI * 180; };
	double DegreesToRad( double deg ) { return deg/180 * PI; };

	double angleBetweenVectors( primitives::vector A, primitives::vector B )
	{
		return std::acos( dotProduct(A,B) / (A.ABS()*B.ABS()) );
	}

	double angleBetweenVectors( Coord A, Coord B )
	{
		return angleBetweenVectors( primitives::vector( A.x, A.y), primitives::vector( B.x, B.y ) );
	}

	double dotProduct(const Coord& A)
	{
		return A.x*A.x + A.y*A.y;
	}

	double dotProduct( const primitives::vector& A, const primitives::vector& B )
	{
		return A.x*B.x + A.y*B.y;
	}
	// a rather simple (but understandable) method
	double radLimit2PI( double rad )
	{
		if( std::abs(rad) < 2*PI )
			return rad;

		if( rad > 2*PI )
		{
			double rest = rad/(2*PI);
			rad -= std::floor( abs(rest) )*2*PI;
			double restOfRest = rest - std::floor( abs(rest) );
			return 2*PI*restOfRest;
		}
		else if(rad < -2*PI)
		{
			double rest = rad/(2*PI);
			rad += std::floor( abs(rest) )*2*PI;
			double restOfRest = rest + std::floor( abs(rest) );
			return 2*PI*restOfRest;
		}

		return rad;
	}

	double radLimitPI(double rad)
	{
		if (std::abs(rad) < PI)
			return rad;

		if (rad >PI)
		{
			double rest = rad / (PI);
			rad -= std::floor(abs(rest)) * PI;
			double restOfRest = rest - std::floor(abs(rest));
			return PI*restOfRest;
		}
		else if (rad < -PI)
		{
			double rest = rad / (PI);
			rad += std::floor(abs(rest)) * PI;
			double restOfRest = rest + std::floor(abs(rest));
			return PI*restOfRest;
		}

		return rad;
	}

	Coord GetPointFromCircle( const Coord& center, double distance, double angle )
	{
		return Coord( center.x + distance*cos(angle), center.y + distance*sin(angle) );
	}

	bool CoordWIthinRectangle(const SDL_Rect& rect, const Coord& pos)
	{
		return geometry::pointWithinRectangle_inclusive(pos, Coord(rect.x, rect.y), rect.w, rect.h);
	}


//
//std::list<Coord> bresenham( Coord startPoint, Coord endPoint, std::list<Coord> points ) //returns a list of point intersections made from the bresenham line and list of pixels coords
//{
//	int x1 = startPoint.x;
//	int y1 = startPoint.y;
//	int x2 = endPoint.x;
//	int y2 = endPoint.y;
//	std::list<Coord> result = std::list<Coord>();
//	std::list<Coord>::iterator it;
//
//    int delta_x(x2 - x1);
//    // if x1 == x2, then it does not matter what we set here
//    signed char const ix((delta_x > 0) - (delta_x < 0));
//    delta_x = std::abs(delta_x) << 1;
// 
//    int delta_y(y2 - y1);
//    // if y1 == y2, then it does not matter what we set here
//    signed char const iy((delta_y > 0) - (delta_y < 0));
//    delta_y = std::abs(delta_y) << 1;
// 
//	it = find (points.begin(), points.end(), Coord(x1,y1));
//    if( it != points.end() )
//		result.push_back( Coord(x1,y1) );
// 
//    if (delta_x >= delta_y)
//    {
//        // error may go below zero
//        int error(delta_y - (delta_x >> 1));
// 
//        while (x1 != x2)
//        {
//            if ((error >= 0) && (error || (ix > 0)))
//            {
//                error -= delta_x;
//                y1 += iy;
//            }
//            // else do nothing
// 
//            error += delta_y;
//            x1 += ix;
// 
//			it = find (points.begin(), points.end(), Coord(x1,y1));
//			 if( it != points.end() )
//				result.push_back( Coord(x1,y1) );
//		}
//    }
//    else
//    {
//        // error may go below zero
//        int error(delta_x - (delta_y >> 1));
// 
//        while (y1 != y2)
//        {
//            if ((error >= 0) && (error || (iy > 0)))
//            {
//                error -= delta_y;
//                x1 += ix;
//            }
//            // else do nothing
// 
//            error += delta_x;
//            y1 += iy;
// 
//			it = find (points.begin(), points.end(), Coord(x1,y1));
//			 if( it != points.end() )
//				result.push_back( Coord(x1,y1) );
//		}
//    }
//	return result;
//}

}

namespace optimization
{
	
	double DIV( double numerator, int value )
	{
		static double divisor[50] = {1, 0.5,0.3333, 0.25, 0.2, 0.1667, 0.1429, 0.125, 0.1111, 0.1, 0.0909, 0.0833, 0.0769, 0.0714, 0.0667, 0.0625, 0.0588, 0.0556, 0.0526, 0.05,
									 0.0476, 0.0455, 0.0435, 0.0417, 0.04, 0.0385, 0.037, 0.0357, 0.0345, 0.0333, 0.0323, 0.0313, 0.0303, 0.0294, 0.0286, 0.0278, 0.027, 0.0263, 0.0256,
									 0.025, 0.0244, 0.0238, 0.0233, 0.0227, 0.0222, 0.0217, 0.0213, 0.0208, 0.0204, 0.02};
		if( value > 0 && value <= 50 )
		{
			return numerator*divisor[value];
		}
		return numerator/value;
	}
}