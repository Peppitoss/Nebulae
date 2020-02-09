#include"GraphicalEntity.h"
#include"InGameGraphicalObjectLink.h"
#include"GameManager.h"
#include"AnimationCollection.h"

GraphicalEntity::GraphicalEntity( std::shared_ptr<InGameGraphicalObjectTree> tree,EntityClass type, bool isActive )
{
	pTimed = false;
	pTimer = -1;
	pType = type;
	pActive = isActive;
	this->pGEntity = tree;
	initialize();
	//deleteSpriteTree = true;
}

GraphicalEntity::GraphicalEntity( std::shared_ptr<InGameGraphicalObject> obj, EntityClass type, bool isActive )
{
	pTimed = false;
	pTimer = -1;
	pType = type;
	pActive = isActive;
	this->pGEntity.reset( new InGameGraphicalObjectTree(obj) );
	initialize();
}

GraphicalEntity::GraphicalEntity( std::shared_ptr<InGameGraphicalObjectTree> tree,int timer, EntityClass type, bool isActive )
{
	pTimed = true;
	pTimer = timer;
	pType = type;
	if( timer > 0)
		pActive = isActive;
	else 
		pActive = false;
	this->pGEntity = tree;
	initialize();
	//deleteSpriteTree = true;
}

GraphicalEntity::GraphicalEntity( std::shared_ptr<InGameGraphicalObject> obj,int timer, EntityClass type, bool isActive )
{
	pTimed = true;
	pTimer = timer;
	pType = type;
	if( timer > 0)
		pActive = isActive;
	else 
		pActive = false;
	this->pGEntity.reset( new InGameGraphicalObjectTree(obj) );
	initialize();
	//deleteSpriteTree = true;
}

void GraphicalEntity::setTimer(int set)
{
	if (set > 0)
		this->pTimed = true;
	pTimer = set; 
};

bool GraphicalEntity::reduceTimer( double red ) 
{ 
	if( pTimed ) 
	{
		if (pTimer < 0)
		{
			pActive = false;
			return false;
		}
		pTimer -= red;
	}
	return true;
};

bool GraphicalEntity::CoordIsCollision(const Coord& pos)
{
	return this->getGraphicalObject()->coordinateIscollision(pos);
}

bool GraphicalEntity::AreaIsCollision(const SDL_Rect& rect)
{
	return this->getGraphicalObject()->areaIsCollision(rect);
}

std::shared_ptr<GraphicalEntity> GraphicalEntity::copy()
{
	std::shared_ptr<InGameGraphicalObjectTree> newObject = this->getGraphicalObject()->copy();
	std::shared_ptr<GraphicalEntity> result = std::shared_ptr<GraphicalEntity>(new GraphicalEntity( newObject, this->getTimer(), EntityClass::PROJECTILE, this->isActive() ));
	return  result;
}

Coord GraphicalEntity::transformPositionToObjectFrame( const Coord& Coord)
{
	return this->pGEntity.get()->transformPositionToObjectFrame( Coord );
}

void GraphicalEntity::rotateRelativeHeadingClockwise( double rad )
{
	double newAngle = this->getRelativeHeading() - rad;
	this->setRelativeHeading( newAngle );
}

void GraphicalEntity::setRelativeHeading( double set )
{
	this->pGEntity.get()->getMainObject()->SetRelativeHeading(set);
}

double GraphicalEntity::getRelativeHeading()
{
	return this->pGEntity.get()->getMainObject()->GetRelativeHeading();
}

void GraphicalEntity::setGlobalHeading( double set )
{ 
	this->pGEntity.get()->setGlobalHeading( set );
}

void GraphicalEntity::setUnitHeading( double set )
{ 
	this->pGEntity.get()->setUnitHeading( set );
}

void GraphicalEntity::disable()
{ 
	pTimer = -1; 
	pActive = false;
};

int GraphicalEntity::getSize()
  {
	return this->getDimension().GetSize();
}

bool GraphicalEntity::DrawHitBoxes() const
{
	//currently not in use
	return this->pDrawHitBoxes;
}
void GraphicalEntity::SetDrawHitBoxes(bool set)
{
	this->pDrawHitBoxes = set;
}

void GraphicalEntity::OffsetHitBoxes( const Coord& offset)
{
	this->getGraphicalObject()->OffsetHitBoxes(offset);
}

bool GraphicalEntity::HasValidHitBoxes()
{
	return this->getGraphicalObject()->HasValidHitBoxes();
}

