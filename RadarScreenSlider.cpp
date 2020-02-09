#include"RadarScreenSlider.h"
#include"Globals.h"
#include"HUDGraphicalCollection.h"
#include"graphics.h"

typedef std::shared_ptr<SpriteObject> SOPointer;  // GO - Graphical Object

RadarScreenSlider::RadarScreenSlider() : DynamicEntity()
{
	realX = 0;
	realY = 0;
	reqRefresh = false;
	reqCollisionDetect = false;
	pVelocityValue = 1;
	pVelocity = primitives::vector(pVelocityValue,pVelocityValue);
	pAcceleration = primitives::vector(0,0);
	pAngularVelocity = 0;
	pAngularAcceleration = 0;
	pAccumulatedMovement = 0;
	pErrorMargin = 0.0001;// 0.0001 is quite low, but this is tested with a variable pVelocity, a more appropriate value could be f.eks pVelocity/10
	resetImpulseForce();
	SOPointer slider = SOPointer( new SpriteObject( std::shared_ptr<Sprite>(new Sprite( HUDCollection::ImportHUD_Slidearrows.get())), false, false));
	this->setTexture( std::shared_ptr<GraphicalEntity>(new GraphicalEntity(  slider, EntityClass::HUDGRAPHICS, true ) ));
	this->pTopInitPosition = Coord( -DEFAULT_SCREEN_WIDTH/2 + 32, -DEFAULT_SCREEN_HEIGHT/2 + 0);
	this->pBottomRightPosition = Coord( DEFAULT_SCREEN_WIDTH/2 - 0, DEFAULT_SCREEN_HEIGHT/2 + -32 );
	this->pPosition = pTopInitPosition + Coord(1,0);
	this->setGlobalHeading( -std::_Pi/2 );
	SetState(enRadarSlideState::enSliderDefault);
	this->Deactivate();
}

void RadarScreenSlider::SetState( enRadarSlideState set )
{
	this->pState = set;
	if( this->pState == enRadarSlideState::enSliderTarget )
	{
		this->pGraphics->getGraphicalObject().get()->getMainObject()->colorModulate( ColorRGB(255,255,255,255) );
	}
	else
		this->pGraphics->getGraphicalObject().get()->getMainObject()->colorModulate( ColorRGB(100,100,100,255) );
	this->pGraphics->getGraphicalObject().get()->getMainObject()->refreshTexture();
}

bool RadarScreenSlider::IsActive()
{
	return this->pGraphics->isActive();
}

void RadarScreenSlider::Deactivate()
{
	this->pGraphics->disable();
}

void RadarScreenSlider::RefreshState()
{
	if (this->GetTargetReference()->isDestroyed())
		this->Deactivate();
}

void RadarScreenSlider::Activate()
{
	this->pGraphics->activate();
}

double RadarScreenSlider::DistanceToTarget()
{
	return ( this->TargetPosition() - this->getPosition() ).SumXY();
}

void RadarScreenSlider::updateVelocity(Uint32 millisecond)
{

}
void RadarScreenSlider::move( Uint32 milliseconds )
{
	if( !this->IsActive() )
		return;

	double vesselTargetAngle = geometry::vectorHeading( this->TargetPosition() - this->pVesselRadarPosition );
	double sliderVesselAngle = geometry::vectorHeading( this->getPosition() );
	double error = vesselTargetAngle - sliderVesselAngle;
	//  pVelocity is meant to be constant, but testing here a kind og P regulated version, 
	pVelocity = primitives::vector(abs(error/10),abs(error/10));
	double errorMargin = pErrorMargin;
	 if( error> errorMargin )
		 SlideMove(error<std::_Pi,milliseconds);
	else if( error < -errorMargin )
		SlideMove(error<-std::_Pi,milliseconds);
}

bool RadarScreenSlider::IsOnTop()
{
	return this->getPosition().y <= this->pTopInitPosition.y;
}
bool RadarScreenSlider::IsOnBottom()
{
	return this->getPosition().y >= pBottomRightPosition.y;
}
bool RadarScreenSlider::IsLeft()
{
	return this->getPosition().x <= this->pTopInitPosition.x;
}
bool RadarScreenSlider::IsRight()
{
	return this->getPosition().x >= pBottomRightPosition.x;
}

void RadarScreenSlider::SlideMove( bool clockwise, Uint32 milliseconds )
{
	if( this->IsOnTop() )
	{
		MoveAlongTop(clockwise, milliseconds);
	}
	else if( this->IsOnBottom() )
	{
		MoveAlongBottom(clockwise, milliseconds);
	}
	else if( this->IsLeft() )
	{
		MoveAlongLeft(clockwise, milliseconds);
	}
	else if( this->IsRight() )
	{
		MoveAlongRight(clockwise, milliseconds);
	}
}

