#pragma once
#include<string>

using std::string;

class LevelFinishStats
{

public:

	LevelFinishStats();
	LevelFinishStats(string pLocationName );

	string GetLevelIDName() const { return this->pLevelID; }

private:

	void Initialize();

	string pLevelID;

};