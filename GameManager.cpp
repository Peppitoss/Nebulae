#include"GameManager.h"

#include"ClickAction.h"
#include"SurfaceGlobalList.h"
#include"VesselGlobalList.h"
#include"ModuleInterfaceGlobalList.h"
#include"RandomNumberGenerator.h"
#include"InteriorGlobalList.h"
#include"LocationCommand.h"

//#include"LocStarCruiser.h"
//#include"LocSatelliteCasper.h"
#include"DialogueManager.h"
#include"ReaderException.h"
#include"ProfileGlobalList.h"

#include"LocationCollection.h"

//#include"demolevel.h"
//#include"ShuttleAid_Level_01.h"

#include"MissionCaptionCollection.h"


PlayerProfile		GameManager::pCurrentProfile;
std::shared_ptr<Vessel>				GameManager::buildVessel = nullptr;
InstanceManager*					GameManager::pInstanceManager;
Location							GameManager::pCurrentLocation = WorldManager::GetStartLocation();
//ProfileWriter						GameManager::pProfileWriter = ProfileWriter();
ProfileReader						GameManager::pProfileReader = ProfileReader();
int									GameManager::pIntegratorSpeed = 100;
ScreenManager						GameManager::Screens = ScreenManager();
SpriteDrawer						GameManager::pDrawer = SpriteDrawer();
GameLoop							GameManager::pGameLoop = GameLoop(GameManager::pDrawer);
std::shared_ptr<particleEffects::ParticleEngine> GameManager::PARTICLEENGINE;
std::shared_ptr<Animator> GameManager::ANIMATOR;
std::shared_ptr<MaskEffector> GameManager::MASKEFFECTOR;

int GameManager::SCREEN_WIDTH = 0;
int GameManager::SCREEN_HEIGHT = 0;
//
//string GameManager::pCUrrentLocationName = "";

GameManager::GameManager( SDL_Renderer* gRenderer, int windowHeight, int  windowWidth)
{
	this->SCREEN_HEIGHT = windowHeight;
	this->SCREEN_WIDTH = windowWidth;
	//this->pRenderer = gRenderer;

	GameManager::PARTICLEENGINE = std::shared_ptr<particleEffects::ParticleEngine>(new particleEffects::ParticleEngine());


	WorldManager::GenerateWorld(1);

	//gWindow = NULL;
	SetupDrawer( gRenderer, windowWidth, windowHeight);
	//pGameLoop.DefineDrawer(this->pDrawer);

	pInstanceManager = new InstanceManager( this->pDrawer.getRenderer() );
	pGameLoop.DefineInstanceManager(std::shared_ptr<InstanceManager>(pInstanceManager));
	FileReader reader = FileReader();
	reader.LoadFolder("../Data/Surface/");
	reader.IterateEntireFolder(".txt");

	TryReadFile("ModuleList", reader);
	TryReadFile("ProjectileList", reader);
	TryReadFile("VesselList", reader);
	TryReadFile("InteriorList", reader);
	TryReadFile("WalkerList", reader);

	reader.LoadFolder("../Data/Shop/");
	reader.IterateEntireFolder(".txt");

	ProfileGlobalList::LoadProfilesDatas();

	GameManager::pCurrentProfile = GameManager::pProfileReader.GetTestProfile();

	this->ANIMATOR = std::shared_ptr<Animator>(new Animator());
	this->MASKEFFECTOR = std::shared_ptr<MaskEffector>(new MaskEffector(gRenderer));

	pInstanceManager->SetupManager( this->ANIMATOR, GameManager::PARTICLEENGINE, GameManager::MASKEFFECTOR);

	GameManager::Screens = ScreenManager(SCREEN_WIDTH, SCREEN_HEIGHT);

	auto lastPlayer = GameManager::pCurrentProfile.GetLastSavedPlayerVessel();

	if (lastPlayer != nullptr)
	{
		Screens.UpdateInventoryShipTemplate(lastPlayer);
		pInstanceManager->GetIntegrator()->unpause();
	}
	GameManager::Screens.quitToMenu(false);

}

void GameManager::TryReadFile(string fileName, FileReader& reader)
{
	reader.LoadFile("../Data/", fileName);
	reader.IterateEntireFile();

}

Coord GameManager::GetMousePositionFromZoomLevel(const Coord& mouseInput)
{
	return Coord(mouseInput.x*GameManager::pDrawer.GetCurrentGlobalZoomLevel(), mouseInput.y * GameManager::pDrawer.GetCurrentGlobalZoomLevel());
}

void GameManager::SelectNextTarget(SDL_Event & e)
{
	Screens.GetHUD()->GetRadarDisplay()->SelectNextItemInList();
}
void GameManager::SelectPreviousTarget(SDL_Event & e)
{
	Screens.GetHUD()->GetRadarDisplay()->SelectPreviousItemInList();
}


