#include"RectangleObject.h"

RectangleObject::RectangleObject() : InGameGraphicalObject(), pRectangle(0,0)
{
	this->pWidth = 0;
	this->pHeight = 0;
}

RectangleObject::RectangleObject( int height, int width, ColorRGB col ) : InGameGraphicalObject(), pRectangle(width,height,col)
{
	this->pWidth = width;
	this->pHeight = height;
}

void RectangleObject::colorSetNoCollision( const Coord&, ColorRGB col )
{
	pRectangle.SetColor( col );
}
void RectangleObject::colorSet( const Coord&, ColorRGB color )
{
	pRectangle.SetColor( color );
}
void RectangleObject::colorSet( const std::vector<CoordWithColor>& coords )
{
	pRectangle.SetColor( coords.back().Object );
}
void RectangleObject::colorSet( const CoordWithColor coords[], int size )
{
}
void RectangleObject::colorSet( const CoordWithCollisionData coords[], int size )
{
}
void RectangleObject::colorSub( const Coord&, ColorRGB col)
{
	this->pRectangle.SetColor( this->pRectangle.GetCustomModulation() - col );
}
void RectangleObject::colorSubNoCollision( const Coord&, ColorRGB col)
{
	this->pRectangle.SetColor( this->pRectangle.GetCustomModulation() - col );
}
void RectangleObject::colorAdd( const Coord&, ColorRGB col)
{
	this->pRectangle.SetColor( this->pRectangle.GetCustomModulation() + col );
}
void RectangleObject::colorAddNoCollision( const Coord&, ColorRGB col)
{
	this->pRectangle.SetColor( this->pRectangle.GetCustomModulation() + col );
}
void RectangleObject::colorSetEdge()
{
}
void RectangleObject::colorModulate( ColorRGB col)
{
	this->pRectangle.SetColor( col );
}
void RectangleObject::colorPointInvisible( const Coord& )
{
}

void RectangleObject::FlipGraphicalObject()
{

}

Coord RectangleObject::AxisOfRotation() const
{
	return Coord(this->pWidth / 2, this->pHeight / 2);
}

SDL_RendererFlip RectangleObject::FlipValue()
{
	return SDL_RendererFlip::SDL_FLIP_NONE;
}

//bool RectangleObject::getInitHullPointAtPos( const Coord & Coord, game::Hull& out )
//{
//	return false;
//}

const game::HitPoints RectangleObject::getHitPoint() const
{
	return game::HitPoints();
}

bool RectangleObject::destroyPixelBox( const Coord& pos )
{
	return false;
}

//std::vector<Coord> RectangleObject::GraphicalObjectToList()
//{
//	return std::vector<Coord>();
//}
//
//int RectangleObject::getPixels() const
//{
//	return this->pWidth*this->pHeight;
//}

ColorRGB RectangleObject::getColorFromPoint( const Coord & ) const
{
	return this->pRectangle.GetCustomModulation();
}
EnumColorKey RectangleObject::getColorKey() const
{
	return EnumColorKey::kUNDEFINED;
}
void RectangleObject::damagePixelBox( const Coord & pos, const game::Damage& damage)
{
}
spriteSize RectangleObject::getSize() const
{
	return spriteSize(this->pWidth, this->pHeight);
}

//bool RectangleObject::hasFigure() const
//{
//	return false;
//}

//int RectangleObject::getSize() const
//{
//	return 0;
//}

bool RectangleObject::coordinateIscollision( const Coord& pos)
{
	if( pos.x >= 0 && pos.x < pWidth )
		if(pos.y >= 0 && pos.y < pHeight )
			return true;
	return false;
}

bool RectangleObject::areaIscollision(const SDL_Rect& rect)
{
	Coord topleft = Coord(0);
	SDL_Rect intersect = geometry::GetRectangleIntersect(topleft, pWidth, pHeight, Coord(rect.x, rect.y), rect.w, rect.h);
	return intersect.w > 0 && intersect.h > 0;
}
void RectangleObject::refreshTexture() const
{
}

std::shared_ptr<InGameGraphicalObject> RectangleObject::copy()
{
	return nullptr;
}

void RectangleObject::setGlobalHeading( double set )
{
}
void RectangleObject::setUnitHeading( double set )
{
}
double RectangleObject::getGlobalHeading() const
{
	return 0;
}
double RectangleObject::getUnitHeading() const
{
	return 0;
}

Coord RectangleObject::getCenter()
{
	return Coord(pWidth / 2, pHeight / 2);
}
