#pragma once
#include"Timer.h"
#include"Vessel.h"
#include"PlayerInterface.h"
#include"ClickAction.h"
#include<set>

class VesselAI;

//enum enumDockingType
//{
//	DOCKNONE = 0,
//	DOCKSTORE = 1,
//	DOCKCOMMAND = 2,
//	DOCKBAR = 3,
//	DEMO_SCREEN_HYDRA = 4,
//	DEMO_SCREEN_CORSAIR = 5,
//	DEMO_SCREEN_PILOTEMBLEM = 6,
//	DEMO_SCREEN_DRAWING = 7,
//	DEMO_SELECT_SHIP = 8
//};



class PlayerVessel : public Vessel, public PlayerInterface
{
public:

	PlayerVessel();
	PlayerVessel(Coord pos);

	void initialize();
	//void handleCtrlEvent( SDL_Event& e );
	void move(Uint32 milliseconds);
	//void moveTo( DynamicEntity* target );
	void DockToTarget( DynamicEntity* target, ClickAction);
	void DockToPoint( const Coord point, double dockedHeading, ClickAction);
	void HandleUnitState(Uint32 milliseconds) override;
	enumPlayerState getPlayerState() { return pState;};

	void AddWPlatform( std::shared_ptr<LauncherEntity> in, Coord pos, Coord anchorPoint ) override;

	bool autoPilotOn();
	void setStateToNormal() override;

	void DetachModule(std::shared_ptr<ExternalModule> toRemove, const primitives::vector& direction) override;

	//void SetRefreshInventory( bool set ) { pRefresInventory = true; };
	//bool RequireRefresh() { return pRefresInventory; };

	void TriggerBooster() override;
	virtual void RefreshInventory() override; 

	bool IsSpaceVessel() override { return true; }

	void TargetVessel( std::shared_ptr<UnitEntity> target ) override;
	void AddTarget(std::shared_ptr<UnitEntity> target) override;
	void RemoveTarget() override;
	std::vector<std::shared_ptr<UnitEntity>> GetTrackedTargets() override;
	void UpdateTrackedTargets() override;
	void AddRadarTargetFromNotTrackedSet(const std::vector<std::shared_ptr<RadarEntity>>& entity);
	void AddRadarTarget(std::shared_ptr<Vessel> reference );
	void RunRadarTargetTracking( double milliseconds ); 

	void ResetRadarEntities();

	static std::shared_ptr<PlayerVessel> GeneratePlayerVesselFromVesselName(string vesselName);

	//void SelectLauncher(int launcherID);
	//void SelectLauncher(string launcherTextID);
	void SelectLauncher(std::shared_ptr<LauncherEntity> entity);

	bool isDestroyed() override;
	void RefreshHotKeyList();

	void SetModulePlayerOwned();
	//void SetPlayerState(enumPlayerState state);

	bool HasAmmo(string type) override;
	int AmmoAmount(string type);
	int AmmoAmount(std::shared_ptr<LauncherEntity> entity);
	bool DecrementAmmo(string type) override;

	void RefreshAmmoMap();

	EntityPrice GetTotalPrice(bool includeShipValue) const override;
	statistics::RandomNumber PlayerShipValue() const;

	double GetBoosterProgressRatio();

protected:

	//bool pRefresInventory;

	VesselAI* autoPilot;
	//void autoPilotMoveToTarget( DynamicEntity* target  );
	void autoPilotMoveToPoint( const Coord point, double finalHeading );

	void EnableAutoPilot();
	void EnableControlDisabled();

private:

	//std::unordered_map<SDL_Keycode, std::vector<std::shared_ptr<LauncherEntity>> > pHotKeyRegister;
	std::set<SDL_Scancode> pHotKeyList;
	int pBoostInterval;

	void HandleLauncherSelection(SDL_Event& e, SDL_Scancode& c);
	void HandleKeyButtonUp(SDL_Event& e);
	void HandleKeyButtonDown(SDL_Event& e);
	void HandleMouseUp(SDL_Event& e);
	void HandleMouseDown(SDL_Event& e);
	void HandleMouseSteering();

	std::map<string, int> pAmmoAmmountList;
	void DefaultControllerGains();
	void SetRotationDriveToStable();

	std::set<std::shared_ptr<Vessel>> pEntitiesNotTracked;
	timer pBoosterTimer;

	//void HandleMouseWheel(SDL_Event& e);

};