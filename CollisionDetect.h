#pragma once
#include"Projectile.h"
#include"Vessel.h"
#include"WalkingEntity.h"
#include"InteriorEntity.h"

enum enCollisionType
{
	ctOnlyAnimation  = 0,
	ctProjectileImpact = 1,
	ctEntityCollision = 2
};


namespace CollisionDetect {

	enum RectSide
	{
		rNone = 0,
		rTop = 1,
		rLeft = 2,
		rRight = 4,
		rBottom = 8
	};

	inline RectSide operator|(RectSide a, RectSide b)
	{
		return static_cast<RectSide>(static_cast<int>(a) | static_cast<int>(b));
	}

	bool rangeIntersection(int r1_minX, int r1_maxX, int r2_minX, int r2_maxX);

	RectSide AngleBetweenRectangle(const SDL_Rect& target, const SDL_Rect& source);

	struct colData
	{
		colData() { isValidCollision = false; collisionType = ctProjectileImpact; };

		bool isValidCollision;
		Coord targetHitpos;
		Coord hunterHitPos;
		primitives::vector vel_A;
		primitives::vector vel_B;
		enCollisionType collisionType;
	};

	bool projectileWithinBoundary(DynamicEntity& A, Projectile& B);
	colData dynamicEntityHullDetect( DynamicEntity& A, Projectile& B );
	colData ProjectileProjectileCollisionDetect( Projectile& A, Projectile& B );
	colData WalkerInteriorCollision(WalkingEntity& A, InteriorEntity& B);
}


class CollisionHandle
{

public:
	
	CollisionHandle(Coord targetHitPos, primitives::vector velA, primitives::vector velB, enCollisionType collisiontype);

	Coord getTargetHitPos() const { return TargetHitPos; };
	primitives::vector getVelA() const { return pVelA; };
	primitives::vector getVelB() const { return pVelB; };

	enCollisionType GetCollisionType() { return this->pCollisionType; };

protected:

	enCollisionType pCollisionType;
	Coord TargetHitPos;
	primitives::vector pVelA;
	primitives::vector pVelB;

private:

	

	CollisionHandle( const CollisionHandle& );
	CollisionHandle( CollisionHandle&& );
	CollisionHandle& operator=( const CollisionHandle& );
	CollisionHandle& operator=( CollisionHandle&& );

};

class CollisionHandle_DE_PR : public CollisionHandle
{

public:
	
	CollisionHandle_DE_PR(std::shared_ptr<DynamicEntity> entity, std::shared_ptr<Projectile> projectile, CollisionDetect::colData& pData);

	std::shared_ptr<DynamicEntity> DE_A;
	std::shared_ptr<Projectile> PR_A;

private:

	CollisionHandle_DE_PR( const CollisionHandle_DE_PR& );
	CollisionHandle_DE_PR( CollisionHandle_DE_PR&& );
	CollisionHandle_DE_PR& operator=(const CollisionHandle_DE_PR& );
	CollisionHandle_DE_PR& operator=(CollisionHandle_DE_PR&& );


};

class CollisionHandle_DE_DE : public CollisionHandle
{

public:
	
	CollisionHandle_DE_DE(std::shared_ptr<DynamicEntity> entityA, std::shared_ptr<DynamicEntity> entityB, Coord pos, primitives::vector velA, primitives::vector velB);

	std::shared_ptr<DynamicEntity> DE_A;
	std::shared_ptr<DynamicEntity> DE_B;

private:

	CollisionHandle_DE_DE( const CollisionHandle_DE_DE& );
	CollisionHandle_DE_DE( CollisionHandle_DE_DE&& );
	CollisionHandle_DE_DE& operator=(const CollisionHandle_DE_DE& );
	CollisionHandle_DE_DE& operator=(CollisionHandle_DE_DE&& );

};

class CollisionHandle_PR_PR : public CollisionHandle
{

public:
	
	CollisionHandle_PR_PR(std::shared_ptr<Projectile> preyProjectile, std::shared_ptr<Projectile> huntingProjectile, Coord preyhitpoint,
		Coord hunterhitpoint, primitives::vector velA, primitives::vector velB);

	std::shared_ptr<Projectile> PR_prey;
	std::shared_ptr<Projectile> PR_hunting;

	Coord hunterHitPoint;

private:

	CollisionHandle_PR_PR( const CollisionHandle_DE_DE& );
	CollisionHandle_PR_PR( CollisionHandle_DE_DE&& );
	CollisionHandle_PR_PR& operator=(const CollisionHandle_DE_DE& );
	CollisionHandle_PR_PR& operator=(CollisionHandle_DE_DE&& );

};

//class CollisionHandle_VE_PR : public CollisionHandle
//{
//
//public:
//	
//	CollisionHandle_VE_PR(std::shared_ptr<Vessel> entity, std::shared_ptr<Projectile> projectile, Coord pos, primitives::vector velA, primitives::vector velB);
//
//	std::shared_ptr<Vessel> VE_A;
//	std::shared_ptr<Projectile> PR_A;
//
//private:
//
//	CollisionHandle_VE_PR( const CollisionHandle_VE_PR& );
//	CollisionHandle_VE_PR( CollisionHandle_VE_PR&& );
//	CollisionHandle_VE_PR& operator=(const CollisionHandle_VE_PR& );
//	CollisionHandle_VE_PR& operator=(CollisionHandle_VE_PR&& );
//
//};
