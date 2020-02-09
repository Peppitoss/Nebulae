//#pragma once
//#include"Hitbox.h"
//
//class Point : Hitbox
//{
//
//public:
//	Point();
//	Point(const Coord& position);
//		
//	bool IsPointHit(const Coord& position) const;
//
//	virtual Coord GetBoundary() const { return Coord(1); };
//	virtual Coord GetTopLeft() const { return this->pPointPosition; };
//
//	void DrawHitBox(SDL_Renderer* renderer, const Coord pos, double heading) const;
//	void OffsetTopLeft(const Coord& offset) { this->pPointPosition += offset; };
//	bool IsValid() const { return true; };
//
//private:
//
//	Coord pPointPosition;
//
//};