#pragma once
#include<vector>
#include"StatsCollection.h"


class ProgressData
{

public:

	ProgressData();
	ProgressData(const ProgressData& other);
	ProgressData(const ProgressData&& other);

	ProgressData& operator=(const ProgressData& other);
	ProgressData& operator=(const ProgressData&& other);

	StatsCollection GetStats() const { return pStats; }
	
	void SetContinueFromLevel(string set) { this->pContinueFromLevelID = set; }
	string GetContinueFromLevel() const { return this->pContinueFromLevelID; }

	int MissionsCompleted() { return 0; }

private:
	
	string pContinueFromLevelID;
	StatsCollection pStats;
};

