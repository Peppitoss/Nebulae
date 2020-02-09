#pragma once
#include"Doodad.h"
#include"ParticleEngine.h"




class Debri : public Doodad
{

public:



	Debri();
	Debri( Coord initpos );
	Debri( std::shared_ptr<GraphicalEntity> );

	void move(Uint32);
	void updateVelocity(Uint32);
	void SetEffect(EnumParticleEffect set);
	void TriggerParticleEffect();
	void SetParticleEngine(particleEffects::ParticleEngine & set );

	Coord pointOfInterest;
	virtual void handleEvent( SDL_Event & e, const Coord& cameraPos );
	~Debri();

private:

	 //used to special effects such as particle engine
	EnumParticleEffect enParticleEffect;
	bool particleEffectOn;
	void init();

	particleEffects::ParticleEngine* particleEngine;

};

