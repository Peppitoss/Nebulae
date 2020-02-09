#include"GenericEntity.h"
#include"Statistics.h"
#include"LinkObject.h"
#include"StringBuilder.h"

#include"ScreenButton.h"
#include"StyleFactory.h"

GenericEntity::GenericEntity()
{
	pGraphics = NULL;
	//this->pCenterPosition = 
	pPosition = Coord(0);
	//pMaxHullPoints = 0;
	//pDamageRecieved = 0;
	Initialize();
}

GenericEntity::GenericEntity( Coord pos)
{
	//this->pCenterPosition = Coord(0);
	this->pPosition = pos;
	//this->pMaxHullPoints = 0;
	//pDamageRecieved = 0;
	
	//linkCallback = NULL;
	Initialize();
}

GenericEntity::GenericEntity( std::shared_ptr<GraphicalEntity> ge )
{
	pGraphics = ge;
	//this->pCenterPosition = Coord(0);
	//this->pMaxHullPoints = pGraphics->getGraphicalObject()->getTotalHitPoints();
	//pDamageRecieved = 0;
	Initialize();
	
}

void GenericEntity::Initialize()
{
	pHull = game::Hull();
	pID = -100;
	LinkTarget = LinkAttachment();
	this->pDestroyed = false;
}

Coord GenericEntity::CenterPosition() const
{
	return this->pPosition;// +GetFigureCenter(); // geometry::rotateCoord(this->getDimension().Center().yflip() / 2, this->getUnitHeading());
};

Coord GenericEntity::getPosition() const
{ 
	return this->pPosition;
};

void GenericEntity::SetPosition(const Coord& pos )
{
	RefreshLinkPosition(pos);
	this->pPosition = pos;
}

void GenericEntity::RefreshLinkPosition(const Coord& pos)
{
	LinkTarget.setAttachmentposition(pos.yflip());
}

void GenericEntity::SetID( int set )
{
	if( !IDIsSet() && set != -100 )
		this->pID = set;
}

bool GenericEntity::IDIsSet()
{
	return this->pID != -100;
}


void GenericEntity::HandleSingleHit( const Coord & Coord, const game::Damage& damageType)
{
	if( (damageType.value * damageType.AOE) == 0 ) 
		return;

	//int recievedDamage = this->getGraphics()->getGraphicalObject().get()->damagePixelBox(Coord,damageType);
	AddDamageRecieved(damageType);
}

void GenericEntity::HandleMultipleHit( CoordWithCollisionData canvas[1024], int size )
{
	//multiple draw to sprite from this location
	this->getGraphics()->getGraphicalObject()->getMainObject()->colorSet( canvas, size );
}

//void GenericEntity::HandleAOEHit( const Coord & impactPoint, const game::Damage& damageType )
//{
//	using CoordWithCollisionData;
//	using primitives::CollisionData;
//
//	CoordWithCollisionData* damageMap = new CoordWithCollisionData[1024];
//	//damageType.GenerateAOEDamage(Coord, damageMap,this->getSize());
//	//for( auto it = damageMap.begin(); it != damageMap.end(); ++it )
//	//{
//	//	HandleSingleHit(it->first,it->second,out_debri);
//	//}
//
//	int AOEradius = std::fmax(damageType.AOE_range,this->getSize());
//		
//	int hitPoints = std::fmin(1024,20*AOEradius);
//	//damageMap[0] = coordWithInt(impactPoint.x,impactPoint.y,255*(this->getHitPoint( impactPoint ).value/damageType.value));
//
//	double AOEDamage = (2*(double)damageType.value);
//	double maxDistance = std::sqrt(AOEDamage);
//	std::normal_distribution<double> distribution = std::normal_distribution<double>(0,AOEradius/6);
//
//    for( int i = 1; i < hitPoints; i++ )
//	{
//		Coord AOEHitPoint = statistics::getRandomCoordInCircle( primitives::Circle(impactPoint,maxDistance), distribution );
//
//		CollisionResult collisionResult = this->getGraphics()->getGraphicalObject()->coordinateCollisionResult( AOEHitPoint );
//		if( collisionResult.IsHitMiss() )
//		{
//			//damageMap[i] = CoordWithCollisionData(primitives::EnumPixelColorCode::CCNoPaint,primitives::EnumPixelColorCode::CCNoPaint, CollisionData( primitives::EnumPixelColorCode::CCNoPaint, collisionResult ) );
//			continue;
//		}
//
//		int aoeDistance = geometry::realDistanceQuadratic( AOEHitPoint,impactPoint);
//		game::Damage hitDamage = damageType;
//
//		if(aoeDistance>0 && aoeDistance < AOEDamage)
//		{
//			hitDamage.value = optimization::DIV(AOEDamage,aoeDistance);
//			int color = 0;
//			int boxHitPoints = this->getHitPoint( AOEHitPoint, collisionResult ).value;
//			/*if( hitDamage.value >= boxHitPoints )
//				damageMap[i] = CoordWithCollisionData(AOEHitPoint.x,AOEHitPoint.y,  CollisionData( primitives::EnumPixelColorCode::CCInvisible, collisionResult ) );
//			else 
//				damageMap[i] = CoordWithCollisionData(AOEHitPoint.x,AOEHitPoint.y, CollisionData( 255 * hitDamage.value/boxHitPoints, collisionResult )  );*/
//
//			this->getGraphics()->getGraphicalObject()->damagePixelBox(AOEHitPoint, hitDamage, collisionResult );
//			AddDamageRecieved( hitDamage );
//		}
//		//else
//		//	damageMap[i] = CoordWithCollisionData(AOEHitPoint.x,AOEHitPoint.y, CollisionData( primitives::EnumPixelColorCode::CCNoPaint, collisionResult ) );
//	}
//	//HandleMultipleHit( damageMap, hitPoints );
//	//delete[] damageMap;
//}

