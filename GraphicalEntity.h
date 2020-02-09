#pragma once
#include"InGameGraphicalObjectTree.h"
#include"InGameGraphicalObject.h"
#include"InGameGraphicalObjectLink.h"
#include"AnimationCollection.h"
//#include"ModuleEntity.h"


enum EntityClass
{
	MAINBODY = 0,
	BODY,
	LAUNCHER,
	PROJECTILE,
	ENGINE,
	RADAR,
	AMMOCONTAINER,
	POWERSUPPLY,
	CONTROLLER,
	VESSEL,
	PLAYER,
	PARTICLE,
	DOODAD,
	ROOM,
	PLATFORM,
	HUDGRAPHICS,
	CHASSIS,
	INTERIOR,
	VESSELEXTERIOR,
	DECORATION,
	DOOR,
	NONE,
};

class GraphicalEntity
{

public:

	GraphicalEntity( std::shared_ptr<InGameGraphicalObject> obj, EntityClass type, bool isactive );
	GraphicalEntity( std::shared_ptr<InGameGraphicalObjectTree> tree, EntityClass type, bool isactive );
	GraphicalEntity( std::shared_ptr<InGameGraphicalObject> tree, int time, EntityClass type, bool isactive );
	GraphicalEntity( std::shared_ptr<InGameGraphicalObjectTree> tree, int time, EntityClass type, bool isactive );

	EntityClass type() const { return pType; };
	Coord getCenter() { return getGraphicalObject()->getCenter(); };

	bool isTimed() const { return pTimed; };
	int getTimer() const { return pTimer; };
	bool reduceTimer( double red );
	void setTimer(int set); // { pTimer = set; };

	bool isActive() const { return pActive; };
	void activate() { pActive = true; };
	void disable();
	int getSize();

	void rotateRelativeHeadingClockwise( double rad );
	void setRelativeHeading( double set );
	double getRelativeHeading();

	void setGlobalHeading( double set );
	void setUnitHeading( double set );
	double GetGlobalHeading() const { return this->pGEntity.get()->getGlobalHeading(); };
	double GetUnitHeading() const { return this->pGEntity.get()->getUnitHeading(); };

	Coord transformPositionToObjectFrame( const Coord& );

	bool DrawHitBoxes() const;
	bool HasValidHitBoxes();
	void SetDrawHitBoxes(bool set);
	void OffsetHitBoxes( const Coord& offset);

	std::vector<Coord> GraphicalEntityToList() { return std::vector<Coord>(); };//{ return this->pGEntity->GraphicalObjectTreeToList(); };
	spriteSize getDimension() const { return this->pGEntity->getSize(); };

	std::shared_ptr<GraphicalEntity> copy();

	InGameGraphicalObjectLink& addGraphicalObject(std::shared_ptr<InGameGraphicalObject>     object, const Coord& relative_pos, const Coord& anchorPoint, double relationalheading );
	InGameGraphicalObjectLink& addGraphicalObject( std::shared_ptr<InGameGraphicalObject>     object, const Coord& relative_pos, const Coord& anchorPoint, double relationalheading, int ID );
	InGameGraphicalObjectLink& addGraphicalObject( std::shared_ptr<InGameGraphicalObjectTree> object, const Coord& relative_pos, const Coord& anchorPoint, double relationalheading );
	InGameGraphicalObjectLink& addGraphicalObject( std::shared_ptr<GraphicalEntity> object, const Coord& relative_pos, const Coord& anchorPoint, double relationalheading );
	//InGameGraphicalObjectLink& addGraphicalObject( std::shared_ptr<GraphicalEntity> object, const Coord& relative_pos, const Coord& anchorPoint, bool intersectFigure );

	bool CoordIsCollision(const Coord& pos);
	bool AreaIsCollision(const SDL_Rect& rect);
	int getConnectedObjectsAmount() { return this->pGEntity->getTreeNodesAmount(); };

	//std::map<Coord,int> GetAndClearAccummulatedDamage();
	virtual std::shared_ptr<InGameGraphicalObjectTree> getGraphicalObject() { return this->pGEntity; };

	void SetDrawAfterParent(bool set);
	bool DrawAfterParent();

	void SetDrawGloballyLast(bool set);

	void HandleDestroyAnimation( const Coord entityPosition );
	void TriggerDestroyAnimation(enHitAnimation animation, int animationInterval, bool dontOverrideCurrentAnimation );

protected:

	std::shared_ptr<InGameGraphicalObjectTree> pGEntity;

private:

	void initialize();

	EntityClass pType;

	//bool deleteSpriteTree;
	bool pActive;
	bool pTimed;
	int pTimer;	//how long until self destruct in milliseconds

	int pAnimationInterval;
	int pNextAnimationTime;
	enHitAnimation pDestroyAnimation;

	bool pDrawHitBoxes;

};

