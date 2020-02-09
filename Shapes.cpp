#include"Shapes.h"
#include"Graphics.h"
#include"Wrapper.h"

namespace Shapes
{
	using namespace::primitives;
	Shape::Shape()
	{

	}

	Coord Shape::GetCenterPosition() const
	{
		return this->pCenterPosition;
	}

	Coord Shape::GetTopLeftPosition() const
	{
		return this->pTopLeftPosition;
	}

	void Shape::SetCenterPosition(const Coord& value)
	{
		this->pCenterPosition = value;
		this->pTopLeftPosition = ConvertFromCenterToTopLeft(value);
	}

	void Shape::SetTopLeftPosition(const Coord& value)
	{
		this->pTopLeftPosition = value;
		this->pCenterPosition = ConvertFromTopLeftToCenter(value);
	}

	void Shape::DrawShape(SDL_Renderer* renderer, Coord position, double heading)
	{
		this->pGraphics.DrawSurface(renderer, position, heading);
	}
	void Shape::DrawShape(SDL_Renderer* renderer, Coord position)
	{
		this->pGraphics.DrawSurface(renderer, position);
	}

	Circle::Circle(const Coord& centerPosition, double radius)
	{
		this->SetCenterPosition(centerPosition);
		this->pRadius = radius;
		
		this->pGraphics = SurfaceTexturePair();
		//Wrapper sdlWrapper = Wrapper();
		this->pGraphics.GenerateSurface(std::shared_ptr<SDL_Surface>(sdlWrapper.CreateCircle(Wrapper::GetSDLColor(COLOUR::C_GREEN), this->pRadius * 2), SDL_FreeSurface));
	}

	double Circle::GetRadius() const
	{
		return this->pRadius;
	}

	Shape* Circle::Copy()
	{
		Circle* result = new Circle(this->pCenterPosition, this->pRadius);
		return result;
	}

	Coord Circle::ConvertFromCenterToTopLeft(const Coord& center) const
	{
		return center - Coord(this->GetRadius());
	}
	Coord Circle::ConvertFromTopLeftToCenter(const Coord& topleft) const
	{
		return topleft + Coord(this->GetRadius());
	}

	bool Circle::IsPointWithinShape(const Coord& point) const
	{
		return geometry::realDistanceQuadratic(this->GetCenterPosition(), point) < std::pow(this->GetRadius(),2);
	}

	Coord Circle::GetSquareBoundarySize() const
	{
		return Coord(2 * pRadius);
	}

	int Circle::GetArea()
	{
		return std::_Pi*this->GetRadius()*this->GetRadius();
	}

	Rectangle::Rectangle(const Coord& topLeftPosition, const Coord& size)
	{
		this->SetTopLeftPosition(topLeftPosition);
		this->pHeight = size.y;
		this->pWidth = size.x;

		this->pGraphics = SurfaceTexturePair();
		//Wrapper sdlWrapper = Wrapper();
		this->pGraphics.GenerateSurface(std::shared_ptr<SDL_Surface>(sdlWrapper.CreateSurface( pWidth, pHeight, COLOUR::C_GREEN), SDL_FreeSurface));
	}

	int Rectangle::GetArea()
	{
		return this->GetSize().MultiplyXY();
	}

	Coord Rectangle::GetSize() const
	{
		return Coord(this->pWidth, this->pHeight);
	}

	Coord Rectangle::ConvertFromCenterToTopLeft(const Coord& center) const
	{
		return center - this->GetSize() / 2;
	}
	Coord Rectangle::ConvertFromTopLeftToCenter(const Coord& topleft) const
	{
		return topleft + this->GetSize() / 2;
	}

	bool Rectangle::IsPointWithinShape(const Coord& point) const
	{
		return geometry::pointWithinRectangle_inclusive(point, GetTopLeftPosition(), this->pWidth, this->pHeight);
	}

	Coord Rectangle::GetSquareBoundarySize() const
	{
		return Coord(this->pWidth, this->pHeight);
	}

	Shape* Rectangle::Copy()
	{
		Rectangle* newRect = new Rectangle( this->pTopLeftPosition, this->GetSize() );
		return newRect;
	}

}
