#pragma once
#include<string>
#include<vector>
#include"RandomNumber.h"
#include"Graphics.h"

class SectorUnitDescription
{

public:
	SectorUnitDescription();

	//Modifier in percent, changes the result based on a determined difficulty
	//int GetUnitAmount(int sectorLocationsAmount);

	std::vector<std::string> UnitNameSet;

	//Mean number of units in sector
	statistics::RandomNumber LocationAmount;
	enumAllegiance Allegiance;

	//Units to produce per location. 100% means the sector amount is perfectly distributed for all locations
	//0% means that the sector amount will occur in one location
	double SectorDistribution;

private:

	//stores the units produced, to know how many are left
	//int producedUnits;


};