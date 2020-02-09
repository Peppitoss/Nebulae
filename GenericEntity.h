#pragma once
#include"GraphicalEntity.h"
#include"LinkObject.h"
#include<vector>
#include<functional>

//using enObjectDiscreteRotation;
//using enObjectFlipped;

class GenericEntity
{

public:

	GenericEntity();
	GenericEntity( Coord pos);
	GenericEntity( std::shared_ptr<GraphicalEntity> );

	//std::map<Coord,int> TransferAccumulatedDamage(); //clears the accumulated damage in this function

	Coord GetFigureCenter() const { return this->getDimension().Center().yflip();	}
	Coord CenterPosition() const;
	virtual Coord getPosition() const;
	virtual void SetPosition(const Coord& pos );
	std::shared_ptr<GraphicalEntity> getGraphics() const { return pGraphics; };
	double getGlobalHeading() const { return pGraphics->GetGlobalHeading(); };
	double getUnitHeading() const { return pGraphics->GetUnitHeading(); };

	virtual void ApplyDamage( const Coord &, const game::Damage& );
	game::Hull getHull();
	void SetHullHitPoints(game::enHitPointType, int amount, bool valueIsNewMax );
	//game::HitPoints getHitPoint( const Coord &, CollisionResult colResult);
	//game::Hull getInitHullAtPosition( const Coord & );

	int getSize() const;
	spriteSize getDimension() const;
	int getMaxHullPoints() const;
	virtual void AddDamageRecieved(const game::Damage& set );
	//virtual void SetDamageRecieved(const game::Damage& set );
	int getDamageRecieved() const;
	double getPercentHullLeft();
	int getHitPointLeft();
	string GetHitPointForTooltip();

	virtual bool isDestroyed();
	virtual void DestroyEntity();
	virtual void setEntityDead() {};
	virtual void setTexture( std::shared_ptr<GraphicalEntity> );
	virtual void handleEvent( SDL_Event & e, const Coord& cameraPos );
	//bool deleteGraphicalEntity;

	//bool hasLink() const { return pTextLink.size()>0  && pTextLink[0].link != nullptr; };
	//std::shared_ptr<SurfaceButton> getLink(int i) const { return pTextLink[i].link; };
	//std::shared_ptr<SurfaceButton> getLink(string id) const;
	//std::shared_ptr<Link> getLink(string type) const;
	int LinkAmount() const { return GetActiveLinks().size(); };

	//void generateSingleLink( Coord relativePosition, std::string text );
	//void generateNewLink( const Coord& relativePosition, std::string text, enLinkType type );
	void generateNewLink( const Coord& relativePosition, std::string text, const Coord& size, enLinkType type, std::string callbackname);
	void generateNewLink(LinkObject& );
	void ClearLinkMemory();

	bool IDIsSet();
	int GetID() const { return this->pID; };
	void SetID( int set ); 

	//void setLinkCallback( void (*callback_arg)(SDL_Event & event, Widget*) ) { linkCallback = callback_arg; };

	virtual void RotateClockwise( double rad );
	virtual void Rotate( enObjectDiscreteRotation );
	virtual void setGlobalHeading( double newHeading );
	virtual void setUnitHeading( double newHeading );
	virtual void FlipGraphics();

	void GenerateTargetLink();

	~GenericEntity();

	virtual void FreeMemory();

	void IterateActiveLinks(std::function<void(LinkAttachment&)>) const;
	virtual std::vector<LinkAttachment> GetActiveLinks() const;

	LinkAttachment LinkTarget;

protected:

	std::shared_ptr<GraphicalEntity> pGraphics;
	Coord pPosition;
	//Coord pCenterPosition; //center of mass???
	game::Hull pHull;


	//
	//std::vector<LinkAttachment> pTextLink;
	int pID;

	//void (*linkCallback)(SDL_Event & event, Widget*);

	virtual void HandleSingleHit( const Coord &, const game::Damage& );
	virtual void HandleMultipleHit( CoordWithCollisionData result[1024], int size );
	//virtual void HandleAOEHit( const Coord &, const game::Damage& );

private:

	void RefreshLinkPosition(const Coord& );
	void Initialize();

	void ClearTextLinks();
	bool pDestroyed;
	
};