bool GameManager::Run()
{
	bool result = this->pGameLoop.Run_2();
	this->MASKEFFECTOR->Run(this->pGameLoop.GetCurrentTimestep().Value);

	return result;
}

void GameManager::HandleCurrentLevelFinished()
{
	//continue here, trigger campaign "manager" that saves the mission complete, and generates a new instance of levels available to the player
	GameManager::SaveCurrentPlayerProfile(GameManager::pCurrentLocation);
	//GameManager::LoadLocation("LocationStarCruiser");
}

void GameManager::InitiateEndCurrentLevel()
{
	GameManager::pInstanceManager->GetCurrentPlayer()->AsUnitEntity()->SetInvulnerable(true);
	GameManager::Screens.openCompleteMissionText();
}

void GameManager::HandleDockAction(ClickAction& da)
{
	switch (da.GetType())
	{
	case(ClickActionType::daChangeLocation):
	{
		GameManager::TravelTo(da.GetLocation());
	}
	break;
	case(ClickActionType::daOpenTablet):
	{
		GameManager::Screens.OpenTablet(da.GetTabledName());
	}
	break;
	case(ClickActionType::daOpenScreen):
	{
		
	}
	break;
	case(ClickActionType::daDialogue):
	{
		GameManager::StartDialogue(da.GetTabledName());
	}
	break;
	case(ClickActionType::daOpenStore):
	{
		GameManager::Screens.openStoreScreen(da.GetStoreName());
	}
	break;
	case(ClickActionType::daSelectShip):
	{
		GameManager::Screens.openShipSelectScreen(da.GetTabledName());
	}
	break;
	case(ClickActionType::daUnknown):
	{

	}
	break;
	}
}


string GameManager::CurrentLocationName()
{
	return GameManager::pCurrentLocation.GetName();
}

void GameManager::SaveCurrentPlayerProfile(Location location)
{
	GameManager::GetPlayerProfile().SaveCurrentPlayerInstance( InstanceManager::GetCurrentPlayer() );
	if (location.GetName() == location.EMPTYLOCATION().GetName())
	{
		GameManager::GetPlayerProfile().SaveLevelFinished(location.GetLevelFinishStats());

	}
	//GameManager::pProfileWriter.WriteProfile(GameManager::GetPlayerProfile().get());
}

void GameManager::QuitGame()
{
	GameManager::pauseGame();
	GameManager::SaveCurrentPlayerProfile(Location::EMPTYLOCATION());

	//pQuitGame = true;
	pGameLoop.DisableEventRun();
}

void GameManager::restart()
{
	GameManager::pInstanceManager->GetIntegrator()->pause();
	auto data = GameManager::pInstanceManager->CurrentLocation();
	if (false ) //data->UsePlayerWalker())
	{
		GameManager::pInstanceManager->SetupInteriorInstance(data, GameManager::GetPlayerProfile().GetLastSavedPlayerWalker(),true);
	}
	else
	{
		auto locationVessel = GameManager::ResetPlayerProfile();
		GameManager::pInstanceManager->SetupSpaceInstance(data, locationVessel,true);
	}
	GameManager::pInstanceManager->ResetSpeed();
	GameManager::pInstanceManager->GetIntegrator()->unpause();
}

bool GameManager::SetupDrawer(SDL_Renderer* gRenderer, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	pDrawer = SpriteDrawer(gRenderer, camera );
	pDrawer.SetScreenHeight(SCREEN_HEIGHT);
	pDrawer.SetScreenWidth(SCREEN_WIDTH);

	return true;
}

std::shared_ptr<Animator> GameManager::getSAnimator()
{
	return GameManager::ANIMATOR;
}

std::shared_ptr<MaskEffector> GameManager::getMaskEffector()
{
	return GameManager::MASKEFFECTOR;
}

std::shared_ptr<particleEffects::ParticleEngine> GameManager::GetParticleEngine()
{
	return GameManager::PARTICLEENGINE;
}

