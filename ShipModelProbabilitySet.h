#pragma once
#include<map>
#include<string>
#include"Graphics.h"

using std::string;

enum enOrderType
{
	oTrade = 0,
	oFight = 1,
	oPatrol = 2

};

class ShipModelProbabilitySet
{
public:
	
	ShipModelProbabilitySet();

	enOrderType GetOrderType( std::default_random_engine& rengine );
	
protected:

	std::map<enOrderType,int> pPropabilityList;
	//std::map<string,enOrderType> pOrderMap;

	//string sAttackPlayer;
	//string sTradeToCivic;
	//string sPatrol;

	void NormalizePercentages();
	
};

class TraderSet : public ShipModelProbabilitySet
{

public:
	TraderSet();
	TraderSet( int difficulty );
};

class FighterSet : public ShipModelProbabilitySet
{

public:
	FighterSet();
	FighterSet( int difficulty );
};