InGameGraphicalObjectLink& GraphicalEntity::addGraphicalObject( std::shared_ptr<InGameGraphicalObject> object, const Coord& relative_pos, const Coord& anchorPoint, double relationalheading )
{
	return pGEntity->addNode( object, relative_pos, anchorPoint,relationalheading, this->getGraphicalObject()->GetNextAvailableLinkID() );
}

InGameGraphicalObjectLink& GraphicalEntity::addGraphicalObject(std::shared_ptr<InGameGraphicalObject> object, const Coord& relative_pos, const Coord& anchorPoint, double relationalheading, int ID)
{
	return pGEntity->addNode(object, relative_pos, anchorPoint, relationalheading, ID);
}

InGameGraphicalObjectLink& GraphicalEntity::addGraphicalObject( std::shared_ptr<InGameGraphicalObjectTree> object, const Coord& relative_pos, const Coord& anchorPoint, double relationalheading)
{
	if( object->getTreeNodesAmount() == 1 )
		return addGraphicalObject( object->getMainObject(), relative_pos, anchorPoint, relationalheading, this->getGraphicalObject()->GetNextAvailableLinkID() );
	else
	{
		throw std::exception( "Not implemented" );
	}
}

InGameGraphicalObjectLink& GraphicalEntity::addGraphicalObject( std::shared_ptr<GraphicalEntity> object, const Coord& relative_pos, const Coord& anchorPoint, double relationalheading)
{
	return this->addGraphicalObject( object->getGraphicalObject(), relative_pos, anchorPoint, relationalheading  );
}

void GraphicalEntity::SetDrawAfterParent(bool set)
{
	this->getGraphicalObject()->getMainObject()->SetDrawAfterParent(set);
}

bool GraphicalEntity::DrawAfterParent()
{
	return this->getGraphicalObject()->getMainObject()->DrawAfterParent();
}

void GraphicalEntity::SetDrawGloballyLast(bool set)
{
	this->getGraphicalObject()->getMainObject()->SetDrawGloballyLast(set);
}

void GraphicalEntity::TriggerDestroyAnimation( enHitAnimation type, int animationInterval, bool DontOverrideCurrentAnimation)
{
	if (DontOverrideCurrentAnimation && pNextAnimationTime > 0)
		return;

	pNextAnimationTime = this->pTimer;
	pAnimationInterval = animationInterval;
	this->pDestroyAnimation = type;
}

void GraphicalEntity::HandleDestroyAnimation( const Coord entityPosition )
{
	if (this->pTimed && pNextAnimationTime > 0)
	{
		if (pNextAnimationTime > this->pTimer)
		{
			pNextAnimationTime = this->pTimer - pAnimationInterval;
			AnimationContainer& animationContainer = this->getGraphicalObject()->GetRandomLink()->GObject()->GetAnimationContainer();
			animationContainer.AddAnimation(FrameStripCollection::GetCollection(enHitAnimation::SHIPDESTRUCTION).GetRandomStrip(Coord(enHitAnimation::SHIPDESTRUCTION/2), false, true));	//slight hack, but the positioning is different here, than when using Animation::(..)
			animationContainer.SetAnimationDrawing(true);
		}
	}
}

void GraphicalEntity::initialize()
{
	pDrawHitBoxes = false;
	pNextAnimationTime = 0;
	pAnimationInterval = 0;
	pDestroyAnimation = enHitAnimation::NOANIMATION;
}

//std::map<Coord,int> GraphicalEntity::GetAndClearAccummulatedDamage()
//{
//	std::map<Coord,int> result = this->pGEntity->refGetMainFigure().getFigure(0).GetAccumulatedDamage();
//	this->pGEntity->refGetMainFigure().getFigure(0).ClearAccumulatedDamageMap();
//	return result;
//}
//void GraphicalEntity::ClearAccumulatedDamage()
//{
//	this->getMainFigure().cGetFigure(0).ClearAccumulatedDamageMap();
//}

//InGameGraphicalObjectLink& GraphicalEntity::addGraphicalObject( std::shared_ptr<GraphicalEntity> object, const Coord& relative_pos, const Coord& anchorPoint, bool intersectFigure )
//{
//	if( intersectFigure )
//	{
//		this->pGEntity.get()->refGetMainFigure().AddFigure( object.get()->getMainFigure().CopyFigure(0) );
//	}
//	else
//		return this->addGraphicalObject( object-, relative_pos, anchorPoint );
//}

