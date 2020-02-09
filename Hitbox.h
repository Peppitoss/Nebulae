#pragma once

#include"Graphics.h"


//enum HitBoxShape
//{
//	hbSquare = 1,
//	hbCircle = 2
//};

class Hitbox
{

public:

	Hitbox();
	virtual bool IsPointHit(const Coord& point) const = 0;
	virtual bool IsCollision(const SDL_Rect& other);

	virtual Coord GetBoundary() const = 0;
	virtual Coord GetTopLeft() const = 0;

	virtual void DrawHitBox(SDL_Renderer* renderer, const Coord pos, double heading) const = 0;
	virtual bool IsValid() const = 0;

	virtual void OffsetTopLeft(const Coord& offset) = 0;
	virtual int GetArea() = 0;

	Coord GetCenterOfRotation() const { return this->pCenterOfRotation; };
	void SetCenterOfRotation( const Coord& set ) { this->pCenterOfRotation = set; };

	SDL_Rect ToSLDRect() const;

protected:

	Coord pCenterOfRotation;

};