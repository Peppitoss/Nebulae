#pragma once
#include"GenericEntity.h"
#include"BoundedDouble.h"
#include"Kinematics.h"

enum enStepExecution
{
	aniPosition = 1,
	aniTime = 2
};

class EntityStepLocomotion
{

public:

	EntityStepLocomotion();
	EntityStepLocomotion(int speed, enStepExecution type);

	bool ExecuteLocomotion(int milliseconds, const primitives::vector& positionChange);
	void ResetCounter();
	void SetCounter(int set) { this->pCounter = set; };
	void SetSpeed(int set) { this->pSpeed = set; };
	void IncrementCounter(int increment) { this->pCounter += increment; };
	int GetCounter() const { return this->pCounter; };
	bool CanExecute() const;

	void IncreaseFrame();
	std::vector<Coord>& GetFrames() { return pStepFrames; };
	int GetFrameCount() const { return this->pStepFrames.size(); }
	Coord GetStep(int step) const { return this->pStepFrames[step]; };
	Coord GetCurrentStep() const { return this->pStepFrames[CurrentStep()]; };
	int GetSpeed() const { return this->pSpeed; }
	int CurrentStep() const { return this->pCurrentStep; }
	enStepExecution GetType() const { return this->pType; }
	Coord GetLastChange() const { return this->pLastChange; }

private:

	std::vector<Coord> pStepFrames;
	Coord pLastChange;
	int pSpeed;
	int pCounter;
	int pCurrentStep;
	enStepExecution pType;
};

class DynamicEntity : public GenericEntity
{
public:

    static const int MAXVEL = 10;
	static const int PIXELPRMETER = 1;

	DynamicEntity();
	DynamicEntity( Coord initpos );
	DynamicEntity( std::shared_ptr<GraphicalEntity> );

	primitives::vector getVelocity() const { return pVelocity; };
	double getAngularVelocity() const { return pAngularVelocity.value(); };
	double getAngularAcceleration() const { return pAngularAcceleration; };

	//mass (can be zero)
	void RevertLastChangeInPosition();
	//void setPosition( Coord );
	void IncreaseVelocity(const primitives::vector& amount) { pVelocity += amount; };
	void setVelocity( const primitives::vector& set ) { pVelocity = set; };
	void SetPosition(const Coord& set) override;
	void setAngularVelocity(double set) { this->pAngularVelocity = set; };
	void SetKinematics(Kinematics kinematics);
	
	void setTexture(std::shared_ptr<GraphicalEntity>) override;
	virtual primitives::vector ChangeInPosition(const primitives::vector& velocity, int milliseconds);

	//

	void resetState();
	virtual void updateVelocity(Uint32 millisecond) = 0;
	virtual void move( Uint32 milliseconds ) = 0;
	virtual void setImpulseForce( primitives::vector set ) { pImpulseForce = set; };
	virtual void resetImpulseForce();
	virtual void setImpulseAngularForce(double set) { this->pImpulseAngularForce = set; };
	virtual double getImpulseAngularForce() { return this->pImpulseAngularForce; };
	virtual primitives::vector GetImpulseForce() { return pImpulseForce; };	//in KiloNewton

	virtual void updateHeading(Uint32 milliseconds);
	void setEntityDead();
	virtual void handleEvent( SDL_Event & e, const Coord& cameraPos );
	primitives::vector GetUnitFrameVelocity() { return pUnitFrameVelocity; };

	bool reqCollisionDetect;
	bool reqRefresh;

	long GetSimpleMass() const { return this->pSimpleMass; };
	long GetSimplifiedMomentOfInertia() const;
	void SetSimpleMass(long set);
	void IncrementSimpleMass(long incremet);
	void DecrementSimpleMass(long incremet);

	void SetSimpleMomentOfInertia(long set);
	void IncrementMomentOfInertia( long mass, double radius);
	void DecrementMomentOfInertia( long mass, double radius);

	primitives::vector GetLastChangeInPosition() const { return this->pLastChangeInPosition; };
	primitives::TargetState GetStateAsTarget( bool ForwardLooking ) const;

	int GetCachedSize() const { return pCacheSize; }

	std::shared_ptr<InGameGraphicalObject> MainObjectPtr() { return ptrMainObject; }

protected:

	//double pHeading;		//in radians
	primitives::vector pImpulseForce;
	double pImpulseAngularForce;

	double pAngularAcceleration;
	BoundedDouble pAngularVelocity;

	primitives::vector pAcceleration;
	primitives::vector pVelocity;
	primitives::vector pUnitFrameVelocity;
	primitives::vector pLastChangeInPosition;

	double realX;
	double realY;

	long pSimpleMass;
	long pSimpleMomentOfInertia;

	std::shared_ptr<InGameGraphicalObject> ptrMainObject;
	void UpdateCacheSize() { pCacheSize = this->getGraphics()->getSize(); }

private:
	
	int pCacheSize;

};
