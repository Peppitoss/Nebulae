#include"Particles.h"
#include"PointColorGraphicalObject.h"
#include"ParticleColorDefinitions.h"


Particle::Particle()
{
	this->reqCollisionDetect = false;
	 particleColor = ColorRGB(255,255,255);
	pLaunchTime = 0;
}

Particle::Particle( ColorRGB color) : DynamicEntity()
{
	this->pGraphics = std::shared_ptr<GraphicalEntity>( new GraphicalEntity( std::shared_ptr<PointColorGraphicalObject>(new PointColorGraphicalObject( color, spriteSize(2) )),1, EntityClass::PARTICLE, true ) );
	this->reqCollisionDetect = false;
	particleColor = color;
	pLaunchTime = 0;
	this->ptrMainObject = this->pGraphics->getGraphicalObject()->getMainObject();
}

void Particle::move( Uint32 ms )
{
	DynamicEntity::move( ms );
	if (this->HasUpdateFunction())
	{
		this->pUpdateFunction(this);
		((PointColorGraphicalObject*)this->getGraphics()->getGraphicalObject()->getMainObject().get())->colorSet(Coord(0), this->particleColor);
	}
}

Particle* Particle::copyParticle()
{	
	//std::shared_ptr<GraphicalEntity> newEntity = this->getGraphics()->copy();
	Particle* result = new Particle(particleColor);
	//result->pCenterPosition = this->pCenterPosition;
	result->pPosition = this->pPosition;
	result->pVelocity = this->pVelocity;
	//result->ptrMainObject = newEntity->getGraphicalObject()->pMainObject();
	return result;
}

Particle* Particle::copyParticleBasicAttributes()
{
	Particle* result = new Particle(ColorRGB (255,255,255));
	//result->pCenterPosition = this->pCenterPosition;
	result->pPosition = this->pPosition;
	result->pVelocity = this->pVelocity;
	return result;
}

void Particle::updateVelocity( Uint32 milliseconds )
{
	DynamicEntity::updateVelocity( milliseconds );
}

void Particle::SetParticleColor(const ColorRGB& col)
{ 
	this->particleColor = col;
	this->ptrMainObject->colorModulate(col);
}

//void Particle::SetParticleColor(const TimeVariabel<ColorRGB>& col)
//{
//	this->particleColor = col;
//	this->ptrMainObject->colorModulate(col.GetVariabel());
//}
