//#include"HitPointBox.h"
//
//
//HitPointBox::HitPointBox()
//{
//	this->InnerHitPoint = -1;
//	this->OuterHitPoint = -1;
//	this->InnerInitHitPoint = -1;
//	this->OuterInitHitPoint = -1;
//	this->pFlag = HPBoxFlag::enInnerOuterDestroyed;
//}
//
//HitPointBox::HitPointBox( int SetInnerHitPoint, int SetOuterHitPoint )
//{
//	this->InnerHitPoint = SetInnerHitPoint;
//	this->OuterHitPoint = SetOuterHitPoint;
//	this->InnerInitHitPoint = InnerHitPoint;
//	this->OuterInitHitPoint = OuterHitPoint;
//	this->pFlag = HPBoxFlag::enNoDamage;
//}
//
//
//HitPointBox::HitPointBox( const HitPointBox & other )
//{
//	InnerHitPoint = other.InnerHitPoint;
//	InnerInitHitPoint = other.InnerHitPoint;
//	OuterHitPoint = other.OuterHitPoint;
//	OuterInitHitPoint = other.OuterInitHitPoint;
//
//	pFlag = other.pFlag;
//}
//
//HitPointBox::HitPointBox( const HitPointBox && other )
//{
//	InnerHitPoint = std::move(other.InnerHitPoint);
//	InnerInitHitPoint = std::move(other.InnerHitPoint);
//	OuterHitPoint = std::move(other.OuterHitPoint);
//	OuterInitHitPoint = std::move(other.OuterInitHitPoint);
//	pFlag = std::move(other.pFlag);
//}
//
//
//HitPointBox& HitPointBox::operator=(const HitPointBox& other)
//{
//	InnerHitPoint = other.InnerHitPoint;
//	InnerInitHitPoint = other.InnerHitPoint;
//	OuterHitPoint = other.OuterHitPoint;
//	OuterInitHitPoint = other.OuterInitHitPoint;
//	pFlag = other.pFlag;
//	return *this;
//}
//
//HitPointBox& HitPointBox::operator=(const HitPointBox&& other)
//{
//	InnerHitPoint = std::move(other.InnerHitPoint);
//	InnerInitHitPoint = std::move(other.InnerHitPoint);
//	OuterHitPoint = std::move(other.OuterHitPoint);
//	OuterInitHitPoint = std::move(other.OuterInitHitPoint);
//	pFlag = std::move(other.pFlag);
//	return *this;
//}
//
//
//void HitPointBox::DamageInnerBox( int amount )
//{
//	this->InnerHitPoint -= amount;
//	ChangeStateAfterDamage( amount );
//}
//void HitPointBox::DamageOuterBox( int amount )
//{
//	this->OuterHitPoint -= amount;
//	ChangeStateAfterDamage( amount );
//}
//HPBoxFlag HitPointBox::DamageBox( int amount )
//{
//	if( pFlag == HPBoxFlag::enOuterDestroyed )
//		DamageInnerBox(amount);
//	DamageOuterBox(amount);
//	return this->pFlag;
//}
//
//bool HitPointBox::IsCollision() const
//{
//	return this->pFlag != enInnerOuterDestroyed;
//}
//
//int HitPointBox::GetOuterHitPoint() const
//{
//	return this->OuterHitPoint;
//}
//int HitPointBox::GetInnerHitPoint() const
//{
//	return this->InnerHitPoint; 
//}
//
//double HitPointBox::GetPercentageInnerHitPointLeft() const
//{
//	if( this->InnerHitPoint > 0)
//		return ((double)(this->InnerHitPoint)/ this->InnerInitHitPoint);
//	return 0;
//}
//double HitPointBox::GetPercentageOuterHitPointLeft() const 
//{
//	if( this->OuterHitPoint > 0)
//		return ((double)(this->OuterHitPoint)/ this->OuterInitHitPoint);
//	return 0;
//}
//
//HPBoxFlag HitPointBox::ChangeStateAfterDamage( int damage )
//{
//	HPBoxFlag newState = this->pFlag;
//
//	switch (this->pFlag)
//	{
//	case(enNoDamage):
//
//		if( damage == 0 )
//			return this->pFlag;
//
//		newState = CalculateStateForDamagedBox();
//		break;
//	case(enDamage):
//		newState = CalculateStateForDamagedBox();
//		break;
//	case(enInnerDestroyed):
//		if( this->OuterHitPoint < 0 )
//			newState = enInnerOuterDestroyed;
//		break;
//	case(enOuterDestroyed):
//		if( this->InnerHitPoint < 0 )
//			newState = enInnerOuterDestroyed;
//		break;
//	default:
//		break;
//	}
//
//	this->pFlag = newState;
//	return this->pFlag;
//}
//
//HPBoxFlag HitPointBox::CalculateStateForDamagedBox()
//{
//	HPBoxFlag newState = enDamage;
//	if( this->InnerHitPoint < 0 )
//	{
//		newState = enInnerDestroyed;
//		if( this->OuterHitPoint < 0 )
//			newState = enInnerOuterDestroyed;
//	}
//	else if( this->OuterHitPoint < 0 )
//	{
//		newState = enOuterDestroyed;
//	}
//	return newState;
//}
//
//std::string HitPointBox::ToString() const
//{
//	return "0"; //supposed to return the neighbourhood numbers, (return 8 if misses a neighbour, and 9 for all neighbours, an idea
//}
//
//HPBoxFlag HitPointBox::GetFlag() const
//{
//	return this->pFlag;
//}