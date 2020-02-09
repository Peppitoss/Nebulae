#pragma once
#include"Graphics.h"
#include"ShipModel.h"
#include<string>
#include<set>

using std::string;

class MonsterSpawner
{

public:
	MonsterSpawner();

	void SpawnMonster(const std::set<ShipModel>& name);

private:



};