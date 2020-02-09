#pragma once
#include"DynamicEntity.h"


class Doodad : public DynamicEntity
{

public:

	Doodad();
	Doodad( Coord initpos );
	Doodad( std::shared_ptr<GraphicalEntity> );

	void move(Uint32);
	void updateVelocity(Uint32);
	virtual void handleEvent( SDL_Event & e, Coord cameraPos );

protected:

	int particleTriggerTime;
};