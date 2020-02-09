#pragma once
#include"Particles.h"


class ParticleObject : InGameGraphicalObject
{
public :

	ParticleObject();
	ParticleObject( const BoxList& );

	//void colorSet( const Coord&, ColorRGB color );
	//void colorSub( const Coord&, ColorRGB );
	//void colorAdd( const Coord&, ColorRGB );
	//void colorSetNoCollision( const Coord&, ColorRGB );  //set colour without testing for collisions
	//void colorSetEdge(); //for debugging
	//void colorModulate( ColorRGB );

	//ColorRGB getColorFromPoint( const Coord & ) const;
	//bool coordinateIscollision( const Coord& );

	//void refreshTexture() const;
	//int placedPixels();


protected:

	//std::list<std::shared_ptr<MassParticle>> pMassParticleList; //make its own class???

};

//class SingleParticleObject : ParticleObject
//{
//public:
//	SingleParticleObject( ColorRGB color );
//
//private:
//};