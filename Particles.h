#pragma once
#include"DynamicEntity.h"
#include"TimeVariabel.h"
#include<functional>

class Particle : public DynamicEntity
{
public:
	Particle();
	Particle( ColorRGB );

	Particle* copyParticle();
	Particle* copyParticleBasicAttributes(); //graphics not included
	void setTimeLeft( int milliseconds ) { this->getGraphics()->setTimer( milliseconds ); };
	void updateVelocity( Uint32 milliseconds );
	void move( Uint32 milliseconds );

	void SetParticleColor(const ColorRGB& col);
	ColorRGB GetParticleColor() { return particleColor;	}

	double GetLaunchTime() const { return this->pLaunchTime; };
	void SetLaunchTime(double set) { this->pLaunchTime = set; };

	bool HasUpdateFunction() { return pUpdateFunction != nullptr; }
	void SetUpdateFunction(std::function<void(Particle*)> callback) { pUpdateFunction = callback; }


private:

	std::function<void(Particle*)> pUpdateFunction;

	double pLaunchTime;	//in seconds
	ColorRGB particleColor;
};
