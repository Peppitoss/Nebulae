#include"Doodad.h"

Doodad::Doodad() : DynamicEntity()
{
}

Doodad::Doodad( Coord initpos ) : DynamicEntity(initpos)
{
}

Doodad::Doodad(std::shared_ptr<GraphicalEntity> ge) : DynamicEntity(ge)
{
}
void Doodad::handleEvent( SDL_Event & e , Coord cameraPosition) 
{
	DynamicEntity::handleEvent(e,cameraPosition);
}

void  Doodad::move(Uint32 millisec )
{
	this->particleTriggerTime += millisec;
    DynamicEntity::move(millisec);
}
void  Doodad::updateVelocity(Uint32 millisec)
{
	DynamicEntity::updateVelocity(millisec);
}
