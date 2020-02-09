#include"CircleObject.h"

CircleObject::CircleObject() : InGameGraphicalObject(), pCircle(0)
{
	this->pRadius = 0;
}
CircleObject::CircleObject(int radius, ColorRGB col) : InGameGraphicalObject(), pCircle(radius, col)
{
	this->pRadius = radius;
}

void CircleObject::colorSetNoCollision(const Coord&, ColorRGB col)
{
	pCircle.SetColor(col);
}
void CircleObject::colorSet(const Coord&, ColorRGB color)
{
	pCircle.SetColor(color);
}
void CircleObject::colorSet(const std::vector<CoordWithColor>& coords)
{
	pCircle.SetColor(coords.back().Object);
}
void CircleObject::colorSet(const CoordWithColor coords[], int size)
{
}
void CircleObject::colorSet(const CoordWithCollisionData coords[], int size)
{
}
void CircleObject::colorSub(const Coord&, ColorRGB col)
{
	this->pCircle.SetColor(this->pCircle.GetCustomModulation() - col);
}
void CircleObject::colorSubNoCollision(const Coord&, ColorRGB col)
{
	this->pCircle.SetColor(this->pCircle.GetCustomModulation() - col);
}
void CircleObject::colorAdd(const Coord&, ColorRGB col)
{
	this->pCircle.SetColor(this->pCircle.GetCustomModulation() + col);
}
void CircleObject::colorAddNoCollision(const Coord&, ColorRGB col)
{
	this->pCircle.SetColor(this->pCircle.GetCustomModulation() + col);
}
void CircleObject::colorSetEdge()
{
}
void CircleObject::colorModulate(ColorRGB col)
{
	this->pCircle.SetColor(col);
}
void CircleObject::colorPointInvisible(const Coord&)
{
}

void CircleObject::FlipGraphicalObject()
{

}

Coord CircleObject::AxisOfRotation() const
{
	return Coord(this->pRadius);
}

SDL_RendererFlip CircleObject::FlipValue()
{
	return SDL_RendererFlip::SDL_FLIP_NONE;
}

//bool RectangleObject::getInitHullPointAtPos( const Coord & Coord, game::Hull& out )
//{
//	return false;
//}

const game::HitPoints CircleObject::getHitPoint() const
{
	return game::HitPoints();
}

bool CircleObject::destroyPixelBox(const Coord& pos)
{
	return false;
}

ColorRGB CircleObject::getColorFromPoint(const Coord &) const
{
	return this->pCircle.GetCustomModulation();
}
EnumColorKey CircleObject::getColorKey() const
{
	return EnumColorKey::kUNDEFINED;
}
void CircleObject::damagePixelBox(const Coord & pos, const game::Damage& damage)
{
}
spriteSize CircleObject::getSize() const
{
	return spriteSize(0);
}

bool CircleObject::coordinateIscollision(const Coord& pos)
{
	if (geometry::dotProduct(pos - Coord(pRadius)) <= pRadius*pRadius)
		return true;
	return false;
}

bool CircleObject::areaIscollision(const SDL_Rect&)
{
	return false;
}

void CircleObject::refreshTexture() const
{
}

std::shared_ptr<InGameGraphicalObject> CircleObject::copy()
{
	return nullptr;
}

void CircleObject::setGlobalHeading(double set)
{
}
void CircleObject::setUnitHeading(double set)
{
}
double CircleObject::getGlobalHeading() const
{
	return 0;
}
double CircleObject::getUnitHeading() const
{
	return 0;
}

Coord CircleObject::getCenter()
{
	return Coord(0);
}
