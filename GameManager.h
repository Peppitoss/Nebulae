#pragma once
#include <iostream>

#include"GameLoop.h"
#include"Animation.h"
#include"InstanceManager.h"

#include"PlayerWalker.h"
#include"OkCancelForm.h"
#include"WorldManager.h"
#include"AmmoContainer.h"
#include"Location.h"
#include"MaskEffector.h"
#include"FileReader.h"
#include"PlayerProfile.h"
#include"ProfileWriter.h"
#include"ProfileReader.h"
#include"ScreenManager.h"
//#include"MessageTablet.h"

//class scrBuilder;
class scrPlayerInventory;
class InstanceManager;
class InGameMenu;
class MainMenu;
class scrProfileSelectMenu;
class PlayerHUD;
class scrStore;
class Campaign; 




class GameManager
{

public:

	GameManager( SDL_Renderer* , int wWidth, int wHeight);

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	

	//static SDL_Renderer* getRenderer() { return GameManager::pDrawer.; };

	static std::shared_ptr<Animator> getSAnimator();
	static std::shared_ptr<MaskEffector> getMaskEffector();
	static std::shared_ptr<particleEffects::ParticleEngine> GetParticleEngine();

	//static void LoadProfile(string profileName);

	static void SetupSpaceInstance(Location&, bool useCurrentPlayer);
	bool Run();
	static void InitiateEndCurrentLevel();
	static void HandleCurrentLevelFinished();

	//static void UpdatePlayerVesselFromProfile();
	static void SaveCurrentPlayerProfile(Location);
	//static void setPlayerVessel(std::weak_ptr<PlayerVessel>	 set) {currentPlayer = set;  };
	//static std::weak_ptr<PlayerVessel> getPlayerVessel() { return GetPlayerProfile()->GetCurrentPlayerUnit(); }

	//static void setPlayerWalker(std::weak_ptr<PlayerWalker>	 set) { currentPlayerWalker = set; };
	//static std::weak_ptr<PlayerWalker> getPlayerWalker() { return currentPlayerWalker; }
	~GameManager();
	 

	static ScreenManager Screens;

	static string CurrentLocationName();

	// SCREEN HANDLES ----------------------------------------------------------------------------------------------------------------------------------


	static void QuitGame();
	static void restart();
	static void ImportChassis( string name );
	static void saveVessel( string name );
	static void loadVessel( string name );

	static void loadModuleBuilder( string name );

	static void HandleDockAction(ClickAction&);

	static void StartDialogue(string name);
	static void executeTransaction();
	static void revertTransaction();

	static void RefreshHUDVesselList();
	//static void RefreshHUDLauncherList();

	//static bool LoadLevel(string name);
	/*static void LoadLocation( std::string locationName);*/
	//static std::shared_ptr<LocationData> GetStartLocationFromMission(string name);

	static void pauseGame();
	static void unPauseGame();


	static void startMission();
	//static void TravelTo( LocationData );
	static void TravelTo(const Coord& solarsystemposition);
	static void TravelTo(string name);
	static void TravelTo(Location);
	static void ShowMenuBackground();

	static void ResetBackgroundLevel();

	static void SelectNextTarget(SDL_Event & e);
	static void SelectPreviousTarget(SDL_Event & e);

	static int GetIntegratorSpeed();

	static PlayerProfile GetPlayerProfile() { return pCurrentProfile; }
	static void SetPlayerProfile(PlayerProfile profile);
	static std::shared_ptr<PlayerVessel> ResetPlayerProfile();

	//static openOkCancel();
	//static closeOkCancel();
	//static 
	static InstanceManager* CurrentInstanceManager() { return GameManager::pInstanceManager; };
	static Coord GetMousePositionFromZoomLevel(const Coord& mouseInput);

private:

	static GameLoop pGameLoop;
	//static MainMenu* pMainMenu;
	
	//static PlayerHUD* pShipHUD;
	//static std::weak_ptr<PlayerVessel> currentPlayer;
	static PlayerProfile pCurrentProfile;
	static std::weak_ptr<PlayerWalker> currentPlayerWalker;
	static std::shared_ptr<Vessel> buildVessel;
	static bool CanUnpause();

	static Location pCurrentLocation;
	static int pIntegratorSpeed;

	static ProfileWriter pProfileWriter;
	static ProfileReader pProfileReader;
	//static WorldManager pWorldManager;
	//static OkCancelForm* okcancelForm;

	//static double playerSize;
	//static Coord playerPosition;

	//static void handlePlayerState();
	
	static void Setuplocation(Location data);
	//static void openMessageScreen(MessageTabletID msgID, string message, string buttonText);

	//static Level* GetLevelFromMissionName(string name);

	bool SetupDrawer(SDL_Renderer*, int, int);
	void close();

	void loadInstance();
	static void TryReadFile(string fileName, FileReader& reader);

	SDL_Window* gWindow;// = NULL;
	//static SDL_Renderer* pRenderer;

	static SpriteDrawer pDrawer; //only one
	static std::shared_ptr<Animator> ANIMATOR; //only one
	static std::shared_ptr<particleEffects::ParticleEngine> PARTICLEENGINE;
	static std::shared_ptr<MaskEffector> MASKEFFECTOR;

	static InstanceManager* pInstanceManager;
	//static std::shared_ptr<ScreenTablet> pCurrentViewedTablet;

	//void HandleAllScreens(SDL_Event& e);
	//void handleInventoryScreens(SDL_Event& e);
	//void renderAllScreens();

	//void AddScreenToList( Screen* scr );
	//void AddScreensToList( ScreenHandler* scr );
	//void removeScreenFromList(Screen* scr);
	//std::list<Screen*> screenList;
	//std::list<ScreenHandler*> screenHandlerList;

	//TransferGridObject UserGridObjectTransfer;

	//static string pCUrrentLocationName;
	//static bool pQuitGame;


	//static void UpdateZoomCameraCenterModifierInverse(int sWidth, int sHeight);

	/*void HandleMouseZoomEvent(SDL_Event& e);*/
	//void HandleHUDControls(SDL_Event& e);
	SDL_SpinLock* test;

	//void HandleMouseZoomEvent(SDL_Event& e);

	//inventory items: store in own static collection??
	//static InventoryItem* invScrapBox;

};