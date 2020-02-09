#pragma once
#include"FiguredGraphics.h"
#include"SpriteObject.h"
#include"DynamicEntity.h"

typedef std::shared_ptr<FiguredGraphics> FGPointer;  // GO - Graphical Object

enum enRadarSlideState
{

	enSliderDefault = 0,
	enSliderTarget

};

class RadarScreenSlider : public DynamicEntity
{

public:

	RadarScreenSlider();

	void SetTarget(std::shared_ptr<GenericEntity> target );
	std::shared_ptr<GenericEntity> GetTargetReference();

	double GetNoiseLevel();
	void SetNoiseLevel( double set );

	void updateVelocity(Uint32 millisecond) override;
	void move( Uint32 milliseconds ) override;

	bool IsActive();
	void Deactivate();
	void Activate();
	
	void RefreshState();

	void SetVesselRadarPosition( const Coord& );
	Coord GetVesselRadarPosition() const { return this->pVesselRadarPosition; };

	void SetState( enRadarSlideState set );
	enRadarSlideState GetState() { return pState; };

private:

	bool IsOnTop();
	bool IsOnBottom();
	bool IsLeft();
	bool IsRight();

	void MoveAlongBottom( bool clockwise, Uint32 milliseconds );	//true = clockwise
	void MoveAlongTop( bool clockwise, Uint32 milliseconds );
	void MoveAlongLeft( bool clockwise, Uint32 milliseconds );
	void MoveAlongRight( bool clockwise, Uint32 milliseconds );

	double DistanceToTarget();
	Coord pVesselRadarPosition;
	Coord pTopInitPosition;
	Coord pBottomRightPosition;

	Coord TargetPosition();

	//void Moveclockwise();
	//void Moveanticlockwise();
	void SlideMove( bool clockwise, Uint32 milliseconds );
	std::shared_ptr<GenericEntity> pTargetReference;
	double pNoiseLevel;
	double pErrorMargin;
	double pVelocityValue;

	double pAccumulatedMovement; //tto counter if velocity levels are so low, the movement change never reaches an integer (pixel)
	enRadarSlideState pState;

};