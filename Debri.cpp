#include"Debri.h"


Debri::Debri() : Doodad()
{
	this->init();
}
Debri::Debri( Coord initpos ) : Doodad(initpos)
{
	this->init();
}
Debri::Debri( std::shared_ptr<GraphicalEntity> ge )  : Doodad(ge)
{
	this->init();
}

void Debri::init()
{
	this->reqCollisionDetect = false;
	this->particleEngine = NULL;
	this->particleEffectOn = true;
	this->enParticleEffect = EnumParticleEffect::particledustspread;
	this->particleTriggerTime = 0;
}

void  Debri::move(Uint32 millisec )
{
	Doodad::move(millisec);
}
void  Debri::updateVelocity(Uint32 millisec)
{
	Doodad::updateVelocity(millisec);
}

void Debri::handleEvent( SDL_Event & e, const Coord& cameraPosition )
{
	Doodad::handleEvent(e,cameraPosition);
}

void Debri::TriggerParticleEffect()
{
	if( this->enParticleEffect == EnumParticleEffect::particledustspread)
	{
         if( ( this->particleTriggerTime - 1000 ) > 0  )
		{
			this->particleTriggerTime = 0;
			this->particleEngine->AddNewEffect( this->particleEngine->GetEffectDustSpread( this->getPosition() + pointOfInterest, 0.2, 2, 0, 2500 ));
		}
	}
}

void Debri::SetEffect(EnumParticleEffect set)
{
	enParticleEffect = set;
}

void Debri::SetParticleEngine(particleEffects::ParticleEngine & set ) 
{ 
	particleEngine = &set; 
};

Debri::~Debri()
{
}