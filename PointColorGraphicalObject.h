#pragma once
#include"InGameGraphicalObject.h"
#include"PointColor.h"

class PointColorGraphicalObject : public InGameGraphicalObject
{

public:
	
	PointColorGraphicalObject();
	PointColorGraphicalObject( ColorRGB );
	PointColorGraphicalObject( ColorRGB, spriteSize);

	void colorSetNoCollision( const Coord&, ColorRGB ) ;
	void colorSet( const Coord&, ColorRGB color ) ;
	void colorSet( const std::vector<CoordWithColor>& coords );
	void colorSet( const CoordWithColor coords[], int size );
	void colorSet( const CoordWithCollisionData coords[], int size );
	void colorSub( const Coord&, ColorRGB );
	void colorSubNoCollision( const Coord&, ColorRGB ) ;
	void colorAdd( const Coord&, ColorRGB ) ;
	void colorAddNoCollision( const Coord&, ColorRGB ) ;
	void colorSetEdge() ; //for debugging
	void colorModulate( ColorRGB ) ;
	void colorPointInvisible( const Coord& ); 

	void FlipGraphicalObject();
	SDL_RendererFlip FlipValue();

	Coord AxisOfRotation() const;

	ColorRGB getColorFromPoint( const Coord & ) const;
	EnumColorKey getColorKey() const;
	void damagePixelBox( const Coord & pos, const game::Damage& damage );
	spriteSize getSize() const;
	//int getSize() const { return 1; }
	//std::vector<Coord> GraphicalObjectToList();

	spriteSize Scale() override;
	//int getPixels() const;

	//bool getInitHullPointAtPos( const Coord & Coord, game::Hull& out );
	//bool hasFigure() const;

	const game::HitPoints getHitPoint() const;
	bool destroyPixelBox( const Coord& pos );
	bool coordinateIscollision( const Coord& );
	bool areaIscollision(const SDL_Rect&) override;

	virtual void refreshTexture() const ;

	virtual std::shared_ptr<InGameGraphicalObject> copy();

	void setGlobalHeading( double set );
	void setUnitHeading( double set );
	double getGlobalHeading() const;
	double getUnitHeading() const;

	virtual Coord getCenter();
	imagehandling::ITextureObjectRenderer* GetTexture() { return &this->pPointColor; }
	
private:

	//std::list<CoordWithColor> generateDebriFromDestruction();
	PointColor pPointColor;

};