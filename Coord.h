#pragma once
#include <SDL.h>
#include <memory>
#include<string>
#include<random>
#include"DiscreteRotations.h"
#include"CollisionData.h"
#include"ColorRGB.h"

enum EnumAdjacency
{
	ZERO = 0,	
	UP = 1,
	DOWN = 2,
	LEFT = 4,
	RIGHT = 8,
	UPLEFT = 16,
	UPRIGHT = 32,
	DOWNLEFT = 64,
	DOWNRIGHT = 128
};

inline EnumAdjacency operator|(EnumAdjacency a, EnumAdjacency b)
{
	return static_cast<EnumAdjacency>(static_cast<int>(a) | static_cast<int>(b));
}

class Coord
{

public:
	Coord() { x = 0; y = 0; };

	Coord(const Coord & other) { x = other.x, y = other.y; };
	Coord(const Coord && other) { x = std::move(other.x), y = std::move(other.y); };
	Coord(const int X, const int Y) { x = X, y = Y; };
	Coord(const int XY) { x = XY, y = XY; };

	Coord operator+(const Coord& b) { return Coord(this->x + b.x, this->y + b.y); };
	Coord & Coord::operator+=(const Coord &rhs) { this->x += rhs.x; this->y += rhs.y;  return *this; };

	Coord operator-(const Coord& b) { return Coord(this->x - b.x, this->y - b.y); };
	Coord & Coord::operator-=(const Coord &rhs) { this->x -= rhs.x; this->y -= rhs.y;  return *this; };

	Coord Coord::operator*(const double &rhs) { return Coord(this->x * rhs, this->y * rhs); };
	Coord & Coord::operator*=(const double &rhs) { this->x *= rhs; this->y *= rhs; return *this; };

	Coord& operator=(const Coord& other) { this->x = other.x; this->y = other.y; return *this; };
	Coord& operator=(const Coord&& other);

	Coord Coord::operator/(const double &rhs) { return Coord(this->x / rhs, this->y / rhs); };
	Coord Coord::operator/=(const int &rhs) { this->x /= rhs; this->y /= rhs;  return *this; };

	bool Coord::operator==(const Coord& rhs) const { return (this->x == rhs.x && this->y == rhs.y); };
	bool Coord::operator!=(const Coord& rhs) const { return (this->x != rhs.x || this->y != rhs.y); };

	bool operator<(const Coord& other) const;
	bool operator>(const Coord& other) const;

	bool IsPositive() const { return x >= 0 && y >= 0; };

	Coord ScaleToLargestDimension() const;
	Coord negate() const { return Coord(-x, -y); };
	Coord xyswap() const { return Coord(y, x); };
	Coord yflip() const { return Coord(x, -y); };
	Coord xflip() const { return Coord(-x, y); };
	Coord Flip(const SDL_RendererFlip& flip);
	Coord yadd(int amount) const { return Coord(x, y + amount); };
	Coord xadd(int amount) const { return Coord(x + amount, y); };
	Coord makepositive() const { return Coord(abs(this->x), abs(this->y)); };
	int largestdimension() const;

	Coord discreteRotation(enObjectDiscreteRotation, int x_anchorpoint);
	//Coord flipRotation(enObjectFlipped, int x_anchorpoint, bool isUneven);
	Coord flipRotation(enObjectFlipped, int x_anchorpoint);

	Coord rotateClockWise() const;	//90 degrees
	Coord rotateAntiClockWise() const;  //90 degrees
	Coord rotate180Degrees() const;

	Coord rotateClockWise(int x_anchorpoint) const;	//90 degrees
	Coord rotateAntiClockWise(int x_anchorpoint) const;  //90 degrees
	Coord rotate180Degrees(int x_anchorpoint) const;

	Coord DotProduct(Coord other) { return Coord(this->x*other.x, this->y*other.y); }

	Coord transpose() const;
	Coord FlipAlongXline(const int xLine) const;
	Coord FlipAlongYline(const int yLine) const;

	virtual void clear() { x, y = 0; };

	std::string ToString();

	int SumXY() const { return this->x + this->y; };
	int MultiplyXY() const { return this->x * this->y; };

	int x;
	int y;

	static Coord GetRectSize(const SDL_Rect&);
	static Coord GetRectPosition(const SDL_Rect&);
	SDL_Rect ToSDLRect(const Coord& size) const;
	SDL_Rect ToSDLRect(int w, int h) const;

};

Coord operator+(const Coord& a, const Coord& b); //{ return Coord(a.x + b.x, a.y + b.y); };
Coord operator-(const Coord& a, const Coord& b);
bool compare_coords(const Coord& first, const Coord& second);

//Coord operator|(int a, int b);

Coord GetCenter(const SDL_Rect& rect);

Coord RandomCoordinate(int minx, int miny, int maxx, int maxy);
Coord RandomDirection(int lenght);

template <class T> class CoordWithObject : public Coord
{

public:

	CoordWithObject() : Coord() { Object = T(); };
	CoordWithObject(const T &obj) : Coord() { Object = obj; };
	CoordWithObject(const CoordWithObject &other) : Coord(other) { Object = other.Object; };
	CoordWithObject(const CoordWithObject &&other) : Coord(other) { std::move(Object = other.Object); };
	CoordWithObject(const Coord &other) : Coord(other) { Object = T(); };
	CoordWithObject(const Coord &other, const T& obj) : Coord(other) { Object = obj; };

	CoordWithObject(int X, int Y, T obj) : Coord(X, Y) { Object = obj; };

	CoordWithObject& operator=(const CoordWithObject& other);
	CoordWithObject& operator=(const CoordWithObject&& other);

	CoordWithObject operator+(const Coord& b) { return CoordWithObject(this->x + b.x, this->y + b.y, this->Object); };
	CoordWithObject operator-(const Coord& b) { return CoordWithObject(this->x - b.x, this->y - b.y, this->Object); };

	T Object;
	void NewObject(T obj) { Object = obj; };
	//void newColor(EnumColor enumColor) { Object.newColor(colorFromEnum(enumColor)); };
};


template class CoordWithObject<int>;
template class CoordWithObject<ColorRGB>;
template class CoordWithObject<CollisionData>;

typedef CoordWithObject<ColorRGB> CoordWithColor;
typedef CoordWithObject<CollisionData> CoordWithCollisionData;
typedef CoordWithObject<int> CoordWithInt;
