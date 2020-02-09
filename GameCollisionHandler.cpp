#include "GameCollisionHandler.h"
#include <stdlib.h>     /* srand, rand */

GameCollisionHandler::GameCollisionHandler()
{
	//burnColor.newColor(ColorRGB(50,0,0,255) );
	//whiteColor.newColor(ColorRGB(255,255,255,255) );
	//AniExplosions AnimatingExplosives1();
}

//bool GameCollisionHandler::ProjectileHit( const Projectile& projectile )
//{
//	return (rand() % 1000) < (int)(projectile.getHitChance()*1000);
//}

void GameCollisionHandler::ComputeDamage( const CollisionHandle_DE_PR& coll , const Coord& impactPosition, std::shared_ptr<InGameGraphicalObject>& debri)
{
	coll.DE_A->ApplyDamage( coll.DE_A->getGraphics()->transformPositionToObjectFrame( impactPosition ), coll.PR_A->getDamage() );
}

void GameCollisionHandler::TriggerAnimation(  const CollisionHandle_DE_PR& coll, const Coord& impactPosition, Animator& out_Animator )
{
	int hitAnimationSize = (int)coll.PR_A->getHitAnimation()*0.5;//(int)coll.PR_A*0.5;
 	out_Animator.generateHitAnimation( coll.PR_A->getHitAnimation(), (coll.DE_A->getPosition() + impactPosition.FlipAlongYline(hitAnimationSize) ), false); //coll.PR_A->()*0.5)
}

void GameCollisionHandler::TriggerAnimation(  const CollisionHandle_PR_PR& coll, const Coord& hunterhitpoint, const Coord& targethitpoint, Animator& out_Animator )
{
	out_Animator.generateHitAnimation( coll.PR_hunting->getHitAnimation(), hunterhitpoint, false);
	out_Animator.generateHitAnimation( coll.PR_prey->getHitAnimation(), targethitpoint, false);
}

std::shared_ptr<Debri> GameCollisionHandler::GenerateDebri( const CollisionHandle_DE_PR& coll, const Coord& impactPosition, std::shared_ptr<InGameGraphicalObject>& debri , particleEffects::ParticleEngine & engine )
{
	std::shared_ptr<Debri> vesselChopOut = this->createVesselDebri(50000, debri, coll.DE_A->getPosition(), coll.DE_A->getGlobalHeading(), impactPosition );
	vesselChopOut->SetParticleEngine( engine );
	//vesselChopOut->setVelocity(  coll.getVelB()/(20+vesselChopOut->getGraphics()->getGraphicalObject()->getPixels()) );
    return vesselChopOut;
}

void GameCollisionHandler::ComputePhysics( const CollisionHandle_DE_PR& coll, const Coord& impactPosition )
{
	primitives::vector hitForce = ( coll.getVelB() );
	hitForce = hitForce*(coll.PR_A->getDamage().value + (coll.PR_A->getDamage().AOE * sqrt(1+coll.PR_A->getDamage().AOE_range))/2 );
	coll.DE_A->setImpulseForce(hitForce);
	coll.PR_A->disable();
	coll.DE_A->reqRefresh = true;
}

//void GameCollisionHandler::ColorAtCollision(const CollisionHandle_DE_PR& coll, const Coord& impactPosition)
//{
//	std::map<Coord,int> damageResult = coll.DE_A->TransferAccumulatedDamage();
//	for( std::map<Coord,int>::const_iterator it = damageResult.begin(); it != damageResult.end(); ++it )
//	{
//		if((*it).second>0)
//			paintScorchMark( coll.DE_A->getGraphics()->transformPositionToObjectFrame( it->first ), *coll.DE_A, coll.PR_A->getDamage().value );//
//	}
//}

void GameCollisionHandler::Handle( std::vector<CollisionHandle_DE_PR*>& handleList, particleEffects::ParticleEngine & engine, Animator& out_Animator ) //input integrator?
{
	for( auto it = handleList.begin() ; it != handleList.end() ; ++it )
	{
		//if( ProjectileHit( *(*it)->PR_A ) )
		//{
			Coord impactPosition = (*it)->getTargetHitPos();
			std::shared_ptr<InGameGraphicalObject> debri;

			ComputePhysics(*(*it), impactPosition);

			if( (*it)->GetCollisionType() == enCollisionType::ctProjectileImpact ) 
  				ComputeDamage( *(*it), impactPosition, debri );
			TriggerAnimation( *(*it), impactPosition, out_Animator );

			//if( debri != nullptr )
			//	out_debri.push_back( GenerateDebri( *(*it), impactPosition, debri, engine ) );
			//ColorAtCollision( *(*it), impactPosition );
			
			engine.AddNewEffect(engine.GetEffectRandomDirectedSpread((*it)->DE_A->getPosition().yflip() + impactPosition, (*it)->getVelB()/3, enParticleColoring::greyscale, 6, 7000 ));
		//}
	}
}

void GameCollisionHandler::Handle(const std::list<CollisionHandle_DE_DE*> handleList, int ms)
{
	for (std::list<CollisionHandle_DE_DE*>::const_iterator it = handleList.begin(); it != handleList.end(); ++it)
	{
		//(*it)->DE_A->RevertLastChangeInPosition();
		//(*it)->DE_A->move(ms);
		//(*it)->DE_A->setAngularVelocity(0);
	}
}

std::shared_ptr<Debri> GameCollisionHandler::createVesselDebri( int lifeTime, std::shared_ptr<InGameGraphicalObject> sprite, Coord position, double heading, Coord burnPoint )
{
	std::shared_ptr<Debri> result = std::shared_ptr<Debri>(new Debri( std::shared_ptr<GraphicalEntity>(new GraphicalEntity( sprite, lifeTime, EntityClass::DOODAD, true ))));
    result->SetPosition( position );
	result->setGlobalHeading( heading );
	result->SetSimpleMass(100);	//might want to change this number to something more appropriate // result->getGraphics()->getGraphicalObject().get()->getPixels();
	result->SetSimpleMomentOfInertia(200);
	result->pointOfInterest = burnPoint;
	return result;
}

//void GameCollisionHandler::paintScorchMark( Coord pos, DynamicEntity& object, int damage )
//{
//	int intensity = (rand() % 10)+10+damage;
// 	object.getGraphics()->getGraphicalObject().get()->colorSub(pos, ColorRGB(intensity,intensity,intensity,255), true );
//}


void GameCollisionHandler::Handle( std::vector<CollisionHandle_PR_PR*>& handleList, particleEffects::ParticleEngine & engine, Animator& out_Animator )
{
	for( auto it = handleList.begin() ; it != handleList.end() ; ++it )
	{
		//if( ProjectileHit( *(*it)->PR_hunting ) )
		//{
			Coord targethitpoint = (*it)->getTargetHitPos();
			Coord hunterhitpoint = (*it)->hunterHitPoint;
			TriggerAnimation( *(*it), hunterhitpoint, targethitpoint, out_Animator );

			(*it)->PR_hunting->DestroyEntity();
			(*it)->PR_prey->DestroyEntity();
		//}
	}
}
