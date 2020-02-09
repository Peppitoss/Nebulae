#pragma once



class CollisionResult
{

public:
	CollisionResult();

	void SetMainBodyhit();
	void SetIndexHit(int index);
	void SetTakeNoDamage(bool noDamage);
	void SetCollisionIsMiss();

	int GetHitIndex() const { return this->pLinkIndexHit; };
	bool IsMainBodyHit() const { return this->pHitMainBody; };
	bool IsHitMiss() const { return this->pCollisionIsMiss; };
	bool TakeNoDamage() const { return this->pTakeNoDamage; };
	bool IsHit() const { return !IsHitMiss(); }

private:

	int pLinkIndexHit;
	bool pHitMainBody;
	bool pCollisionIsMiss;
	bool pTakeNoDamage;

};