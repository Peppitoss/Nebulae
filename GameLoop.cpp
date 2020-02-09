#include"GameLoop.h"
#include"Globals.h"
#include"GameManager.h"
#include"DialogueManager.h"
#include <iostream>

GameLoop::GameLoop(SpriteDrawer& drawer) : pDrawer(drawer)
{
	pInstanceManager = nullptr;
	Initialize();
}

//void GameLoop::DefineDrawer(SpriteDrawer& drawer)
//{
//	pDrawer = drawer;
//}

void GameLoop::Initialize()
{
	FPSHandler = timer();
	PreferredFPS = 60;
	secPrFrame = (double)1 / PreferredFPS;
	msPrFrame = secPrFrame * 1000;
	pDisableEventRun = false;
}

bool GameLoop::CurrentLevelActive()
{
	return true;
}

bool GameLoop::Run_2()
{
	pCurrentTimestep = pLoopTimer.GetStepTime();
	pLoopTimer.StartTick();

	Background* currentBackground = pInstanceManager->getBackground();
	bool quitGame = !RunEvents(pInstanceManager->GetIntegrator());
	integrator* currentIntegrator = pInstanceManager->GetIntegrator().get();	//integrator might change in event (due to change location) therefore this line is after RunEvents(..)

	if (quitGame)
		return false;

	try
	{
		Millisecond dt = 10;
		pAccumulator += pCurrentTimestep;

		while (pAccumulator >= dt)
		{
			RunIntegrator(currentIntegrator, dt.Value);
			pInstanceManager->HandleLocationChange();
			pAccumulator -= dt;
		}

		RunDrawer(currentBackground, pInstanceManager.get());
	}
	catch (std::runtime_error & e)
	{
		std::cerr << e.what();
	}

	pLoopTimer.StopTick();
	return true;
}

bool GameLoop::Run()
{
	pLoopTimer.StartTick();

	Background* currentBackground = pInstanceManager->getBackground();
	bool quitGame = !RunEvents(pInstanceManager->GetIntegrator());
	integrator* currentIntegrator = pInstanceManager->GetIntegrator().get();	//integrator might change in event (due to change location) therefore this line is after RunEvents(..)

	if (quitGame)
		return false;

	bool runDrawer = (FPSHandler.TickSumEqual(msPrFrame));

	try
	{
		RunIntegrator(currentIntegrator, 10);
		if (runDrawer)
		{
			RunDrawer(currentBackground, pInstanceManager.get());
		}
	}
	catch (std::runtime_error & e)
	{
		std::cerr << e.what();
	}
	pLoopTimer.StopTick();
	pCurrentTimestep = pLoopTimer.GetStepTime();
	return true;
}

bool GameLoop::RunEvents(std::shared_ptr<integrator> integrator)
{
	while (SDL_PollEvent(&sdlevent) != 0)
	{
		if (sdlevent.type == SDL_QUIT || pDisableEventRun)
		{
			return false;
		}
		if (pDisableEventRun == false && !integrator->IsDisabled())
		{
			integrator->HandleEvents(sdlevent, pDrawer.GetCameraPosition());
			HandleMouseZoomEvent(sdlevent);
			HandleHUDControls(sdlevent);
		}

		GameManager::Screens.EventHandler(sdlevent);
	}
	return true;
}
void GameLoop::RunIntegrator(integrator* integrator, int stepTime)
{
	bool run = integrator->run(stepTime);
	if (run)
	{
		//if (CurrentLevelActive())
		//	pCurrentLevel->Run(integrator->currentStepTime);

		auto particleEngine = GameManager::GetParticleEngine();
		particleEngine->clearInactiveEffects();
		particleEngine->clearInactiveParticles();
		particleEngine->UpdateParticleEffectList();
	}
}
void GameLoop::RunDrawer(Background* currentBackground, InstanceManager* instancemanager )
{
	/*FPSHandler.refreshTimer();*/
	auto currentIntegrator = instancemanager->GetIntegrator().get();
	auto currentLocation = instancemanager->CurrentLocation();
	auto previousLocation = instancemanager->PreviousLocation();

	pDrawer.clearScreen();
	GameCamera& camera = pDrawer.GetCamera();
	camera.SetCameraCenterPoint(currentIntegrator->PlayerPosition().yflip() - camera.GetCameraCenterModifier());
	camera.SetCameraScreenSize(camera.GetZoomLevel());
	pDrawer.drawBackground(currentBackground);
	



	SDL_RenderSetScale(pDrawer.getRenderer(), pDrawer.GetCamera().GetZoomDrawPositionModifier(), pDrawer.GetCamera().GetZoomDrawPositionModifier());
	if (currentLocation.IsEmptyLocation() == false)
		currentLocation.RenderLocationBorder(this->pDrawer.getRenderer(), pDrawer.GetCameraPosition());
	if (previousLocation.IsEmptyLocation() == false)
		previousLocation.RenderLocationBorder(this->pDrawer.getRenderer(), pDrawer.GetCameraPosition());

	pDrawer.DrawMainObjectsInIntegrator(currentIntegrator);
	pDrawer.drawPEffect(*GameManager::GetParticleEngine(), ORIGO);
	pDrawer.drawMaskEffects(*GameManager::getMaskEffector());

	SDL_RenderSetScale(pDrawer.getRenderer(), 1, 1);


	camera.ScaleCameraPoint(camera.GetZoomDrawPositionModifier());


	//DialogueManager::SetZoomModifier(pDrawer.GetCamera().GetZoomGlobalPositionModifier(enZoomLevel::ZoomCLOSE)); //already set
	pDrawer.drawDialogues(camera.GetZoomDrawPositionModifier());
	RenderAllScreens();
	//if (GameManager::pCurrentViewedTablet.get())
	//	GameManager::pCurrentViewedTablet->Render(*this->pRenderer);

	camera.ScaleCameraPoint(camera.GetZoomGlobalPositionModifier());

	pDrawer.refresh();
}

void GameLoop::RenderAllScreens()
{
	pDrawer.DrawScreens(GameManager::Screens.GetScreensList());
	GameManager::Screens.GetCurrentViewedTablet()->Render(*pDrawer.getRenderer());
}


void GameLoop::HandleMouseZoomEvent(SDL_Event& e)
{
	if (e.type == SDL_MOUSEWHEEL)
	{
		if (e.wheel.y > 0)
		{
			pDrawer.GetCamera().DecreaseZoomLevel(1);
		}
		else if (e.wheel.y < 0)
		{
			pDrawer.GetCamera().IncreaseZoomLevel(1);
		}
	}
}

void GameLoop::HandleHUDControls(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_TAB)
		{
			GameManager::SelectNextTarget(e);
		}
	}
}