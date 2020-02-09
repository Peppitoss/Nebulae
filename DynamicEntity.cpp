#include"DynamicEntity.h"


EntityStepLocomotion::EntityStepLocomotion()
{
	this->pSpeed = 0;
	this->pType = enStepExecution::aniTime;
	this->pCounter = 0;
	this->pCurrentStep = 0;
	this->pStepFrames = std::vector<Coord>();
}

EntityStepLocomotion::EntityStepLocomotion(int speed, enStepExecution type)
{
	this->pSpeed = speed;
	this->pType = type;
	this->pCounter = 0;
	this->pCurrentStep = 0;
	this->pStepFrames = std::vector<Coord>();
}

void EntityStepLocomotion::IncreaseFrame()
{
	int lastFrame = this->pCurrentStep;
	(this->pCurrentStep += 1) %= this->GetFrames().size();
	this->pLastChange = pStepFrames[pCurrentStep] - this->pStepFrames[lastFrame];
}

void EntityStepLocomotion::ResetCounter()
{
	pCounter = 0;
}

bool EntityStepLocomotion::ExecuteLocomotion(int milliseconds, const primitives::vector& positionChange)
{
	switch (this->GetType())
	{
	case(enStepExecution::aniPosition):
	{
		primitives::vector changeinPos = positionChange;
		this->IncrementCounter(geometry::dotProduct(changeinPos, changeinPos));
	}
	if (std::pow(this->GetSpeed(), 2) < this->GetCounter())
		return true;
	break;
	case(enStepExecution::aniTime):
		this->IncrementCounter(milliseconds);
		if (this->GetSpeed() < this->GetCounter())
			return true;
		break;
	}
	return false;
}

bool EntityStepLocomotion::CanExecute() const
{
	return this->pSpeed > 0 && this->pStepFrames.size() > 0;
}

DynamicEntity::DynamicEntity() : GenericEntity()
{
 	realX = 0;
	realY = 0;
	reqRefresh = false;
	reqCollisionDetect = true;
	pVelocity = primitives::vector(0,0);
	pAcceleration = primitives::vector(0,0);
	pAngularVelocity = 0;
	pAngularAcceleration = 0;
	pSimpleMass = 20;
	pSimpleMomentOfInertia = 20;
	pLastChangeInPosition = primitives::vector(0);
	pCacheSize = 0;
	resetImpulseForce();
}

DynamicEntity::DynamicEntity( Coord pos ) : GenericEntity(pos)
{
	
	realX = pos.x;
	realY = pos.y;
	reqRefresh = false;
	reqCollisionDetect = true;
	pVelocity = primitives::vector(0,0);
	pAcceleration = primitives::vector(0,0);
	pAngularVelocity = 0;
	pAngularAcceleration = 0;
	pSimpleMass = 20;
	pSimpleMomentOfInertia = 20;
	pLastChangeInPosition = primitives::vector(0);
	pCacheSize = 0;
	resetImpulseForce();
}

DynamicEntity::DynamicEntity( std::shared_ptr<GraphicalEntity> ge ) : GenericEntity(ge)
{
	
	realX = 0;
	realY = 0;
	reqRefresh = false;
	reqCollisionDetect = true;
	pVelocity = primitives::vector(0,0);
	pAcceleration = primitives::vector(0,0);
	pAngularVelocity = 0;
	pAngularAcceleration = 0;
	pSimpleMass = 20;
	pSimpleMomentOfInertia = 20;
	pLastChangeInPosition = primitives::vector(0);
	resetImpulseForce();
	pCacheSize = ge->getSize();
	ptrMainObject = ge->getGraphicalObject()->getMainObject();
}



void DynamicEntity::resetState()
{
	pVelocity = primitives::vector(0,0);
	pAcceleration = primitives::vector(0,0);
	realX = 0;
	realY = 0;
	pLastChangeInPosition = primitives::vector(0);
}

void DynamicEntity::setTexture(std::shared_ptr<GraphicalEntity> ge)
{
	GenericEntity::setTexture(ge);
	pCacheSize = ge->getSize();
}

void DynamicEntity::updateVelocity(Uint32 millisecond)
{
	primitives::vector impulseAcceleration = this->GetImpulseForce()  / (1+this->GetSimpleMass());
	double impulseAngularAcceleration = getImpulseAngularForce() / (1 + this->GetSimplifiedMomentOfInertia());

	pVelocity.x += (pAcceleration.x + impulseAcceleration.x*1000)* (double)millisecond / 1000;
	pVelocity.y += ( pAcceleration.y + impulseAcceleration.y * 1000)*  (double)millisecond/1000;
	pAngularVelocity += ( pAngularAcceleration + impulseAngularAcceleration)* (double)millisecond/1000;

	resetImpulseForce();
}

