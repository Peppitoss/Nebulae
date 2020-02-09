#pragma once
#include"CollisionResult.h"

struct CollisionData
{
	CollisionData();
	CollisionData(int damage, CollisionResult collisionResult);
	int pubDamage;
	CollisionResult pubCollisionresult;
};