//#include"HitPointMap.h"
//#include"SurfaceGlobalList.h"
//
//HitPointMap::HitPointMap()
//{
//	Initialize();
//	pBoxes = CoordMap<HitPointBox>();
//	
//}
//
//HitPointMap::HitPointMap( int size , int InnerHitPoints, int OuterHitPoints)
//{
//	Initialize();
//	pBoxes = CoordMap<HitPointBox>(size);
//	
//}
//
//HitPointMap::HitPointMap( const BoxList & list, const Coord& centerPos, int InnerHitPoints, int OuterHitPoints )
//{
//	Initialize();
//	GenerateMap(list, InnerHitPoints, OuterHitPoints );
//	
//}
//
//HitPointMap::HitPointMap( const PixelFigure& figure, int InnerHitPoints, int OuterHitPoints )
//{
//	GenerateMap(figure, InnerHitPoints, OuterHitPoints );
//	Initialize();
//}
//
//HitPointMap::HitPointMap( const Sprite& sprite, int InnerHitPoints, int OuterHitPoints )
//{
//	Initialize();
//	GenerateMap(sprite, InnerHitPoints, OuterHitPoints );
//	
//}
//
//HitPointMap::HitPointMap( const HitPointMap& other)
//{
//	this->pBoxes = other.pBoxes;
//	this->pTotalHitPoints = other.pTotalHitPoints;
//}
//HitPointMap::HitPointMap( const HitPointMap&& other)
//{
//	this->pBoxes = std::move(other.pBoxes);
//	this->pTotalHitPoints = std::move(other.pTotalHitPoints);
//}
//
//HitPointMap& HitPointMap::operator=(const HitPointMap& other)
//{
//	this->pBoxes = other.pBoxes;
//	this->pTotalHitPoints = other.pTotalHitPoints;
//	return *this;
//}
//HitPointMap& HitPointMap::operator=(const HitPointMap&& other)
//{
//	this->pBoxes = std::move(other.pBoxes);
//	this->pTotalHitPoints = std::move(other.pTotalHitPoints);
//	return *this;
//}
//
//std::shared_ptr<Sprite> HitPointMap::GenerateMapFromSurface( string surfaceName )
//{
//	std::shared_ptr<Sprite> result = std::shared_ptr<Sprite>( new Sprite( SurfaceGlobalList::GetSurface(surfaceName) ));
//	this->GenerateMap(*result.get(),1,1);
//	return result;
//}
//
//void HitPointMap::GenerateMap( const PixelFigure& list,int InnerHitPoints, int OuterHitPoints )
//{
//	pBoxes = CoordMap<HitPointBox>(list.getX());
//	for( auto it = list.begin(); it != list.end(); it++ )
//	{
//		this->AddBox( (it).Coord(), HitPointBox(InnerHitPoints, OuterHitPoints ) );
//	}
//}
//
//void HitPointMap::GenerateMap( const BoxList & list,int InnerHitPoints, int OuterHitPoints )
//{
//	pBoxes = CoordMap<HitPointBox>(list.GetSize().getIntValue());
//	for( auto it = list.begin(); it != list.end(); it++ )
//	{
//		this->AddBox( (*it), HitPointBox(InnerHitPoints, OuterHitPoints ) );
//	}
//	this->pBoxes.OptimizeIteratorConstraints();
//}
//
//void HitPointMap::GenerateMap( const Sprite & sprite,int InnerHitPoints, int OuterHitPoints )
//{
//	int size = sprite.getSize();
//	pBoxes = CoordMap<HitPointBox>(size);
//
//	for(int x = 0; x < size; x++ )
//	{
//		for(int y = 0; y < size; y++ )
//		{
//			ColorRGB color = sprite.getColorFromPoint(Coord(x,y));
//			if( !(color == colorKeyFromEnum( sprite.getColorKey() ) ) )
//				this->AddBox( Coord(x,y), HitPointBox(InnerHitPoints, OuterHitPoints ) ); // CoordWithColor(x,y,color) );
//		}
//	}
//	this->pBoxes.OptimizeIteratorConstraints();
//}
//
//HPBoxFlag HitPointMap::DamageBox( const Coord& Coord, int amount)
//{
//	if( this->pBoxes.CoordIsFilled(Coord) )
//	{
//		//CoordMapItem<HitPointBox> item = pBoxes.GetItem( Coord );
//		HPBoxFlag damageResult = pBoxes.GetItem(Coord).Item->GetFlag(); //item.Item->GetFlag();
//
//		if( CoordinateIsCollision(Coord) )
//		{
//			damageResult = pBoxes.GetItem(Coord).Item->DamageBox( amount );
//			this->pTotalHitPoints -= amount;
//		}
//		return damageResult;
//	}
//	return HPBoxFlag::enNoDamage;
//}
//
//int HitPointMap::GetTotalHitPoints() const
//{
//	return this->pTotalHitPoints;
//}
//
//bool HitPointMap::CoordinateIsCollision( const Coord & Coord) const
//{
//	if( this->pBoxes.CoordIsFilled(Coord) )
//		return CoordinateIsCollision( pBoxes.GetItem( Coord ) );
//	return false;
//}
//
//bool HitPointMap::CoordinateIsCollision( const CoordMapItem<HitPointBox> & item ) const
//{
//	return item.Status == enCoordStatus::csOccupy && item.Item->IsCollision();
//}
//
//bool HitPointMap::IsInnerPointHit( const Coord& pos) const
//{
//	if( this->pBoxes.CoordIsFilled(pos) )
//	{
//		CoordMapItem<HitPointBox> item = pBoxes.GetItem( pos );
//		return item.Status == enCoordStatus::csOccupy && item.Item->GetFlag() == HPBoxFlag::enOuterDestroyed;
//	}
//	return false;
//}
//bool HitPointMap::IsOuterPointHit( const Coord& pos) const
//{
//	if( this->pBoxes.CoordIsFilled(pos) )
//	{
//		CoordMapItem<HitPointBox> item = pBoxes.GetItem( pos );
//		return item.Status == enCoordStatus::csOccupy && item.Item->GetFlag() != HPBoxFlag::enInnerOuterDestroyed && item.Item->GetFlag() != HPBoxFlag::enOuterDestroyed;
//	}
//	return false;
//}
//
//bool HitPointMap::IsInnerPointDestroyed( const Coord& pos) const
//{
//	if( this->pBoxes.CoordIsFilled(pos) )
//	{
//		CoordMapItem<HitPointBox> item = pBoxes.GetItem( pos );
//		return item.Status == enCoordStatus::csOccupy && ( item.Item->GetFlag() == HPBoxFlag::enInnerOuterDestroyed || item.Item->GetFlag() == HPBoxFlag::enInnerDestroyed );
//	}
//	return false;
//}
//bool HitPointMap::IsOuterPointDestroyed( const Coord& pos) const
//{
//	if( this->pBoxes.CoordIsFilled(pos) )
//	{
//		CoordMapItem<HitPointBox> item = pBoxes.GetItem( pos );
//		return item.Status == enCoordStatus::csOccupy && ( item.Item->GetFlag() == HPBoxFlag::enInnerOuterDestroyed || item.Item->GetFlag() == HPBoxFlag::enOuterDestroyed );
//	}
//	return false;
//}
//
//double HitPointMap::GetPercentageOuterHitPointLeft(const Coord& pos) const
//{
//	if( this->pBoxes.CoordIsFilled(pos) )
//	{
//		return pBoxes.GetItem( pos ).Item->GetPercentageOuterHitPointLeft();
//	}
//	return 0;
//}
//double HitPointMap::GetPercentageInnerHitPointLeft(const Coord& pos) const
//{
//	if( this->pBoxes.CoordIsFilled(pos) )
//	{
//		return pBoxes.GetItem( pos ).Item->GetPercentageInnerHitPointLeft();
//	}
//	return 0;
//}
//
//bool HitPointMap::IsEmpty() const
//{
//	return this->GetHitPointBoxAmount() == 0;
//}
//
//int HitPointMap::GetHitPointBoxAmount() const
//{
//	return this->pBoxes.GetInsertedBoxes();
//}
//
//int HitPointMap::GetOuterHitPointAtCoord( const Coord & pos ) const 
//{
//	if( this->pBoxes.CoordIsFilled(pos) )
//		return this->pBoxes[pos]->GetOuterHitPoint();
//	return 0;
//}
//int HitPointMap::GetInnerHitPointAtCoord( const Coord & pos ) const
//{
//	if( this->pBoxes.CoordIsFilled(pos) )
//		return this->pBoxes[pos]->GetInnerHitPoint();
//	return 0;
//}
//
//void HitPointMap::AddBox( const Coord pos, const HitPointBox& box)
//{
//	pBoxes.InsertBox( std::shared_ptr<HitPointBox>( new HitPointBox( box ) ), pos );
//	pTotalHitPoints += box.GetInnerHitPoint();
//	pTotalHitPoints += box.GetOuterHitPoint();
//
//}
//
//void HitPointMap::Initialize()
//{
//	pTotalHitPoints = 0;
//	//pTotalInnerHitPoints = 0;
//	//pTotalOuterHitPoints = 0;
//}