void GameManager::close()
{
    //Destroy window    
    //SDL_DestroyWindow( gWindow );
    //gWindow = NULL;

	//delete DRAWER;
	//delete ANIMATOR;
	//delete PARTICLEENGINE;
	delete pInstanceManager;
	//delete pShipHUD;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void GameManager::ImportChassis( string name )
{
	std::shared_ptr<Vessel> tempVessel = std::shared_ptr<Vessel>( new Vessel() );
	std::shared_ptr<FiguredGraphics> vesselGe = std::shared_ptr<FiguredGraphics>(  SurfaceGlobalList::GetChassisGEntity( ModuleGlobalList::GetInterface(name).GetSurfaceName()) );
	tempVessel->DefineChassis(vesselGe, name);
	GameManager::buildVessel = tempVessel;
	GameManager::Screens.GetShipBuilder()->ImportVessel(GameManager::buildVessel);
}

void GameManager::saveVessel(string name)
{
	GameManager::Screens.GetShipBuilder()->SaveVessel(name);
}
void GameManager::loadVessel( string name )
{
	Vessel* newVessel = VesselGlobalList::GetVessel(name);
	GameManager::buildVessel = std::shared_ptr<Vessel>(newVessel);
	GameManager::Screens.GetShipBuilder()->LoadVessel(GameManager::buildVessel);
}

void GameManager::loadModuleBuilder( string name )
{
	//if( pStore->GetInventoryFrame()->isActive() )
	//	pStore->openStore();
	//else
	GameManager::Screens.GetShipBuilder()->LoadModule( name );
}

void GameManager::pauseGame()
{
	GameManager::pInstanceManager->GetIntegrator()->pause();
}

int GameManager::GetIntegratorSpeed()
{
	return GameManager::pIntegratorSpeed;
}

void GameManager::SetPlayerProfile(PlayerProfile profile)
{ 
	pCurrentProfile = profile; 
	auto vessel = pCurrentProfile.GetLastSavedPlayerVessel();
	GameManager::Screens.GetInventory()->GetInventory()->generateShipTemplate(vessel);
	GameManager::Screens.GetStore()->GetInventoryFrame()->generateShipTemplate(vessel);

}

std::shared_ptr<PlayerVessel> GameManager::ResetPlayerProfile()
{
	auto locationVessel = GameManager::GetPlayerProfile().GetLastSavedPlayerVessel();
	GameManager::Screens.GetInventory()->GetInventory()->generateShipTemplate(locationVessel);
	GameManager::Screens.GetStore()->GetInventoryFrame()->generateShipTemplate(locationVessel);
	return locationVessel;
}

void GameManager::ShowMenuBackground()
{
	TravelTo(LocationCollection::BakLocationCasper);
}

void GameManager::TravelTo(const Coord& solarsystemposition)
{
	TravelTo(WorldManager::GetLocation(solarsystemposition));
}

void GameManager::TravelTo(string locationName)
{
	//if (data->IsMission() && GameManager::LoadLevel(data->GetMissionName()))
	//	GameManager::pCurrentLevel->StartLevel();
	//else
	//{
	//custom location,
	//WorldManager::(this->GetGrid()->GetSelectedLocationData().GetLocation());
	TravelTo(WorldManager::GetLocation(locationName));

	//	GameManager::pCurrentLevel = nullptr;
	//}
}

void GameManager::TravelTo(Location data)
{
	//if (data->IsMission() && GameManager::LoadLevel(data->GetMissionName()))
	//	GameManager::pCurrentLevel->StartLevel();
	//else
	//{
		//custom location,
		//WorldManager::(this->GetGrid()->GetSelectedLocationData().GetLocation());
		Setuplocation(data);
		WorldManager::MakeLocationVisited(data.GetName());
	//	GameManager::pCurrentLevel = nullptr;
	//}
}

void GameManager::ResetBackgroundLevel()
{
	ShowMenuBackground();
}

void GameManager::Setuplocation(Location location)
{
	LocationData& data = location.GetData();
	data.InitializeLocation();

	if (location.GetData().IsInterior())
		pDrawer.GetCamera().ResetZoomLevel(enZoomLevel::ZoomCLOSE);
	else
		pDrawer.GetCamera().ResetZoomLevel(enZoomLevel::ZoomNORMAL);

	MASKEFFECTOR->ResetEffects();
	bool resetLocation = GameManager::pCurrentLocation.Type == enLocationType::enBackgroundLocation ||
						location.Type == enLocationType::enFriendly;

	GameManager::pCurrentLocation = location;
	if (false ) //data->UsePlayerWalker())
	{
		GameManager::pInstanceManager->SetupInteriorInstance(location, GameManager::GetPlayerProfile().GetLastSavedPlayerWalker(), resetLocation);
	}
	else
	{
		auto locationVessel = pCurrentProfile.GetProfileVessel();
		GameManager::pInstanceManager->SetupSpaceInstance(location, locationVessel, resetLocation);
	}

	if (data.IncludePlayerInLocation() == false)
		Screens.closeHUD();
	else
	{
		GameManager::Screens.openHUD();
		GameManager::Screens.GetHUD()->ResetHUDForInstance(GameManager::pInstanceManager->GetCurrentPlayer());
	}
}

void GameManager::startMission()
{
	GameManager::pInstanceManager->GetCurrentPlayer()->AsUnitEntity()->SetInvulnerable(false);
	GameManager::Screens.closeDestination();
	GameManager::Screens.closeCampaign();
}

void GameManager::unPauseGame()
{
	if(CanUnpause())
		GameManager::pInstanceManager->GetIntegrator()->unpause();
}

bool GameManager::CanUnpause()
{
	return GameManager::Screens.GetInventory()->GetInventory()->GetScreenState() == enScreenState::SCREEN_CLOSED &&
		GameManager::Screens.GetMenu()->GetScreenState() == enScreenState::SCREEN_CLOSED;
}

void GameManager::StartDialogue(string name)
{
	DialogueManager::ExecuteDialogueConversation(name);
	//GameManager::pCurrentViewedTablet->AddScreen(TabletGenerator::GenerateTablet(name), "tablet", true);
	//GameManager::pCurrentViewedTablet->OpenAllScreens();
	//GameManager::pauseGame();
}

void GameManager::RefreshHUDVesselList()
{
	GameManager::pInstanceManager->GetCurrentPlayer()->UpdateTrackedTargets();
	GameManager::Screens.GetHUD()->RefreshTargetList(GameManager::pInstanceManager->GetCurrentPlayer());//ResetHUDForInstance(GameManager::pInstanceManager->GetCurrentPlayer());
}

//void GameManager::RefreshHUDLauncherList()
//{
//	//GameManager::Screens.GetHUD()->GetLauncherList()->UpdateLaunchersState();
//}

//std::shared_ptr<LocationData> GameManager::GetStartLocationFromMission(string name)
//{
//	auto mission = GameManager::GetLevelFromMissionName(name);
//	if (mission == nullptr)
//		return nullptr;
//	return mission->GetStartLocation();
//}

//void GameManager::LoadLocation(std::string locationName)
//{
//	//if (handleDialog)
//	//{
//	//	//open dialog code
//	//	if (false)
//	//		return;
//	//}
//	GameManager::TravelTo(WorldManager::GetLocation(locationName));
//
//	//if (locationName == "command")
//	//{
//	//	std::shared_ptr<LocationCommand> loc = std::shared_ptr<LocationCommand>(new LocationCommand());
//	//	GameManager::TravelTo(location);
//	//}
//	//if (locationName == "irgen")
//	//{
//	//	std::shared_ptr<LocationCommand> loc = std::shared_ptr<LocationCommand>(new LocationCommand());
//	//	GameManager::TravelTo(location);
//	//}
//	//if (locationName == "LocationStarCruiser")
//	//{
//	//	std::shared_ptr<LocationStarCruiser> loc = std::shared_ptr<LocationStarCruiser>(new LocationStarCruiser());
//	//	GameManager::TravelTo(location);
//	//}
//	//if (locationName == "Mission")
//	//{
//	//	GameManager::TravelTo(pCurrentLevel->GetLocation());
//	//}
//	//if (locationName == "MenuBackground")
//	//{
//	//	std::shared_ptr<LocSatelliteCasper> loc = std::shared_ptr<LocSatelliteCasper>(new LocSatelliteCasper());
//	//	GameManager::TravelTo(loc);
//	//}
//}

//bool GameManager::LoadLevel(string name)
//{
//	if (GameManager::pCurrentLevel != nullptr)
//		delete GameManager::pCurrentLevel;
//
//	if (name == "test")
//		GameManager::pCurrentLevel = new demolevel();
//	else
//		GameManager::pCurrentLevel = GetLevelFromMissionName(name);
//
//	if (GameManager::pCurrentLevel != nullptr)
//	{
//		Setuplocation(GameManager::pCurrentLevel->GetLocation());
//		return true;
//	}
//
//	return false;
//}
//
//Level* GameManager::GetLevelFromMissionName(string name)
//{
//	if(name == "BackgrounLevel")
//		return new Background_Level_00(GameManager::getMaskEffector(), std::shared_ptr<LocSatelliteCasper>(new LocSatelliteCasper()));
//	if (name == MissionName::Mission_1)
//		return new ShuttleAid_Level_01(GameManager::getMaskEffector());
//	if (name == MissionName::Mission_2)
//		return nullptr;
//	if (name == MissionName::Mission_3)
//		return nullptr;
//	return nullptr;
//}

void GameManager::executeTransaction()
{
	if (GameManager::Screens.GetStore()->IsActive())
		GameManager::Screens.GetStore()->executeShopTransaction();
	//GameManager::Screens.CloseTabletScreens(); // pCurrentViewedTablet->CloseAllScreens();

}
void GameManager::revertTransaction()
{
	if (GameManager::Screens.GetStore()->IsActive())
		GameManager::Screens.GetStore()->ExecuteRevertTransaction();
	GameManager::unPauseGame();
}

GameManager::~GameManager()
{
	close();
}

