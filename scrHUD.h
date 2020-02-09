#pragma once
#include "ScreenHandler.h"
#include"PlayerVessel.h"
#include"scrHUDTargetList.h"
#include"scrLauncherSelector.h"
#include<string>

using std::string;

class PlayerHUD : public Screen
{
public:

	PlayerHUD() {};
	PlayerHUD( int, int, Coord pos ); 

	void render( SDL_Renderer& );
	void HandleEvent( SDL_Event& e) override;
	
	void ResetHUDForInstance();

	std::function<void()> RightExpandButtonClicked;
	std::function<void()> LeftExpandButtonClicked;

private:

	std::shared_ptr<PlayerVessel> pPlayer;

	std::shared_ptr<Widget> pLocationLink;
	std::shared_ptr<Widget> pShipHeadingLink;
	std::shared_ptr<Widget> pShipPositionLink;
	std::shared_ptr<Widget> pShipVelocityLink;
	std::shared_ptr<Link> pGameSpeed;

	void RenderLocationTxt(SDL_Renderer& renderer);
	void RenderShipHeadingTxt( SDL_Renderer& renderer );
	void RenderShipPositionTxt( SDL_Renderer& );
	void RenderShipVelocityTxt( SDL_Renderer& renderer );

	static string pShipHeadingTxt;
	string pShipHeadingResult;

	static string pShipPositionTxt;
	string pShipPositionResult;

	static string pShipVelocityTxt;
	string pShipVelocityResult;

	static string pLocationText;
	string pLocationTextResult;

	//scrHUDTargetList pTargetList;
	//LauncherSelector pLauncherList;
	//ListFrame 
};

