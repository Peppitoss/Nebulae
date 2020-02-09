//#include"Background_Level_00.h"
//#include"GameManager.h"
//
//Background_Level_00::Background_Level_00(std::shared_ptr<MaskEffector> effect, std::shared_ptr<LocationData> location) : Level(effect)
//{
//	this->SetLevelID("backgroundlevel");
//	this->pLevelLocation = location;
//}
//
//std::shared_ptr<LocationData> Background_Level_00::GetStartLocation()
//{
//	return pLevelLocation;
//}
//
//std::shared_ptr<LocationData> Background_Level_00::GetLocation()
//{
//	return pLevelLocation;
//}
//
//void Background_Level_00::RunLevel(const Millisecond& milliseconds)
//{
//	if (this->pLevelStage == 0)
//	{
//		GameManager::GetParticleEngine()->AddNewEffect(GameManager::GetParticleEngine()->GetEffectParticleTrail(Coord(0)));
//		pLevelStage++;
//	}
//	else if (this->pLevelStage == 1)
//	{
//		if (this->GetTimerValue() > Second(180))
//		{
//			pLevelStage++;
//		}
//	}
//	else if (this->pLevelStage == 2)
//	{
//		pLevelStage++;
//		EndLevel();
//	}
//
//
//}
//
//void Background_Level_00::TriggerFinishLevelFirstStage()
//{
//}
//
//void Background_Level_00::TriggerFinishLevelLastStage()
//{
//	Millisecond delay = GetEndLevelTimerValue();
//	if (delay > Second(8))
//	{
//		GameManager::ResetBackgroundLevel();
//	}
//}
//
//void Background_Level_00::EndLevel()
//{
//	Level::EndLevel();
//
//}
//
//void Background_Level_00::StartLevel()
//{
//}
//bool Background_Level_00::IsLevelFinished()
//{
//	return pLevelFinished;
//}
//
//string Background_Level_00::StartMissionText()
//{
//	return "";
//}
//string Background_Level_00::EndMissionText()
//{
//	return "";
//}
//
//void Background_Level_00::AddRegions()
//{
//
//}
