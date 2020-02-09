//#pragma once
//
//#include"Level.h"
//#include"Background_Level_00.h"
//
//class Background_Level_00 : public Level
//{
//
//public:
//
//	Background_Level_00(std::shared_ptr<MaskEffector>, std::shared_ptr<LocationData> location);
//
//	void EndLevel()override;
//
//	std::shared_ptr<LocationData> GetStartLocation() override;
//	std::shared_ptr<LocationData> GetLocation() override;
//
//	void RunLevel(const Millisecond& milliseconds) override;
//
//	void TriggerFinishLevelFirstStage() override;
//	void TriggerFinishLevelLastStage() override;
//
//	string StartMissionText() override;
//	string EndMissionText() override;
//
//	void StartLevel() override;
//	bool IsLevelFinished() override;
//
//private:
//
//	void AddRegions();
//	std::shared_ptr<LocationData> pLevelLocation;
//
//};