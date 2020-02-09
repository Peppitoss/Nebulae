#include"HitBoxEntity.h"


HitBoxEntity::HitBoxEntity( int height, int width, std::shared_ptr<GraphicalEntity> ge, int type) : ModuleEntity(ge, "noname")
{
	this->pHeight = height;
	this->pWidth = width;
	this->pRadius = -1;
	if (type >= 0 && type < 3)
		this->pCollision = (enHitBoxCollision)type;
	else
		this->pCollision = enHitBoxCollision::ColProjectile;
}

HitBoxEntity::HitBoxEntity(int radius, std::shared_ptr<GraphicalEntity> ge, int type) : ModuleEntity(ge, "noname")
{
	this->pHeight = -1;
	this->pWidth = -1;
	this->pRadius = radius;
	if (type >= 0 && type < 3)
		this->pCollision = (enHitBoxCollision)type;
	else
		this->pCollision = enHitBoxCollision::ColProjectile;
}

bool HitBoxEntity::PointInsideHitBox( const Coord pos )
{
	return geometry::pointWithinRectangle_inclusive( pos, this->getPosition(), this->pWidth, this->pHeight );
}