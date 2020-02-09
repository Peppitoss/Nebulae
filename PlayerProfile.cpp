#include"PlayerProfile.h"
#include"ProfileReader.h"
#include"ProfileWriter.h"
#include"ProfileGlobalList.h"

PlayerProfile::PlayerProfile()
{
	pProfileName = "";
	pProfileCurrency = 100000;
	Initialize();
}

void PlayerProfile::Initialize()
{

	pProfileName = "noname";
	pProgressData = ProgressData();
	pLoadedVessel = nullptr;

}

std::shared_ptr<PlayerVessel> PlayerProfile::GetProfileVessel()
{
	if (pLoadedVessel == nullptr)
		return GetLastSavedPlayerVessel();
	return pLoadedVessel;
}

std::shared_ptr<PlayerVessel> PlayerProfile::GetLastSavedPlayerVessel()
{
	auto result = ProfileGlobalList::GetSavedVessel(pProfileName);
	pLoadedVessel = result;
	return result;

}
std::shared_ptr<PlayerWalker> PlayerProfile::GetLastSavedPlayerWalker() const
{
	return ProfileGlobalList::GetSavedWalker(pProfileName);

}
void PlayerProfile::SaveCurrentPlayerInstance(std::shared_ptr<PlayerInterface> player)
{
	ProfileWriter writer;

	if (player->IsSpaceVessel())
	{
		writer.WriteVessel(this, std::dynamic_pointer_cast<PlayerVessel>(player));
	}
	else
	{
		writer.WriteWalker(this, std::dynamic_pointer_cast<PlayerWalker>(player));
	}
}

//void PlayerProfile::RevertCurrentPlayerInstance()
//{
//	//if (auto vessel = std::dynamic_pointer_cast<Vessel>(pPlayerVesselCurrentInstance))
//	//	pPlayerVesselCurrentInstance = this->pPlayerVessel;// a copy !!
//	//else if (auto walker = std::dynamic_pointer_cast<Vessel>(pPlayerVesselCurrentInstance))
//	//	pPlayerVesselCurrentInstance = walker;// a copy !!
//}

void PlayerProfile::SaveLevelFinished(LevelFinishStats stats)
{
	this->pProgressData.GetStats().AddStats(stats);
}

//void PlayerProfile::GeneratePlayerVesselFromVesselName(string vesselName)
//{
//	SetPlayerVessel(PlayerVessel::GeneratePlayerVesselFromVesselName(vesselName));
//}
//
//void PlayerProfile::GeneratePlayerWalkerFromWalkerName(string vesselName)
//{
//	SetPlayerWalker(PlayerWalker::GeneratePlayerWalkerFromWalkerName(vesselName));
//}