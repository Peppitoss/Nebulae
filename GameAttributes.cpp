#include"GameAttributes.h"
#include"Statistics.h"

namespace game {

	const static int INITHP = 20;
	const static double StructuralDamageReduction = 0.8;
	const static double StructuralMinThreshold = 0.1;

	HitPoints::HitPoints()
	{
		pValue = INITHP;
		pMaxValue = pValue;
		type = enHitPointType::HP_NORMAL;
	}

	HitPoints::HitPoints(enHitPointType type)
	{
		pValue = INITHP;
		pMaxValue = pValue;
		this->type = type;
	}

	HitPoints::HitPoints(int setValue, enHitPointType type)
	{
		this->pValue = setValue;
		this->pMaxValue = pValue;
		this->type = type;
	}

	HitPoints::HitPoints(int setValue )
	{
		this->pValue = setValue;
		this->pMaxValue = pValue;
		this->type = enHitPointType::HP_NORMAL;
	}

	HitPoints::HitPoints(const HitPoints& other)
	{
		this->pValue = other.pValue;
		this->pMaxValue = other.pMaxValue;
		this->type = other.type;
	}

	HitPoints::HitPoints(HitPoints&& HitPoints)
	{
		this->pValue = std::move(HitPoints.pValue);
		this->pMaxValue = std::move(HitPoints.pMaxValue);
		this->type = std::move(HitPoints.type);
	}

	HitPoints& HitPoints::operator=(const HitPoints& other) 
	{
		this->pValue = other.pValue;
		this->pMaxValue = other.pMaxValue;
		this->type = other.type;
		return *this;
	}

	HitPoints& HitPoints::operator=(const HitPoints&& other)
	{
		this->pValue = std::move(other.pValue);
		this->pMaxValue = std::move(other.pMaxValue);
		this->type = std::move(other.type);
		return *this;
	}

	bool HitPoints::HasNoHitPoint() const
	{
		return this->pValue <= 0;
	}
	int HitPoints::ReduceHitPoints(int amount)
	{
		if (this->pValue > 0)
			this->pValue -= amount;
		return this->pValue;
	}

	void HitPoints::SetHitPoints(int amount)
	{
		SetHitPoints(amount, false);
	}

	void HitPoints::SetHitPoints(int amount, bool setNewMax)
	{
		this->pValue = amount;
		if (setNewMax)
			this->pMaxValue = this->pValue;
	}

	double HitPoints::PercentageLeft()
	{
		return 1 - ( ((double)this->damageRecieved()) / this->pMaxValue );
	}

	int HitPoints::remainingHitPoints() const
	{
		return this->pValue;
	}

	int HitPoints::damageRecieved() const
	{
		return this->pMaxValue - this->pValue;
	}
	int HitPoints::maxHitPoints() const
	{
		return this->pMaxValue;
	}

	Hull::Hull()
	{
		Initialize();
	}

	Hull::Hull(int materialHP, int structuralHP)
	{
		Initialize();
		this->pMaterialHP = materialHP;
		this->pStructuralHP = structuralHP;
	}

	Hull::Hull( const Hull& other )
	{
		this->pMaterialHP = other.pMaterialHP;
		this->pStructuralHP = other.pStructuralHP;
	}

	Hull::Hull( Hull&& other)
	{
		this->pMaterialHP = std::move(other.pMaterialHP);
		this->pStructuralHP = std::move(other.pStructuralHP);
	}

	Hull& Hull::operator=(const Hull& other)
	{
		this->pMaterialHP = other.pMaterialHP;
		this->pStructuralHP = other.pStructuralHP;
		return *this;
	}
	Hull& Hull::operator=(Hull&& other)
	{
		this->pMaterialHP = std::move(other.pMaterialHP);
		this->pStructuralHP = std::move(other.pStructuralHP);
		return *this;
	}

	void Hull::Initialize()
	{
		this->pStructuralHP = HitPoints(enHitPointType::HP_STRUCTURAL);
		this->pMaterialHP = HitPoints(enHitPointType::HP_MATERIAL);
	}

	HitPoints Hull::getHitPoints(enHitPointType type) const
	{
		switch (type)
		{
		case(enHitPointType::HP_STRUCTURAL):
			return this->pStructuralHP;
		case(enHitPointType::HP_MATERIAL):
			return this->pMaterialHP;
		}
	}
	bool Hull::noHitPoints(enHitPointType type) const
	{
		switch (type)
		{
		case(enHitPointType::HP_STRUCTURAL):
			return this->pStructuralHP.HasNoHitPoint();
		case(enHitPointType::HP_MATERIAL):
			return this->pMaterialHP.HasNoHitPoint();
		}
	}

