#include"MonsterSpawner.h"
#include"GameManager.h"

MonsterSpawner::MonsterSpawner()
{

}

void MonsterSpawner::SpawnMonster(const std::set<ShipModel>& name)
{
	std::set<int> UnitIDs = std::set<int>();
	for (auto it = name.begin(); it != name.end(); ++it)
		UnitIDs.insert((*it).GetID());

	GameManager::CurrentInstanceManager()->SpawnUnit(UnitIDs);
}