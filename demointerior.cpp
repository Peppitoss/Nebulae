//#include"demolevel.h"
//#include"MaskEffector.h"
//#include"DialogueManager.h"
//#include"GameManager.h"
//
//demolevel::demolevel() : Level(nullptr)
//{
//	pIrgenLocation = std::shared_ptr<IrgenStation>(new IrgenStation());
//	pStarCruiser = std::shared_ptr<LocationStarCruiser>(new LocationStarCruiser());
//	pLevelStage = -1;
//	this->SetLevelID("demolevel");
//	/*pMaskEffector = std::shared_ptr<MaskEffector>(new MaskEffector(nullptr));*/
//}
//
//demolevel::demolevel(std::shared_ptr<MaskEffector> effector) : Level(effector)
//{
//	pIrgenLocation = std::shared_ptr<IrgenStation>(new IrgenStation());
//	pStarCruiser = std::shared_ptr<LocationStarCruiser>( new LocationStarCruiser());
//	pLevelStage = -1;
//
//	//pMaskEffector = effector;
//	//pMaskEffector->GenerateFadeAwayMask(ColorRGB(0,0,0), 100);
//	//pMaskEffector->GenerateBlendMask(ColorRGB(165, 112, 66), SDL_BlendMode::SDL_BLENDMODE_BLEND, 70);
//	//pMaskEffector->GenerateNoiseMask(18, 0);
//	this->SetLevelID("demolevel");
//
//	DialogueManager::LoadDialogues(CURRENTLANGUAGE, "/Irgen/dialog", true);
//	DialogueManager::GetDialogue("andrej")->SetPosition(pIrgenLocation->GetVesselFromName("andrej").GetCurrentPosition().yflip());	//Coord(260, 920));
//	//DialogueManager::GetDialogue("d1")->Enable(true);
//
//	DialogueManager::GetDialogue("oleus")->SetPosition(pIrgenLocation->GetVesselFromName("oleus").GetCurrentPosition().yflip());
//	//DialogueManager::GetDialogue("d2")->Enable(true);
//
//	DialogueManager::GetDialogue("guard")->SetPosition(pIrgenLocation->GetVesselFromName("orange suit").GetCurrentPosition().yflip());
//	//DialogueManager::GetDialogue("d3")->Enable(true);
//
//	DialogueManager::GetDialogue("commander")->SetPosition(pIrgenLocation->GetVesselFromName("commander").GetCurrentPosition().yflip());
//	//DialogueManager::GetDialogue("d4")->Enable(true);
//	AddRegions();
//	
//}
//
//void demolevel::AddRegions()
//{
//	Region reg = Region(0, -1000, 1000, 1000);
//	reg.SetName("demoRegion");
//	pRegionMap.AddRegion(reg);
//}
//
//void demolevel::RunLevel(const Millisecond& milliseconds)
//{
//	if (pLevelStage == 10)	//should be 0, but used as 10 to stop dialogue from running
//	{
//		auto test = GameManager::CurrentInstanceManager()->GetCurrentPlayer()->AsUnitEntity()->getPosition();
//		DialogueManager::GetDialogue("andrej")->RunDialoge(milliseconds.GetValue());
//		DialogueManager::GetDialogue("oleus")->RunDialoge(milliseconds.GetValue());
//		DialogueManager::GetDialogue("guard")->RunDialoge(milliseconds.GetValue());
//		DialogueManager::GetDialogue("commander")->RunDialoge(milliseconds.GetValue());
//
//
//		auto region = pRegionMap.GetRegion("demoRegion");
//		if (region.IsPointInRegion(test) && region.IsActive())
//		{
//
//		}
//
//		//shake effect
//		//music
//		
//
//	}
//
//
//	if (pLevelStage == 1)
//	{
//
//		//open dialog for mission
//
//	}
//}
//
//
//void demolevel::StartLevel()
//{
//
//}
//
//bool demolevel::IsLevelFinished()
//{
//	return false;
//}
//
//std::shared_ptr<LocationData> demolevel::GetStartLocation()
//{
//	return pIrgenLocation;
//}
//
//std::shared_ptr<LocationData> demolevel::GetLocation()
//{
//	if (pLevelStage == -1)
//	{
//		pLevelStage++;
//		return pIrgenLocation;
//	}
//	else if (pLevelStage == 0)
//	{
//		pMaskEffector->ResetEffects();
//		pMaskEffector->GenerateFadeAwayMask(ColorRGB(0, 0, 0), 100);
//		pLevelStage++;
//		return pStarCruiser;
//	}
//		
//}