#pragma once
#include<SDL.h>
#include"ModuleEntity.h"

enum enDoorState
{
	dStateClosed = 1,
	dStateOpening = 2,
	dStateClosing = 3,
	dStateOpen = 4
};

enum enDoorType
{
	dtSlideUp = 1,
	dtSlideDown = 2,
	dtSlideRight = 3,
	dtSlideLeft = 4
};

class DoorModule : public ModuleEntity
{

public:

	DoorModule();
	DoorModule::DoorModule(std::shared_ptr<GraphicalEntity> graphicalentity, string name);

	bool IsActive() const { return this->pDoorActive; }
	void SetActive(bool set) { this->pDoorActive = set; }

	SDL_Rect GetTriggerRegion() const { return pTriggerRegion; };
	void SetTriggerRegion(const SDL_Rect set) { pTriggerRegion = set; };

	bool PointIsInTriggerRegion(const Coord& pos);
	void AutoGenerateTriggerRegion();

	bool IsMoving() const { return pState == enDoorState::dStateClosing || pState == enDoorState::dStateOpening;  }
	void move(Uint32 milliseconds) override;
	void HandleDoor(int ms);

	void OpenDoor();
	void CloseDoor();

	enDoorState GetDoorState() const { return this->pState; };

	enDoorType GetDoorType() const { return this->pDoorType; };
	void SetDoorType(enDoorType type) { this->pDoorType = type; };

private:

	void SetState(enDoorState state);
	void ResetTimer() { this->pTimeCounter = 0; }
	void SetTimer(int amount) { this->pTimeCounter = amount; }
	int GetTimer() const { return this->pTimeCounter; }

	enDoorType pDoorType;
	enDoorState pState;

	primitives::vector GetVelocityFromType(enDoorType type);

	void StopDoorMovement();
	primitives::vector pAccumulatedMovement;
	int pDoorDirection;
	int pDoorVelocity;
	bool pDoorActive;
	SDL_Rect pTriggerRegion;

	int pWaitUntilClose;
	int pTimeCounter;
	void Initialize();


};