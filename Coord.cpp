#include"Coord.h"
#include"StringBuilder.h"
#include"RandomNumberGenerator.h"


Coord operator+(const Coord& a, const Coord& b)
{
	return Coord(a.x + b.x, a.y + b.y);
}

Coord operator-(const Coord& a, const Coord& b)
{
	return Coord(a.x - b.x, a.y - b.y);
}



Coord& Coord::operator=(const Coord&& other)
{
	this->x = std::move(other.x);
	this->y = std::move(other.y); 
	return *this;
}

bool Coord::operator<(const Coord& other)  const
{
	if (x < other.x)
	{
		return true;
	}
	else if (x == other.x)
		return y < other.y;
	else
		return false;
}

bool Coord::operator>(const Coord& other) const
{
	if (x > other.x)
	{
		return true;
	}
	else if (x == other.x)
		return y > other.y;
	else
		return false;
}

std::string Coord::ToString()
{
	return IntToString(x) + "," + IntToString(y);
}

SDL_Rect Coord::ToSDLRect(const Coord& size) const
{
	return ToSDLRect(size.x, size.y);
}

SDL_Rect Coord::ToSDLRect(int w, int h) const
{
	SDL_Rect result = { this->x, this->y,w, h };
	return result;
}

Coord Coord::GetRectSize(const SDL_Rect& rect)
{
	Coord result;
	result.x = rect.w;
	result.y = rect.h;
	return result;
}
Coord Coord::GetRectPosition(const SDL_Rect& rect)
{
	Coord result;
	result.x = rect.x;
	result.y = rect.y;
	return result;
}

Coord GetCenter(const SDL_Rect& rect)
{
	return Coord(rect.x + rect.w / 2, rect.y + rect.h / 2);
}

Coord RandomCoordinate(int minx, int miny, int maxx, int maxy)
{
	std::uniform_int_distribution<int> dist_x(minx, maxx);
	std::uniform_int_distribution<int> dist_y(miny, maxy);

	return Coord(dist_x(RANDGENERATOR()), dist_y(RANDGENERATOR()));
}
Coord RandomDirection(int lenght)
{
	return RandomCoordinate(-1, -1, 1, 1);
}


Coord Coord::FlipAlongXline(const int xLine) const
{
	int difference = xLine - this->x;
	int newX = difference + xLine;
	return Coord(newX, this->y);
}
Coord Coord::FlipAlongYline(const int yLine) const
{
	int difference = yLine - this->y;
	int newY = difference + yLine;
	return Coord(this->x, newY);
}

int Coord::largestdimension() const
{
	return x > y ? x : y;
}

Coord Coord::ScaleToLargestDimension() const
{
	return Coord(this->largestdimension());
}

Coord Coord::Flip(const SDL_RendererFlip& flip)
{
	switch (flip)
	{
	case(SDL_RendererFlip::SDL_FLIP_NONE):
			return Coord(*this);
	case(SDL_RendererFlip::SDL_FLIP_HORIZONTAL):
		return this->xflip();
	case(SDL_RendererFlip::SDL_FLIP_VERTICAL):
		return this->yflip();
	default:
		return Coord(*this);
	}
}

Coord Coord::discreteRotation(enObjectDiscreteRotation rotation, int x_anchorpoint)
{
	switch (rotation)
	{
	case(enObjectDiscreteRotation::go90Rotation):
		return this->rotateClockWise(x_anchorpoint);
	case(enObjectDiscreteRotation::go180Rotation):
		return rotate180Degrees(x_anchorpoint);
	case(enObjectDiscreteRotation::go270Rotation):
		return this->rotateAntiClockWise(x_anchorpoint);
	default:
		return *this;
	}
}

Coord Coord::flipRotation(enObjectFlipped flip, int x_anchorpoint)
{
	switch (flip)
	{
	case(enObjectFlipped::goIsFlipped):
		return this->FlipAlongXline(x_anchorpoint);
	default:
		return *this;
	}
}

Coord Coord::rotateClockWise() const
{
	return rotateClockWise(0);
}
Coord Coord::rotateAntiClockWise() const
{
	return rotateAntiClockWise(0);
}

Coord Coord::rotate180Degrees() const
{
	return this->rotateClockWise(0).rotateClockWise(0);
}

Coord Coord::rotateClockWise(int x_anchorpoint) const
{
	return this->xyswap().FlipAlongXline(x_anchorpoint);
}
Coord Coord::rotateAntiClockWise(int x_anchorpoint) const
{
	return this->FlipAlongXline(x_anchorpoint).xyswap();
}
Coord Coord::rotate180Degrees(int x_anchorpoint) const
{
	return this->rotateClockWise(x_anchorpoint).rotateClockWise(x_anchorpoint);
}

Coord Coord::transpose() const
{
	return this->xyswap();
}

template <class T> CoordWithObject<T>& CoordWithObject<T>::operator=(const CoordWithObject& other)
{
	this->x = other.x;
	this->y = other.y;
	this->Object = other.Object;
	return *this;
}

template <class T> CoordWithObject<T>& CoordWithObject<T>::operator=(const CoordWithObject&& other)
{
	this->x = std::move(other.x);
	this->y = std::move(other.y);
	this->Object = std::move(other.Object);
	return *this;
}

bool compare_coords(const Coord& first, const Coord& second)
{
	if (first.y == second.y)
		return first.x<second.x;
	else
		return first.y < second.y;
}