void DynamicEntity::resetImpulseForce()
{ 
	//if (pImpulseForce.DotProduct() < 10000)
		pImpulseForce = primitives::vector(0);
	//else
	//	pImpulseForce *= 0.98;
	//if (pImpulseAngularForce < 10)
		pImpulseAngularForce = 0;
	//else
	//	pImpulseAngularForce *= 0.98;
}

void DynamicEntity::RevertLastChangeInPosition()
{
	auto change = this->GetLastChangeInPosition();
	realX -= change.x;
	realY -= change.y;
	GenericEntity::SetPosition(Coord((int)realX*PIXELPRMETER, (int)realY*PIXELPRMETER));
	//this->SetPosition(this->getPosition() - this->GetLastChangeInPosition().toCoord() );
}

void DynamicEntity::handleEvent( SDL_Event & e , const Coord& cameraPosition)
{
	GenericEntity::handleEvent(e,cameraPosition);
}

void DynamicEntity::setEntityDead()
{
	pAngularAcceleration = 0;
	pAngularVelocity = 0;
}

void DynamicEntity::IncrementSimpleMass(long increment)
{
	this->SetSimpleMass(this->GetSimpleMass() + increment);
}

void DynamicEntity::DecrementSimpleMass(long decrement)
{
	this->SetSimpleMass(this->GetSimpleMass() - decrement);
}

void DynamicEntity::SetSimpleMass(long set)
{
	if (set < 0)
		this->pSimpleMass = 0;
	else if (set < 500000)
		this->pSimpleMass = set;
	else
		this->pSimpleMass = 500000;
};


void DynamicEntity::SetSimpleMomentOfInertia(long set)
{
	if (set < 0)
		this->pSimpleMomentOfInertia = 0;
	else if(set < 5000000)
		this->pSimpleMomentOfInertia = set;
	else
		this->pSimpleMomentOfInertia = 5000000;
}
void DynamicEntity::IncrementMomentOfInertia( long mass, double radius)
{
	long increment = mass*radius / 16;
	this->SetSimpleMomentOfInertia( (this->GetSimplifiedMomentOfInertia() + increment) );
}
void DynamicEntity::DecrementMomentOfInertia( long mass, double radius)
{
	long decrement = mass*radius / 16;
	this->SetSimpleMomentOfInertia(( this->GetSimplifiedMomentOfInertia() - decrement) );
}

long DynamicEntity::GetSimplifiedMomentOfInertia() const
{
	//double halfrad = this->getSize() / 4;
	//return this->GetSimpleMass()*halfrad*halfrad;	//should calculate per point and store in ship
	return pSimpleMomentOfInertia;
}

void DynamicEntity::SetPosition(const Coord& set)
{ 
	GenericEntity::SetPosition(set); 
	realX = set.x; 
	realY = set.y; 
};

void DynamicEntity::SetKinematics(Kinematics kinematics)
{
	setAngularVelocity(kinematics.CurrentAngularSpeed);
	setVelocity(kinematics.CurrentVelocity);
	setUnitHeading(kinematics.CurrentHeading);
}

primitives::vector DynamicEntity::ChangeInPosition(const primitives::vector& velocity, int milliseconds)
{
	if (velocity.x == 0 && velocity.y == 0)
		return primitives::vector(0,0);

	primitives::vector oldPosition = primitives::vector(realX, realY);
	//velocity in m/s, position in pixels

	realX += velocity.x * (double)milliseconds / 1000; //NEDFrameVelocity
	realY += velocity.y * (double)milliseconds / 1000; //NEDFrameVelocity
	
	GenericEntity::SetPosition(Coord((int)realX*PIXELPRMETER, (int)realY*PIXELPRMETER));
	//IterateActiveLinks([&](LinkAttachment& link) {
	//	link.setAttachmentposition(this->getPosition().yflip());
	//});

	pLastChangeInPosition = primitives::vector(realX - oldPosition.x, realY - oldPosition.y);
	return pLastChangeInPosition;
}

void DynamicEntity::move(Uint32 millisecond)
{
	primitives::vector changeInPosition = ChangeInPosition(this->pVelocity, millisecond);
	updateHeading(millisecond);
}

void DynamicEntity::updateHeading(Uint32 milliseconds)
{
	if (std::abs(pAngularVelocity.value()) > 0.00001)
	{
		double newHeading = this->getUnitHeading() + pAngularVelocity*(double)milliseconds / 1000;
		this->setUnitHeading(geometry::radLimit2PI(newHeading));
	}
}

primitives::TargetState DynamicEntity::GetStateAsTarget(bool ForwardLooking) const
{
	if(ForwardLooking)
		return primitives::TargetState(this->CenterPosition() + this->getVelocity().toCoord(), this->getVelocity(), this->getUnitHeading());
	return primitives::TargetState(this->CenterPosition(), this->getVelocity(), this->getUnitHeading());
}

