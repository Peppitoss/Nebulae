#include"CollisionData.h"


CollisionData::CollisionData(int damage, CollisionResult collisionResult)
{
	pubDamage = damage;
	pubCollisionresult = collisionResult;
}

CollisionData::CollisionData() : pubCollisionresult()
{
	pubDamage = 0;
}
