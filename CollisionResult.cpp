#include"CollisionResult.h"


CollisionResult::CollisionResult()
{
	pLinkIndexHit = -1;
	pHitMainBody = false;
	pCollisionIsMiss = false;
	pTakeNoDamage = false;
}

void CollisionResult::SetMainBodyhit()
{
	pLinkIndexHit = -1;
	pHitMainBody = true;
	pCollisionIsMiss = false;
}

void CollisionResult::SetIndexHit(int index)
{
	pLinkIndexHit = index;
	pHitMainBody = false;
	pCollisionIsMiss = false;
}
void CollisionResult::SetTakeNoDamage(bool value)
{
	pTakeNoDamage = value;
}
void CollisionResult::SetCollisionIsMiss()
{
	pLinkIndexHit = -1;
	pHitMainBody = false;
	pCollisionIsMiss = true;
}

