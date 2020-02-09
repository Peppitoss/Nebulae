#pragma once
#include"InGameGraphicalObject.h"
#include"RectangleColor.h"

class RectangleObject : public InGameGraphicalObject
{

public:

	RectangleObject();
	RectangleObject( int height, int width, ColorRGB );
	void colorSetNoCollision( const Coord&, ColorRGB );  //set colour without testing for collisions
	void colorSet( const Coord&, ColorRGB color );
	void colorSet( const std::vector<CoordWithColor>& coords );
	void colorSet( const CoordWithColor coords[], int size );
	void colorSet( const CoordWithCollisionData coords[], int size );
	void colorSub( const Coord&, ColorRGB );
	void colorSubNoCollision( const Coord&, ColorRGB );
	void colorAdd( const Coord&, ColorRGB );
	void colorAddNoCollision( const Coord&, ColorRGB );
	void colorSetEdge(); //for debugging
	void colorModulate( ColorRGB );
	void colorPointInvisible( const Coord& ); 

	void FlipGraphicalObject();
	SDL_RendererFlip FlipValue();

	Coord AxisOfRotation() const;

	ColorRGB getColorFromPoint( const Coord & ) const;
	EnumColorKey getColorKey() const;
	void damagePixelBox( const Coord & pos, const game::Damage& damage );
	spriteSize getSize() const;
	//int RectangleObject::getSize() const;

	//bool getInitHullPointAtPos( const Coord & Coord, game::Hull& out );

	const game::HitPoints getHitPoint() const;
	bool destroyPixelBox( const Coord& pos );
	//int getPixels() const;

	//std::vector<Coord> GraphicalObjectToList();
	bool coordinateIscollision( const Coord& ) override;
	bool areaIscollision(const SDL_Rect&) override;

	void refreshTexture() const;
	//bool hasFigure() const;

	std::shared_ptr<InGameGraphicalObject> copy();

	void setGlobalHeading( double set );
	void setUnitHeading( double set );
	double getGlobalHeading() const;
	double getUnitHeading() const;

	Coord getCenter();
	imagehandling::ITextureObjectRenderer* GetTexture() { return &this->pRectangle; }

private:

	int pWidth;
	int pHeight;
	RectangleColor pRectangle;

};