//std::map<Coord,int> GenericEntity::TransferAccumulatedDamage()
//{
//	return std::move( this->pGraphics.get()->GetAndClearAccummulatedDamage() );
//}

void GenericEntity::ApplyDamage( const Coord & Coord, const game::Damage& damageType  )
{
	HandleSingleHit( Coord, damageType );
}

void GenericEntity::handleEvent( SDL_Event & e, const Coord& cameraPosition )
{
	IterateActiveLinks([&](LinkAttachment& link) {
		link.link->SetOrigoPosition(cameraPosition);
		link.link->HandleEvent(e);
	});
}

//std::shared_ptr<SurfaceButton> GenericEntity::getLink(string id) const
//{ 
//	for (int i = 0; i < this->pTextLink.size(); i++)
//		if (pTextLink[i].link->ID() == id)
//			return pTextLink[i].link;
//	return nullptr;
//}


void GenericEntity::ClearLinkMemory()
{
	//for( auto it = this->pTextLink.begin(); it != this->pTextLink.end(); ++ it )
	//	it->clearMemory();
}

//void GenericEntity::generateSingleLink( Coord relativePosition, std::string text )
//{
//	ClearLinkMemory();
//	generateNewLink( relativePosition, text, enLinkType::LINKSTANDARD );
//}
//
void GenericEntity::generateNewLink(LinkObject& object)
{
	generateNewLink(object.relativepos, object.text, object.size, object.GetLinkType(), object.type);
}
//
void GenericEntity::generateNewLink( const Coord& relativePosition, std::string text, const Coord& size, enLinkType type, std::string callbackname)
{
	LinkAttachment newAttachment = LinkAttachment();
	//this->pTextLink.push_back( newAttachment );
	//this->pTextLink.back().generateLink( relativePosition, this->getPosition() + relativePosition, text, type, callbackname);
	//this->pTextLink.back().Resize(size);
}
//
//void GenericEntity::generateNewLink( const Coord& relativePosition, std::string text, enLinkType type )
//{
//	LinkAttachment newAttachment = LinkAttachment();
//	this->pTextLink.push_back( newAttachment );
//	this->pTextLink.back().generateLink( relativePosition, this->getPosition() + relativePosition, text,type );
//}

void GenericEntity::setTexture( std::shared_ptr<GraphicalEntity> set)
{
	pGraphics = set;
}

void GenericEntity::FlipGraphics()
{
	this->pGraphics->getGraphicalObject()->getMainObject()->FlipGraphicalObject();
}

void GenericEntity::Rotate( enObjectDiscreteRotation rotation )
{
	switch(rotation)
	{
	case(enObjectDiscreteRotation::go90Rotation):
		RotateClockwise(geometry::PI_div2);
		break;
	case(enObjectDiscreteRotation::go180Rotation):
		RotateClockwise(std::_Pi);
		break;
	case(enObjectDiscreteRotation::go270Rotation):
		RotateClockwise(-geometry::PI_div2);
		break;
	case(enObjectDiscreteRotation::goNoRotation):
		this->getGraphics()->setRelativeHeading(0);	//correct???
		break;
	}
}

void GenericEntity::RotateClockwise( double rad )
{
	this->pGraphics->rotateRelativeHeadingClockwise( rad );
}
void GenericEntity::setGlobalHeading( double newHeading )
{ 
	this->pGraphics->setGlobalHeading( newHeading );
}
void GenericEntity::setUnitHeading( double newHeading )
{ 
	this->pGraphics->setUnitHeading( newHeading ); 
}

