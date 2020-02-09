#pragma once
#include"InstanceManager.h"
#include"DrawClass.h"
//#include"Timer.h"

class GameLoop
{

public:

	GameLoop(SpriteDrawer& drawer);


	bool Run();
	bool Run_2();
	bool CurrentLevelActive();

	void DisableEventRun() { pDisableEventRun = true; }
	void EnableEventRun() { pDisableEventRun = false; }
	void DefineInstanceManager(std::shared_ptr<InstanceManager> instanceE) { this->pInstanceManager = instanceE; }
	//void DefineDrawer(SpriteDrawer&);

	Millisecond GetCurrentTimestep() const { return this->pCurrentTimestep; }

private:

	void Initialize();
	bool RunEvents(std::shared_ptr<integrator> integrator);
	void RunIntegrator(integrator*, int stepTime);
	void RunDrawer(Background*, InstanceManager*);

	void HandleMouseZoomEvent(SDL_Event& e);
	void HandleHUDControls(SDL_Event& e);

	void RenderAllScreens();

	timer FPSHandler;
	timer pLoopTimer;
	double PreferredFPS;
	double secPrFrame;// = 1/PreferredFPS;
	double msPrFrame;// = secPrFrame* 1000;
	bool pDisableEventRun;
	Millisecond pCurrentTimestep;
	Millisecond pAccumulator;

	SDL_Event sdlevent;
	std::shared_ptr<InstanceManager> pInstanceManager;
	SpriteDrawer& pDrawer;
	SDL_Renderer* pRenderer;

};