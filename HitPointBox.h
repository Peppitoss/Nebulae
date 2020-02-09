//#pragma once
//#include<algorithm>
//#include<string>
//
//enum HPBoxFlag
//{
//	enNoDamage = 0,
//	enDamage = 1,
//	enOuterDestroyed = 2,
//	enInnerDestroyed = 3,
//	enInnerOuterDestroyed = 4,
//};
//
//class HitPointBox
//{
//public:
//
//	HitPointBox();
//	HitPointBox( int SetInnerHitPoint, int SetOuterHitPoint );
//
//	HitPointBox( const HitPointBox& );
//	HitPointBox( const HitPointBox&& );
//
//	HitPointBox& operator=(const HitPointBox& other);
//	HitPointBox& operator=(const HitPointBox&& other);
//
//	void DamageInnerBox( int amount );
//	void DamageOuterBox( int amount );
//	HPBoxFlag DamageBox( int amount );
//
//	int GetOuterHitPoint() const;
//	int GetInnerHitPoint() const;
//
//	double GetPercentageOuterHitPointLeft() const;
//	double GetPercentageInnerHitPointLeft() const;
//
//	bool IsCollision() const;
//	std::string ToString() const;
//
//	HPBoxFlag GetFlag() const;
//
//private:
//
//	HPBoxFlag ChangeStateAfterDamage( int damage );
//	HPBoxFlag CalculateStateForDamagedBox();
//
//	int InnerInitHitPoint;
//	int OuterInitHitPoint;
//
//	int InnerHitPoint;
//	int OuterHitPoint;
//
//	HPBoxFlag pFlag;
//
//
//};