#include"ShipModelProbabilitySet.h"

ShipModelProbabilitySet::ShipModelProbabilitySet()
{
	pPropabilityList[oFight] = 34;
	pPropabilityList[oTrade] = 33;
	pPropabilityList[oPatrol] = 33;
}

enOrderType ShipModelProbabilitySet::GetOrderType( std::default_random_engine& rengine )
{
	std::uniform_int_distribution<int> dist_percentage(0, 100);
	int roll100 = dist_percentage(rengine);

	int sum = 0;

	for( auto it = this->pPropabilityList.begin(); it != this->pPropabilityList.end(); ++it )
	{
		sum += it->second;
		if( roll100 < sum )
			return it->first;
	}
}

void ShipModelProbabilitySet::NormalizePercentages()
{
	int max = 0;
	for( auto it = this->pPropabilityList.begin(); it != this->pPropabilityList.end(); ++it )
	{
		max += (*it).second;
	}

	for( auto it = this->pPropabilityList.begin(); it != this->pPropabilityList.end(); ++it )
	{
		( ( (*it).second ) *= 100 ) /= max;
	}
}

TraderSet::TraderSet() : ShipModelProbabilitySet()
{
	pPropabilityList[oFight] = 0;
	pPropabilityList[oTrade] = 80;
	pPropabilityList[oPatrol] = 20;
}

TraderSet::TraderSet( int difficulty )
{
	pPropabilityList[oFight] = 0;
	pPropabilityList[oTrade] = 80;
	pPropabilityList[oPatrol] = 20;

	NormalizePercentages();
}

FighterSet::FighterSet() : ShipModelProbabilitySet()
{
	pPropabilityList[oFight] = 20;
	pPropabilityList[oTrade] = 50;
	pPropabilityList[oPatrol] = 30;
}

FighterSet::FighterSet( int difficulty ) : ShipModelProbabilitySet()
{
	pPropabilityList[oFight] = 20;
	pPropabilityList[oTrade] = 50;
	pPropabilityList[oPatrol] = 30;

	NormalizePercentages();
}