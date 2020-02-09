#pragma once
#include"ModuleEntity.h"

enum enHitBoxCollision
{
	ColProjectile = 0,
	ColVessel = 1,
	ColBoth = 2
};

class HitBoxEntity : public ModuleEntity
{

public :
	
	HitBoxEntity( int height, int width, std::shared_ptr<GraphicalEntity> ge, int type);
	HitBoxEntity( int radius, std::shared_ptr<GraphicalEntity> ge, int type);

	bool PointInsideHitBox( const Coord pos );

	enHitBoxCollision GetCollisionType() const { return this->pCollision; }

private:

	enHitBoxCollision pCollision;
	int pWidth;
	int pHeight;
	int pRadius;

};