	void Hull::setHitpoints(enHitPointType type, int set, bool valueIsNewMax)
	{
		switch (type)
		{
		case(enHitPointType::HP_STRUCTURAL):
			this->pStructuralHP.SetHitPoints(set, valueIsNewMax);
			break;
		case(enHitPointType::HP_MATERIAL):
			this->pMaterialHP.SetHitPoints(set, valueIsNewMax);
			break;
		}
	}
	int Hull::ReduceHitPoints(enHitPointType type, int red)
	{
		switch (type)
		{
		case(enHitPointType::HP_STRUCTURAL):
			return this->pStructuralHP.ReduceHitPoints(red);
		case(enHitPointType::HP_MATERIAL):
			return this->pMaterialHP.ReduceHitPoints(red);
		}
	}

	Hull& Hull::DamageHull(const Damage& damage)
	{
		return DamageHull(damage, 1);
	}

	Hull& Hull::DamageHull(const Damage& damage, double AOEDamageMultiplier)
	{
		this->pStructuralHP.ReduceHitPoints(damage.AOE*AOEDamageMultiplier);
		if(this->pStructuralHP.PercentageLeft() > StructuralMinThreshold)
			damage.value*(1- StructuralDamageReduction);
		this->pMaterialHP.ReduceHitPoints(damage.value);
		return *this;
	}

	Damage::Damage()
	{ 
		value = 0;
		AOE = 0;
		AOE_range = 0;
		ExplosiveForce = 1;
	}

	Damage::Damage( const Damage & other)
	{
		value = other.value;
		AOE = other.AOE;
		AOE_range = other.AOE_range;
		this->Direction = other.Direction;
		this->ExplosiveForce = other.ExplosiveForce;
	}

	Damage::Damage(int set)
	{ 
		 value=set;
		 AOE = 0;
		 AOE_range = 0;
		 ExplosiveForce = 1;
	}
	Damage::Damage(int set, int setAOE, int setAOERange )
	{ 
		 value=set;
		 AOE = setAOE;
		 AOE_range = setAOERange;
		 ExplosiveForce = 1;
	}

	bool Damage::isAOEDamage() const
	{ 
		return AOE > 0 && AOE_range > 0; 
	}

	Damage& Damage::operator=(const Damage & other)
	{
		this->value = other.value;
		this->AOE = other.AOE;
		this->AOE_range = other.AOE_range;
		this->Direction = other.Direction;
		this->ExplosiveForce = other.ExplosiveForce;
		return *this;
	}

	Damage& Damage::operator=(const Damage && other)
	{
		this->value = std::move(other.value);
		this->AOE = std::move(other.AOE);
		this->AOE_range = std::move(other.AOE_range);
		this->Direction = std::move(other.Direction);
		this->ExplosiveForce = std::move(other.ExplosiveForce);
		return *this;
	}

	Damage Damage::ReduceDamage(double reductionPercent) const
	{ 
		Damage result = *this;
		result.AOE *= reductionPercent;
		result.value *= reductionPercent;
		return result;
	}

	double Damage::DamageReductionBasedOnTargetSize(int targetSize) const
	{
		if (this->AOE_range < targetSize)
		{
			int difference = targetSize - this->AOE_range;
			if (difference > 256)
				return 0.3;
			if (difference > 128)
				return 0.6;
			if (difference > 64)
				return 0.8;
			if (difference > 32)
				return 0.9;
		}
		return 1;
	}

	primitives::vector Damage::GetExplosiveForce() const
	{
		return this->Direction.yflip() * this->ExplosiveForce;
	}

	//void Damage::GenerateAOEDamage(const Coord& impactPoint, coordWithInt result[1024], int vesselsize) const
	//{
	//	int AOEradius = std::fmax(this->AOE_range,vesselsize);
	//	
	//	int hitPoints = std::fmax(1023,20*AOEradius);
	//	//result.reserve( hitPoints );
	//	result[0] = coordWithInt(impactPoint.x,impactPoint.y,this->value);

	//	double AOEDamage = (2*(double)this->value);
	//	std::normal_distribution<double> distribution = std::normal_distribution<double>(0,AOEradius/6);

 //       for( int i = 1; i < hitPoints; i++ )
	//	{
	//		Coord AOEHitPoint = statistics::getRandomCoordInCircle( primitives::Circle(impactPoint,AOEDamage), distribution );

	//		double aoeDistance = geometry::realDistanceQuadratic( AOEHitPoint,impactPoint);
	//		game::Damage hitDamage = *this;

	//		if(aoeDistance>0 && aoeDistance < AOEDamage)
	//		{
	//			hitDamage.value = AOEDamage / aoeDistance;
	//			result[i] = coordWithInt(AOEHitPoint.x,AOEHitPoint.y, hitDamage.value );
	//		}
	//	}
	//	result[1023] =  coordWithInt(-1,-1,-1);
	//}


	LauncherAttributes::LauncherAttributes()
	{
		pRateOfFire = 0;
	}
}