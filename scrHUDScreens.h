#pragma once
#include"ScreenHandler.h"
#include"scrHUD.h"
#include"scrLauncherSelector.h"
#include"HUDSlider.h"
#include"HUDProgressBar.h"
#include"scrHUDDisplay.h"
#include"scrHUDTargetList.h"
#include"PlayerVessel.h"

class scrHUDScreens : public ScreenHandler
{

public:

	scrHUDScreens();

	void ResetHUDForInstance(std::shared_ptr<PlayerInterface> player);
	void RefreshTargetList(std::shared_ptr<PlayerInterface> player);

	HUDSlider* GetHUDHPSlider();
	HUDSlider* GetHUDThrustSlider();
	HUDProgressBar_1* GetHudBoosterProgressBar();
	scrHUDDisplay* GetRadarDisplay();
	scrHUDDisplay* GetShipDamageDisplay();
	PlayerHUD* GetPlayerHUD();
	LauncherSelector* GetLauncherList();

	void HandleEvent( SDL_Event& e ) override;

protected:

	void SwitchScreens(const Coord& mousePos) override;

private:

	std::shared_ptr<PlayerInterface> pCurrentPlayer;

	string strHUDBoosterBar;
	string strHUDHPSlider;
	string strHUDThrustSlider;
	string strPlayerHUD;	//temporary??? for all sorts of texts velocity and pos
	string strLauncherList;
	string strHUDRadarDisplay;
	string strHUDShipDamageDisplay;


	void AddTargetToRadarList(std::shared_ptr<UnitEntity> target);

	void Init();
};