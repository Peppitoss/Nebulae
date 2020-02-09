#pragma once
#include<map>
#include"Graphics.h"
#include<unordered_map>

namespace game {

	struct Damage
	{
		Damage();

		Damage( const Damage & );
		Damage(int set);
		Damage(int set, int AOE, int AOERange);

		Damage& operator=(const Damage & other);
		Damage& operator=(const Damage && other);

		Damage ReduceDamage(double reductionPercent) const;
		double DamageReductionBasedOnTargetSize(int targetSize) const;

		primitives::vector GetExplosiveForce() const;
		
		bool isAOEDamage() const;

		int value;	//material damage
		int AOE;	//structural damage
		int AOE_range;

		primitives::vector Direction;
		unsigned long ExplosiveForce;


		//void GenerateAOEDamage(const Coord& impactpnt, coordWithInt result[1024], int vesselsize ) const;

	};


enum enHitPointType
{
	HP_NORMAL = 1,
	HP_STRUCTURAL = 2,
	HP_MATERIAL = 3,
	HP_INVALID = 4
};

class HitPoints
{

public:
	HitPoints();
	HitPoints(enHitPointType);
	HitPoints(const HitPoints& HitPoints);
	HitPoints(HitPoints&& HitPoints);
	HitPoints( int );
	HitPoints(int, enHitPointType);

	HitPoints& operator=(const HitPoints& other);
	HitPoints& operator=(const HitPoints&& other);
	
	bool HasNoHitPoint() const;
	int ReduceHitPoints(int amount);
	void SetHitPoints(int amount, bool setNewMax);
	void SetHitPoints(int amount);
	double PercentageLeft();

	int damageRecieved() const;
	int maxHitPoints() const;
	int remainingHitPoints() const;

private:

	int pMaxValue;
	int pValue;
	enHitPointType type;
};

class Hull
{

public:

	Hull();
	Hull(int materialHP, int structuralHP);
	Hull( const Hull& other);
	Hull( Hull&& other);

	Hull& operator=(const Hull& other);
	Hull& operator=(Hull&& other);

	Hull& DamageHull(const Damage&);
	Hull& DamageHull(const Damage&, double AOEDamageMultiplier);

	//Hull& DamageHull(const Damage&);
	//Hull& DamageHull(const Damage&, double AOEDamageMultiplier);

	HitPoints getHitPoints(enHitPointType type) const;
	bool noHitPoints(enHitPointType type) const;

	void setHitpoints(enHitPointType, int set, bool valueIsNewMax );
	int ReduceHitPoints(enHitPointType, int red );

private:

	void Initialize();

	HitPoints pStructuralHP;
	HitPoints pMaterialHP;

};

class LauncherAttributes
{

public:

	LauncherAttributes();

	void SetRateOfFire( double set ) { pRateOfFire = set; };
	double GetRateOfFire() { return pRateOfFire; };


private:

	double pRateOfFire;

};

}