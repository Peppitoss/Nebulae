#include"PointColorGraphicalObject.h"


PointColorGraphicalObject::PointColorGraphicalObject() : InGameGraphicalObject()
{
	pPointColor = PointColor();
}

PointColorGraphicalObject::PointColorGraphicalObject( ColorRGB col ) : InGameGraphicalObject()
{
	pPointColor = PointColor(col);
}
PointColorGraphicalObject::PointColorGraphicalObject( ColorRGB col, spriteSize size ) : InGameGraphicalObject()
{
	pPointColor = PointColor(col,size);
}


void PointColorGraphicalObject::colorSetNoCollision( const Coord&, ColorRGB col )
{
	this->pPointColor.SetColor( col );
}
void PointColorGraphicalObject::colorSet( const Coord&, ColorRGB col )
{
	this->pPointColor.SetColor( col );
}
void PointColorGraphicalObject::colorSet( const std::vector<CoordWithColor>& coords )
{
	this->pPointColor.SetColor( coords.back().Object );
}
void PointColorGraphicalObject::colorSet( const CoordWithCollisionData coords[], int size )
{
}

void PointColorGraphicalObject::colorSet( const CoordWithColor coords[], int size )
{
}

void PointColorGraphicalObject::colorSub( const Coord&, ColorRGB col )
{
	this->pPointColor.SetColor( this->pPointColor.GetCustomModulation() - col );
}
void PointColorGraphicalObject::colorSubNoCollision( const Coord&, ColorRGB col )
{
	this->pPointColor.SetColor( this->pPointColor.GetCustomModulation() - col );
}
void PointColorGraphicalObject::colorAdd( const Coord&, ColorRGB col )
{
	this->pPointColor.SetColor( this->pPointColor.GetCustomModulation() + col );
}
void PointColorGraphicalObject::colorAddNoCollision( const Coord&, ColorRGB col )
{
	this->pPointColor.SetColor( this->pPointColor.GetCustomModulation() + col );
}
void PointColorGraphicalObject::colorSetEdge()
{
	this->pPointColor.SetColor( ColorRGB(255,0,0) );
}
void PointColorGraphicalObject::colorModulate( ColorRGB col )
{
	this->pPointColor.SetColor( col );
}

void PointColorGraphicalObject::colorPointInvisible( const Coord& )
{

}

void PointColorGraphicalObject::FlipGraphicalObject()
{

}

SDL_RendererFlip PointColorGraphicalObject::FlipValue()
{
	return SDL_RendererFlip::SDL_FLIP_NONE;
}

Coord PointColorGraphicalObject::AxisOfRotation() const
{
	return Coord(0);
}

ColorRGB PointColorGraphicalObject::getColorFromPoint( const Coord & ) const
{
	return this->pPointColor.GetCustomModulation();
}
EnumColorKey PointColorGraphicalObject::getColorKey() const
{
	return EnumColorKey::kUNDEFINED;
}
void PointColorGraphicalObject::damagePixelBox( const Coord & pos, const game::Damage& damage )
{
}
spriteSize PointColorGraphicalObject::getSize() const
{
	return spriteSize(1);
}

spriteSize PointColorGraphicalObject::Scale()
{
	return this->pPointColor.GetScaling();
}

//bool PointColorGraphicalObject::getInitHullPointAtPos( const Coord & Coord, game::Hull& out )
//{
//	return false;
//}

//bool PointColorGraphicalObject::hasFigure() const
//{
//	return false;
//}
//
//std::vector<Coord> PointColorGraphicalObject::GraphicalObjectToList()
//{
//	return std::vector<Coord>();
//}
//
//int PointColorGraphicalObject::getPixels() const
//{
//	return 1;
//}

bool PointColorGraphicalObject::destroyPixelBox( const Coord& pos )
{
	return false;
}

const game::HitPoints PointColorGraphicalObject::getHitPoint() const
{
	return game::HitPoints();
}

bool PointColorGraphicalObject::coordinateIscollision( const Coord& Coord)
{
	return this->getCenter() == Coord;
}

bool PointColorGraphicalObject::areaIscollision(const SDL_Rect& rect)
{
	return geometry::pointWithinRectangle_inclusive(this->getCenter(), Coord(rect.x, rect.y), rect.w, rect.h);	//not tested
}

void PointColorGraphicalObject::refreshTexture() const
{
}
//
//bool PointColorGraphicalObject::getInitHullPointAtPos( const Coord & Coord, game::Hull& out )
//{
//	return false;
//}

std::shared_ptr<InGameGraphicalObject> PointColorGraphicalObject::copy()
{
	std::shared_ptr<InGameGraphicalObject> result = std::shared_ptr<InGameGraphicalObject>( new PointColorGraphicalObject( this->pPointColor.GetCustomModulation() ) );
	return result;
}

void PointColorGraphicalObject::setGlobalHeading( double set )
{
}

void PointColorGraphicalObject::setUnitHeading( double set )
{
}

double PointColorGraphicalObject::getGlobalHeading() const
{
	return 0;
}

double PointColorGraphicalObject::getUnitHeading() const
{
	return 0;
}

Coord PointColorGraphicalObject::getCenter()
{
	return Coord(0);
}
