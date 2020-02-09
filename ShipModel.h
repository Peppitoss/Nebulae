#pragma once
#include"Graphics.h"
#include"ShipModelProbabilitySet.h"
#include"Kinematics.h"
#include"Behaviours.h"
#include<string>
#include<list>

using std::string;

class ShipModel
{

public:

	ShipModel(string shipName, int ID, const Coord& initPos);
	ShipModel(string shipName, int ID, const Coord& initPos, double heading);
	ShipModel( const ShipModel& );
	ShipModel( ShipModel&& );

	bool operator<(const ShipModel& other) const;

	ShipModel& operator=(const ShipModel& other);
	ShipModel& operator=(const ShipModel&& other);

	void SetCurrentPosition( const Coord set );

	Coord GetCurrentPosition() const;
	primitives::waypoint GetTargetPosition() const;

	string GetName() const { return this->pShipName; };

	bool IsInitiallyActive() const { return this->pIsInitiallyActive; }
	void SetInitiallyActive(bool set) { this->pIsInitiallyActive = set; }

	int GetID() const { return this->pID; };

	BehaviorTree GetBehaviour() const { return this->pBehaviour; };
	void SetBehaviour(BehaviorTree set) { this->pBehaviour = set; }

	enumAllegiance GetAllegiance() const { return this->pAllegiance; }
	void SetAllegiance(enumAllegiance set) { this->pAllegiance = set; }

	bool IsEnemy() const { return this->GetAllegiance() == enumAllegiance::aENEMY; }

	std::vector<string>& GetLinks() { return this->pShipLinks; };
	void AddLink(string linkdescription) { pShipLinks.push_back(linkdescription); };

	Kinematics& GetKinematics() { return this->pKinematics; }

	std::map<std::string, Kinematics>& ExtenalEntityKinematics() { return this->pExternalEntityKinematics; }

private:

	void Initialize();

	Kinematics pKinematics;
	Coord pCurrentPosition;
	std::map<std::string, Kinematics> pExternalEntityKinematics;

	string pShipName;
	int pID;

	bool pIsInitiallyActive;

	enumAllegiance pAllegiance;
	BehaviorTree pBehaviour;
	std::vector<string> pShipLinks;

};