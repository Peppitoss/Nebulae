#pragma once
#include "Debri.h"
#include "CollisionDetect.h"
#include"ParticleEngine.h"
#include"Animation.h"

#include<list>

class GameCollisionHandler
{
public:
	
	GameCollisionHandler();

	void Handle(const std::list<CollisionHandle_DE_DE*> collisions, int ms);
	void Handle( std::vector<CollisionHandle_DE_PR*>& collisions, particleEffects::ParticleEngine & engine, Animator& out_Animator );
	void Handle( std::vector<CollisionHandle_PR_PR*>& collisions, particleEffects::ParticleEngine & engine, Animator& out_Animator );
	//static void paintScorchMark( Coord pos, DynamicEntity& object, int damage ); 

	//static bool ProjectileHit(const Projectile&);

private:

	void ComputeDamage( const CollisionHandle_DE_PR& , const Coord& impactPosition, std::shared_ptr<InGameGraphicalObject>& );

	void TriggerAnimation(  const CollisionHandle_DE_PR& , const Coord& impactPosition, Animator& );
	void TriggerAnimation(  const CollisionHandle_PR_PR& , const Coord& impactPosition1, const Coord& impactPositionEnemy, Animator& );
	void ComputePhysics( const CollisionHandle_DE_PR& , const Coord& impactPosition );
	//void ColorAtCollision(const CollisionHandle_DE_PR& , const Coord& impactPosition);
	std::shared_ptr<Debri> GameCollisionHandler::GenerateDebri( const CollisionHandle_DE_PR& , const Coord& impactPosition, std::shared_ptr<InGameGraphicalObject>& debri, particleEffects::ParticleEngine & engine);
		
 	//bool paintBurnMark( Coord pos, DynamicEntity& object );				moved to graphics
	//bool paintColorKey( Coord pos, DynamicEntity& object ); in use??

	std::shared_ptr<Debri> createVesselDebri( int lifeTime, std::shared_ptr<InGameGraphicalObject>, Coord pos, double heading, Coord burnPoint ); //uses new

	//AniExplosions AnimatingExplosives1;

	GameCollisionHandler( const GameCollisionHandler& );
	GameCollisionHandler( GameCollisionHandler&& );
	GameCollisionHandler& operator=( const GameCollisionHandler& );
	GameCollisionHandler& operator=( GameCollisionHandler&& );


};


