#pragma once
#include"WalkingEntity.h"
#include"PlayerInterface.h"
#include<set>

class PlayerWalker : public WalkingEntity, public PlayerInterface
{

public:

	PlayerWalker();
	PlayerWalker(const Coord& pos);

	void setStateToNormal() override;
	bool isDestroyed() override;

	void HandleUnitState(Uint32 milliseconds) override;

	void RefreshHotKeyList();
	void DockToTarget(DynamicEntity* target, ClickAction);
	void DockToPoint(const Coord point, double dockedHeading, ClickAction);

	void SetModulePlayerOwned();
	void handleWalkerState(Uint32 milliseconds) override;

	bool HasAmmo(string type) override;
	int AmmoAmount(string type);
	int AmmoAmount(std::shared_ptr<LauncherEntity> entity);
	bool DecrementAmmo(string type) override;

	static std::shared_ptr<PlayerWalker> GeneratePlayerWalkerFromWalkerName(string name);
	
	void RefreshInventory(){}
	void RefreshAmmoMap();

	std::vector<std::shared_ptr<UnitEntity>> GetTrackedTargets() override { return std::vector<std::shared_ptr<UnitEntity>>(); };
	void UpdateTrackedTargets() override {};

	bool IsSpaceVessel() override { return false; }

protected:

	void EnableAutoPilot();
	void EnableControlDisabled();
	void UpdateDiscreteHeading(bool masked) override;
	void ChangeExternalModuleDirection(bool bridge, bool frame, bool wings, double angle);
	void ResetRelativeHeading();

private:

	void initialize();

	std::set<SDL_Scancode> pHotKeyList;

	void HandleLauncherSelection(SDL_Event& e, SDL_Scancode& c);
	void HandleKeyButtonUp(SDL_Event& e);
	void HandleKeyButtonDown(SDL_Event& e);
	void HandleMouseUp(SDL_Event& e);
	void HandleMouseDown(SDL_Event& e);
	void HandleMouseSteering();

	std::map<string, int> pAmmoAmmountList;

};