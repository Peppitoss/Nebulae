#include"DockingEntity.h"


DockingEntity::DockingEntity() :  DynamicEntityControlled()
{

	initialize();

}

DockingEntity::DockingEntity( const Coord pos ) :  DynamicEntityControlled(pos)
{

	initialize();

}

void DockingEntity::initialize()
{
	this->isDocked = true;
}

primitives::vector DockingEntity::GetImpulseForce() 
{ 
	if( IsDocked() )
		return primitives::vector(0); 
	else
		return DynamicEntity::GetImpulseForce();
}; 
