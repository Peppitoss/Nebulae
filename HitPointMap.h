//#pragma once
//#include"CoordMap.h"
//#include"Graphics.h"
//#include"BoxList.h"
//#include"PixelFigure.h"
//#include"Sprite.h"
//
//class HitPointMap
//{
//
//public:
//	
//	HitPointMap();
//	HitPointMap( const PixelFigure&, int InnerHitPoints, int OuterHitPoints );
//	HitPointMap( const Sprite&, int InnerHitPoints, int OuterHitPoints );
//	HitPointMap( int size, int InnerHitPoints, int OuterHitPoints );
//	HitPointMap( const BoxList & list, const Coord& centerPos, int InnerHitPoints, int OuterHitPoints );
//
//	HitPointMap( const HitPointMap& );
//	HitPointMap( const HitPointMap&& );
//
//	HitPointMap& operator=(const HitPointMap& other);
//	HitPointMap& operator=(const HitPointMap&& other);
//
//	void AddBox( const Coord pos, const HitPointBox& );
//	bool CoordinateIsCollision( const Coord & ) const;
//
//	int GetOuterHitPointAtCoord( const Coord & ) const;
//	int GetInnerHitPointAtCoord( const Coord & ) const;
//
//	bool IsInnerPointHit( const Coord& ) const;
//	bool IsOuterPointHit( const Coord& ) const;
//
//	bool IsInnerPointDestroyed( const Coord& ) const;
//	bool IsOuterPointDestroyed( const Coord& ) const;
//
//	double GetPercentageOuterHitPointLeft(const Coord&) const;
//	double GetPercentageInnerHitPointLeft(const Coord&) const;
//
//	HPBoxFlag DamageBox( const Coord& , int amount );
//
//	int GetHitPointBoxAmount() const;
//	int GetTotalHitPoints() const;
//	const CoordMap<HitPointBox>& const GetBoxes() const { return pBoxes; };
//
//	//void DestroyHitPointBox( const Coord& );
//
//	bool IsEmpty() const;
//	std::shared_ptr<Sprite> GenerateMapFromSurface( string surfaceName );
//
//private:
//
//	void Initialize();
//
//	void GenerateMap( const BoxList & list,int InnerHitPoints, int OuterHitPoints );
//	void GenerateMap( const PixelFigure& list,int InnerHitPoints, int OuterHitPoints );
//	void GenerateMap( const Sprite& list,int InnerHitPoints, int OuterHitPoints );	//particularly slow i assume
//	CoordMap<HitPointBox> pBoxes;
//
//	bool CoordinateIsCollision( const CoordMapItem<HitPointBox> & ) const;
//	int pTotalHitPoints;
//	//int pTotalInnerHitPoints;
//	//int pTotalOuterHitPoints;
//};