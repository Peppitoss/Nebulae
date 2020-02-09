#pragma once
#include"Integrator.h"

class VesselIntegrator : public integrator
{
public:

	VesselIntegrator();
	VesselIntegrator(bool fixedIndependentStepTime);

	void HandleEvents(SDL_Event & e, const Coord& cameraPos) override;
	void HandlePlayer(Uint32 ms) override;

	void RunUnitEntity(double ml) override;
	void RunPlayer(double ml) override;

	void RemoveUnit(std::shared_ptr<UnitEntity> object) override;
	void AddVessel(std::shared_ptr<Vessel> object, bool launchers, bool enemy);

	void RemoveVessel(std::shared_ptr<Vessel> object); //imporant as integrator rests anyway upon loading new map??
	void RemoveVesselAI(std::shared_ptr<VesselAI>);
	void RemoveTargetSelectionOfUnit(Widget* ExcludeVesselOfLink) const override;
	void RemoveTargetSelectionOfUnit(std::shared_ptr<UnitEntity> IncludeThisVessel) const override; 

	std::shared_ptr<UnitEntity> GetUnitEntityFromID(int ID) override;
	std::vector<std::shared_ptr<UnitEntity>> GetUnitEntities(bool enemy) override;
	enumAllegiance GetUnitAllegiance(std::shared_ptr<UnitEntity> unit) override;

	bool SetPlayer(std::shared_ptr<PlayerInterface> player) override;
	void SetPlayerVessel(std::weak_ptr<PlayerVessel> set);
	std::weak_ptr<PlayerVessel> GetPlayerVessel() const { return currentPlayerVessel; }
	std::weak_ptr<DynamicEntity> CurrentPlayer() override { return GetPlayerVessel(); }

	void CollDetect() override;

	std::vector<std::shared_ptr<UnitEntity>> enemyVesselObjects;
	std::vector<std::shared_ptr<UnitEntity>> alliedVesselObjects;


	void ClearVessels() override;

	bool IsSpaceMode() override { return true; }
	bool IsInteriorMode() override { return false; }



private:

	void Initialize();

	void LoadPlayerLaunchers() override;

	std::weak_ptr<PlayerVessel> currentPlayerVessel; //used as reference, no delete
	void RemoveVesselAndLaunchers(std::shared_ptr<Vessel>);

	void GenerateThreadForVessel(std::shared_ptr<Vessel> vessel);

};