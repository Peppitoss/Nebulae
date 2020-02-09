#pragma once
#include<string>
#include"ProgressData.h"
#include"PlayerVessel.h"
#include"PlayerWalker.h"

using std::string;

class PlayerProfile
{
public:

	PlayerProfile();

	string GetProfileName() const { return this->pProfileName; }
	void SetProfileName(string set) { pProfileName = set; }

	string GetVesselFileName() const { return this->pProfileName + "_vessel"; }

	int GetProfileCurrency() const { return this->pProfileCurrency; }
	void SetProfileCurrency(int set) { this->pProfileCurrency = set; }

	void SetProgressData(ProgressData data) { this->pProgressData = data; }
	ProgressData GetProgressData() const { return this->pProgressData; }

	//std::shared_ptr<PlayerInterface> GetCurrentPlayerUnit() const; // { return this->pPlayerVessel; }

	std::shared_ptr<PlayerVessel> GetProfileVessel();

	std::shared_ptr<PlayerVessel> GetLastSavedPlayerVessel();
	std::shared_ptr<PlayerWalker> GetLastSavedPlayerWalker() const;

	//void SetPlayerVessel(std::shared_ptr<PlayerVessel> set);
	//void SetPlayerWalker(std::shared_ptr<PlayerWalker> set);

	void SaveCurrentPlayerInstance(std::shared_ptr<PlayerInterface> player);
	//void RevertCurrentPlayerInstance();
	void SaveLevelFinished(LevelFinishStats stats);

private:

	void Initialize();
	
	std::shared_ptr<PlayerVessel> pLoadedVessel;

	string pProfileName;
	int pProfileCurrency;

	ProgressData pProgressData;

};