bool GenericEntity::isDestroyed()
{
	//if( getPercentHullLeft() < 0.3 ) //0.3 originally
	//	return true;
	//return false;
	return this->pDestroyed;
}
void GenericEntity::DestroyEntity()
{
	this->pHull.setHitpoints(game::enHitPointType::HP_STRUCTURAL, 0, false); //  pDamageRecieved = 1 + pMaxHullPoints;
	this->ClearTextLinks();
	this->pDestroyed = true;
}

void GenericEntity::ClearTextLinks()
{
	LinkTarget.link.reset();

}


int GenericEntity::getSize() const
{
	return this->getGraphics()->getSize();
}

spriteSize GenericEntity::getDimension() const
{
	return this->getGraphics()->getDimension();
}

void GenericEntity::AddDamageRecieved( const game::Damage& set )
{
	this->pHull.DamageHull(set, set.DamageReductionBasedOnTargetSize( this->getSize() ));
}

int GenericEntity::getMaxHullPoints() const
{ 
	return this->pHull.getHitPoints(game::enHitPointType::HP_STRUCTURAL).maxHitPoints();
};

int GenericEntity::getDamageRecieved() const  
{
	return this->pHull.getHitPoints(game::enHitPointType::HP_STRUCTURAL).damageRecieved();
};

//void GenericEntity::SetDamageRecieved(const game::Damage& set)
//{ 
//	this->pHitPoints.setHitpoints(set);
//}

double GenericEntity::getPercentHullLeft()
{
	double result = this->pHull.getHitPoints(game::enHitPointType::HP_STRUCTURAL).PercentageLeft();
	if( result < 0 )
		return 0;
	return result;
}

int GenericEntity::getHitPointLeft()
{
	return this->pHull.getHitPoints(game::enHitPointType::HP_STRUCTURAL).remainingHitPoints();
}

string GenericEntity::GetHitPointForTooltip()
{
	auto hitpoints = this->pHull.getHitPoints(game::enHitPointType::HP_STRUCTURAL);
	return IntToString(hitpoints.remainingHitPoints()) + "/" + IntToString(hitpoints.maxHitPoints());
}

GenericEntity::~GenericEntity()
{
	this->FreeMemory();
}

void GenericEntity::FreeMemory()
{
	this->ClearLinkMemory();
	this->pGraphics.reset();
}

void GenericEntity::GenerateTargetLink()
{
	if (this->LinkTarget.link == false)
	{
		Coord linkSize = this->getGraphics()->getGraphicalObject()->GetBoundarySize().ScaleToLargestDimension();
		Coord pos;
		if (linkSize.MultiplyXY() == 0)
		{
			linkSize = this->getSize();
			pos = Coord(0, 0);
		}
		else
		{
			Coord sizeDiff = this->getSize() - linkSize;
			pos = sizeDiff / 2;
		}
		this->LinkTarget.generateLink(pos, this->getPosition() + pos, "", enLinkType::LINKTARGETSELECTED);// std::shared_ptr<ScreenButton>(new ScreenButton(pos, StyleFactory::SelectTargetStyle(64, 64), "selecttarget"));
	}
}

void GenericEntity::IterateActiveLinks(std::function<void(LinkAttachment&)> func) const
{
	auto activeLinks = GetActiveLinks();
	for (int i = 0; i < activeLinks.size(); i++)
	{
		func(activeLinks[i]);
	}
}

std::vector<LinkAttachment> GenericEntity::GetActiveLinks() const
{
	std::vector<LinkAttachment> result = std::vector<LinkAttachment>();
	if (this->LinkTarget.link)
		result.push_back(this->LinkTarget);
	return result;
}

void GenericEntity::SetHullHitPoints(game::enHitPointType type, int amount, bool valueIsNewMax)
{
	this->pHull.setHitpoints(type, amount, valueIsNewMax);
}

game::Hull GenericEntity::getHull()
{
	return this->pHull;
}
//
//game::HitPoints GenericEntity::getHitPoint( const Coord & pos, CollisionResult colResult)
//{
//	if( colResult.IsMainBodyHit() )
//		return getHitPoint();
//	else if( colResult.IsHitMiss() )
//		return game::HitPoints();
//
//	game::HitPoints result;
//
//	/*this->getGraphics()->getGraphicalObject()->GetHitPointsFromLink(pos,colResult.GetHitIndex());
//	if( result.type != game::enHitPointType::HP_INVALID )
//		return result;
//	else
//	{
//		throw std::runtime_error("Error occur at getting hitpoint damage");
//		return game::HitPoints(game::enHitPointType::HP_INVALID);
//	}*/
//}

//game::HitPoints GenericEntity::getHitPoint()
//{
//	return this->pHitPoints();
//}
//