//
//void RadarScreenSlider::Moveanticlockwise()
//{
//
//}
//
//void RadarScreenSlider::Moveclockwise()
//{
//	
//}
//
void RadarScreenSlider::MoveAlongBottom(bool clockwise, Uint32 milliseconds )
{

	//BE seperate function------------------------------------------------------------------------
	double traverseLenght = pVelocity.x*milliseconds;
	int naturalTraverseLenght = (int)traverseLenght;
	
	pAccumulatedMovement += (traverseLenght-naturalTraverseLenght);
	int naturalAccumulatedValue = (int)pAccumulatedMovement;

	pAccumulatedMovement -= naturalAccumulatedValue;

	traverseLenght = naturalTraverseLenght + naturalAccumulatedValue;

	//------------------------------------------------------------------------

	if( clockwise )
	{
		this->pPosition.x -= traverseLenght;
		if(this->IsLeft())
		{
			this->pPosition.y -= pVelocity.y;
			this->pPosition.x = this->pTopInitPosition.x;
			this->pGraphics->getGraphicalObject().get()->getMainObject()->setGlobalHeading( std::_Pi );
		}
	}
	else if( !clockwise )
	{
		this->pPosition.x += traverseLenght;
		if(this->IsRight())
		{
			this->pPosition.y -= pVelocity.y;
			this->pPosition.x = pBottomRightPosition.x;
			this->pGraphics->getGraphicalObject().get()->getMainObject()->setGlobalHeading( 0 );
		}
	}
}
void RadarScreenSlider::MoveAlongTop(bool clockwise, Uint32 milliseconds )
{
	//BE seperate function------------------------------------------------------------------------
	double traverseLenght = pVelocity.x*milliseconds;
	int naturalTraverseLenght = (int)traverseLenght;
	
	pAccumulatedMovement += (traverseLenght-naturalTraverseLenght);
	int naturalAccumulatedValue = (int)pAccumulatedMovement;

	pAccumulatedMovement -= naturalAccumulatedValue;

	traverseLenght = naturalTraverseLenght + naturalAccumulatedValue;
	//------------------------------------------------------------------------

	if( !clockwise )
	{
		this->pPosition.x -= traverseLenght;
		if( this->IsLeft() )
		{
			this->pPosition.y += pVelocity.y;
			this->pPosition.x = this->pTopInitPosition.x;
			this->pGraphics->getGraphicalObject().get()->getMainObject()->setGlobalHeading( std::_Pi );
		}
	}
	else if( clockwise )
	{
		this->pPosition.x += traverseLenght;
		if(this->IsRight())
		{
			this->pPosition.y += pVelocity.y;
			this->pPosition.x = pBottomRightPosition.x;
			this->pGraphics->getGraphicalObject().get()->getMainObject()->setGlobalHeading( 0 );
		}
	}
}

void RadarScreenSlider::MoveAlongLeft(bool clockwise, Uint32 milliseconds )
{
	//BE seperate function------------------------------------------------------------------------
	double traverseLenght = pVelocity.y*milliseconds;
	int naturalTraverseLenght = (int)traverseLenght;
	
	pAccumulatedMovement += (traverseLenght-naturalTraverseLenght);
	int naturalAccumulatedValue = (int)pAccumulatedMovement;

	pAccumulatedMovement -= naturalAccumulatedValue;

	traverseLenght = naturalTraverseLenght + naturalAccumulatedValue;
	//------------------------------------------------------------------------

	if( !clockwise )
	{
		this->pPosition.y += traverseLenght;
		if(this->IsOnBottom())
		{
			this->pPosition.x += pVelocity.x;
			this->pPosition.y = pBottomRightPosition.y;
			this->pGraphics->getGraphicalObject().get()->getMainObject()->setGlobalHeading( std::_Pi/2 );
		}
	}
	else if( clockwise )
	{
		this->pPosition.y -= traverseLenght;
		if(this->IsOnTop())
		{
			this->pPosition.x += pVelocity.x;
			this->pPosition.y = pTopInitPosition.y;
			this->pGraphics->getGraphicalObject().get()->getMainObject()->setGlobalHeading( -std::_Pi/2 );
		}
	}
}
void RadarScreenSlider::MoveAlongRight(bool clockwise, Uint32 milliseconds )
{
	//BE seperate function------------------------------------------------------------------------
	double traverseLenght = pVelocity.y*milliseconds;
	int naturalTraverseLenght = (int)traverseLenght;
	
	pAccumulatedMovement += (traverseLenght-naturalTraverseLenght);
	int naturalAccumulatedValue = (int)pAccumulatedMovement;

	pAccumulatedMovement -= naturalAccumulatedValue;

	traverseLenght = naturalTraverseLenght + naturalAccumulatedValue;
	//------------------------------------------------------------------------

	if( clockwise )
	{
		this->pPosition.y += traverseLenght;
		if(this->IsOnBottom())
		{
			this->pPosition.x -= pVelocity.x;
			this->pPosition.y = pBottomRightPosition.y;
			this->pGraphics->getGraphicalObject().get()->getMainObject()->setGlobalHeading( std::_Pi/2 );
		}
	}
	else if( !clockwise )
	{
		this->pPosition.y -= traverseLenght;
		if(this->IsOnTop())
		{
			this->pPosition.x -= pVelocity.x;
			this->pPosition.y = pTopInitPosition.y;
			this->pGraphics->getGraphicalObject().get()->getMainObject()->setGlobalHeading( -std::_Pi/2 );
		}
	}
}

void RadarScreenSlider::SetVesselRadarPosition( const Coord& pos)
{
	this->pVesselRadarPosition = pos;
}

double RadarScreenSlider::GetNoiseLevel()
{ 
	return this->pNoiseLevel;
}
void RadarScreenSlider::SetNoiseLevel( double set ) 
{ 
	this->pNoiseLevel = set; 
}

void RadarScreenSlider::SetTarget(std::shared_ptr<GenericEntity> target )
{
	this->pTargetReference = target;
	this->Activate();
}

std::shared_ptr<GenericEntity> RadarScreenSlider::GetTargetReference()
{
	if(this->pTargetReference.get())
		return this->pTargetReference;
	else
	{
		this->Deactivate();
		return nullptr;
	}
}

Coord RadarScreenSlider::TargetPosition()
{
	if (GetTargetReference() != nullptr)
		return GetTargetReference()->getPosition();
	return Coord(0);
}