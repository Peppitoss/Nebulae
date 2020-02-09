//#pragma once
//#include"Level.h"
//#include"IrgenStation.h"
//#include"LocStarCruiser.h"
//
//class demolevel : public Level
//{
//
//public:
//
//	demolevel();
//	demolevel(std::shared_ptr<MaskEffector> effector);
//
//	std::shared_ptr<LocationData> GetLocation() override;
//	std::shared_ptr<LocationData> GetStartLocation() override;
//
//	void RunLevel(const Millisecond& milliseconds) override;
//
//	void StartLevel() override;
//	bool IsLevelFinished() override;
//
//private:
//
//	void AddRegions();
//	std::shared_ptr<IrgenStation> pIrgenLocation;
//	std::shared_ptr<LocationStarCruiser> pStarCruiser;
//
//};