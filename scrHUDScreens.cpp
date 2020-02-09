#include"scrHUDScreens.h"
#include"Globals.h"
#include"StringBuilder.h"

scrHUDScreens::scrHUDScreens() : ScreenHandler()
{
	Init();
	this->AddScreen(std::shared_ptr<Screen>(new LauncherSelector(Coord(256 + 128 - 30, DEFAULT_SCREEN_HEIGHT - 59))), strLauncherList);
	//this->AddScreen(std::shared_ptr<Screen>(new scrHUDTargetList(Coord(256, 256), Coord(1024, 128))), strTargetList);

	auto playerMainHudScreen = new PlayerHUD(DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH, Coord(0, 0));

	this->AddScreen(std::shared_ptr<Screen>(playerMainHudScreen), strPlayerHUD);

	auto radarDisplay = std::shared_ptr<scrHUDDisplay>(new scrHUDDisplay(Coord(929 + 63, 529)));
	this->AddScreen(radarDisplay, strHUDRadarDisplay);
	this->AddScreen(std::shared_ptr<Screen>(new scrHUDDisplay( Coord(-13+63, 529))), strHUDShipDamageDisplay);
	
	this->AddScreen(std::shared_ptr<Screen>(new HUDSlider(Coord(-13, 666), enHUDSliderMeasurer::MeasureHitPoint)), strHUDHPSlider);
	this->AddScreen(std::shared_ptr<Screen>(new HUDSlider(Coord(929, 666), enHUDSliderMeasurer::MeasureAvailableThrust, SDL_FLIP_HORIZONTAL)), strHUDThrustSlider);
	this->AddScreen(std::shared_ptr<Screen>(new HUDProgressBar_1(Coord(240, 515), enHUDProgressBarMeasurer::MeasureBooster)), strHUDBoosterBar);

	playerMainHudScreen->RightExpandButtonClicked = [&]()
	{
		auto screen = GetRadarDisplay();
		if (screen->isActive())
			screen->CloseScreen();
		else
			screen->OpenScreen();
	};

	playerMainHudScreen->LeftExpandButtonClicked = [&]()
	{
		auto screen = GetShipDamageDisplay();
		if (screen->isActive())
			screen->CloseScreen();
		else
			screen->OpenScreen();
	};

	radarDisplay->LineClicked = [&](Widget* line)
	{
		//pCurrentPlayer->AsUnitEntity()->TargetVessel(targetVessel);
		//binding here is from list to ship, but there should also be a binding from link to ship!

	};


	GetPlayerHUD()->OpenScreen();
	GetLauncherList()->OpenScreen();
	GetHUDHPSlider()->OpenScreen();
	GetHUDThrustSlider()->OpenScreen();
	GetHudBoosterProgressBar()->OpenScreen();
	GetRadarDisplay()->OpenScreen();
	GetShipDamageDisplay()->OpenScreen();

	this->SelectNewScreen(  strPlayerHUD );
}

void scrHUDScreens::ResetHUDForInstance(std::shared_ptr<PlayerInterface> player)
{
	this->RefreshTargetList(player);
	this->GetLauncherList()->RefreshLauncherList();
	this->GetHUDHPSlider()->DefinePlayer(player->AsUnitEntity());
	if (player->IsSpaceVessel())
		this->GetHudBoosterProgressBar()->DefinePlayer(std::dynamic_pointer_cast<PlayerVessel>(player).get());
	this->GetHUDThrustSlider()->DefinePlayer(player->AsUnitEntity());
}

void scrHUDScreens::RefreshTargetList(std::shared_ptr<PlayerInterface> player)
{
	pCurrentPlayer = player;
	auto display = GetRadarDisplay();
	display->ResetLines();
	auto trackedTargets = player->GetTrackedTargets();

	for( auto it = trackedTargets.begin(); it != trackedTargets.end(); ++it )
	{
		if ((*it)->isDestroyed() == false && (*it)->IsSpawned() == true)
		{
			AddTargetToRadarList(*it);
		}
	}


}

void scrHUDScreens::AddTargetToRadarList(std::shared_ptr<UnitEntity> target)
{
	auto display = GetRadarDisplay();
	auto addedLine = display->AddLine(target->GetUnitName() + " : " + IntToString(target->GetID()), target->GetID());
	addedLine->OnWidgetSelected = [&](Widget* w)
	{
		pCurrentPlayer->AsUnitEntity()->TargetVessel(std::stoi(w->ID()));

	};

	auto targetSelectLink = target->LinkTarget.link;
	if (targetSelectLink != nullptr)
	{
		targetSelectLink->AddLinkToBindingList(addedLine);
		addedLine->AddLinkToBindingList(targetSelectLink);
		
	}
	//return item;
}

PlayerHUD* scrHUDScreens::GetPlayerHUD()
{
	return (PlayerHUD*)(this->GetScreenByName(strPlayerHUD).get());
}
LauncherSelector* scrHUDScreens::GetLauncherList()
{
	return (LauncherSelector*)(this->GetScreenByName(strLauncherList).get());
}

HUDProgressBar_1* scrHUDScreens::GetHudBoosterProgressBar()
{
	return (HUDProgressBar_1*)(this->GetScreenByName(strHUDBoosterBar).get());
}

HUDSlider* scrHUDScreens::GetHUDHPSlider()
{
	return (HUDSlider*)(this->GetScreenByName(strHUDHPSlider).get());
}
HUDSlider* scrHUDScreens::GetHUDThrustSlider()
{
	return (HUDSlider*)(this->GetScreenByName(strHUDThrustSlider).get());
}
scrHUDDisplay* scrHUDScreens::GetRadarDisplay()
{
	return (scrHUDDisplay*)(this->GetScreenByName(strHUDRadarDisplay).get());
}
scrHUDDisplay* scrHUDScreens::GetShipDamageDisplay()
{
	return (scrHUDDisplay*)(this->GetScreenByName(strHUDShipDamageDisplay).get());
}

void scrHUDScreens::HandleEvent( SDL_Event& e )
{
	ScreenHandler::HandleEvent(e);

	//if( HasScreenSelection() )
	//	this->GetSelectedScreen()->HandleEvent(e);
}

void scrHUDScreens::SwitchScreens(const Coord& mousePos)
{
	if(GetSelectedScreen() != nullptr && GetSelectedScreen() != GetPlayerHUD() && GetSelectedScreen()->PointWithinScreen(mousePos) ) //mouse still within screen
		return;

	//if( GetTargetList()->PointWithinScreen(mousePos) )
	//	this->SelectNewScreen(strTargetList);
	if (GetLauncherList()->PointWithinScreen(mousePos))
		this->SelectNewScreen(strLauncherList);
	else if (GetRadarDisplay()->PointWithinScreen(mousePos))
		this->SelectNewScreen(strHUDRadarDisplay);
	else
		this->SelectNewScreen(strPlayerHUD);
}


void scrHUDScreens::Init()
{
	strPlayerHUD = "playerhud";
	strLauncherList = "x_launcherlist";
	strHUDHPSlider = "sliderscreen_1";
	strHUDThrustSlider = "sliderscreen_2";
	strHUDBoosterBar = "boosterbar_1";
	strHUDRadarDisplay = "radarDisplay";
	strHUDShipDamageDisplay = "damagedisplay";
}