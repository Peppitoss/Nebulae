#include"ProgressData.h"



LevelFinishStats StatsCollection::GetStatsFromLevelName(string name)
{
	for (int i = 0; i < pLevelsCompleted.size(); i++)
		if (pLevelsCompleted[i].GetLevelIDName() == name)
			return pLevelsCompleted[i];
	return LevelFinishStats();
}



ProgressData::ProgressData()
{
	pContinueFromLevelID = "";
	pStats = StatsCollection();
}

ProgressData::ProgressData(const ProgressData& other)
{
	this->pStats = other.pStats;
	this->pContinueFromLevelID = other.pContinueFromLevelID;
}

ProgressData::ProgressData(const ProgressData&& other)
{
	this->pStats = std::move(other.pStats);
	this->pContinueFromLevelID = std::move(other.pContinueFromLevelID);
}

ProgressData& ProgressData::operator=(const ProgressData& other)
{
	this->pStats = other.pStats;
	this->pContinueFromLevelID = other.pContinueFromLevelID;
	return *this;
}
ProgressData& ProgressData::operator=(const ProgressData&& other)
{
	this->pStats = std::move(other.pStats);
	this->pContinueFromLevelID = std::move(other.pContinueFromLevelID);
	return *this;
}

