#pragma once
#include"Graphics.h"
#include"SurfaceTexturePair.h"

namespace Shapes
{
	using namespace::primitives;

	class Shape
	{
		
	public:

		Shape();

		virtual bool IsPointWithinShape(const Coord& point ) const = 0;

		Coord GetCenterPosition() const;
		Coord GetTopLeftPosition() const;

		virtual void SetCenterPosition(const Coord& value );
		virtual void SetTopLeftPosition(const Coord& value );

		virtual Coord GetSquareBoundarySize() const = 0;

		void DrawShape(SDL_Renderer* renderer, Coord position, double heading);
		void DrawShape(SDL_Renderer* renderer, Coord position);

		virtual int GetArea() = 0;

		virtual Shape* Copy() = 0;

	protected:

		virtual Coord ConvertFromCenterToTopLeft(const Coord& center) const  = 0;
		virtual Coord ConvertFromTopLeftToCenter(const Coord& topleft) const  = 0;

		Coord pCenterPosition;
		Coord pTopLeftPosition;

		SurfaceTexturePair pGraphics;

	private:

		
	
	};


	class Circle : public Shape
	{

	public:

		Circle(const Coord& centerPosition, double radius);
		double GetRadius() const;
		bool IsPointWithinShape(const Coord& point) const;
		Coord GetSquareBoundarySize() const;

		int GetArea();

		Shape* Copy();

	protected:

		Coord ConvertFromCenterToTopLeft(const Coord& center) const;
		Coord ConvertFromTopLeftToCenter(const Coord& topleft) const;

	private:

		double pRadius;

	};

	class Rectangle : public Shape
	{

	public:

		Rectangle(const Coord& topLeftPosition, const Coord& size);

		bool IsPointWithinShape(const Coord& point) const;
		Coord GetSquareBoundarySize() const;
		Coord GetSize() const;

		int GetArea();

		Shape* Copy();

	protected:

		Coord ConvertFromCenterToTopLeft(const Coord& center) const;
		Coord ConvertFromTopLeftToCenter(const Coord& topleft) const;

	private:

		int pWidth;
		int pHeight;

	};





}
