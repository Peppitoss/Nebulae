#pragma once
#include"DynamicEntityControlled.h"

class DockingEntity : public DynamicEntityControlled
{
public:

	DockingEntity();
	DockingEntity(const Coord pos);

	//void setRelativePosition( std::shared_ptr<Coord> pos );
	//Coord getRelativePosition() const { return *pRelativePosition; };
	//oid setGlobalPosition( Coord pos ) { this->pCenterPosition = pos; };

	//double GetVelocitySetPointAbsValue() override { return 0; };

	void SetIsDocked( bool set) { isDocked = set; };
	bool IsDocked() { return isDocked; };

	primitives::vector GetImpulseForce() override; //impulse force should be redistributed to the connected component

private:

	double dockStrength; //?? use this to determine whether the docked object is disconnected?? use this in links instead?? :)

	bool isDocked; // is used?? replaced by rotating launcher entity module..
	void initialize();
	//std::shared_ptr<Coord> pRelativePosition;

};

