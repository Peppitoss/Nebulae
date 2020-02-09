#include"SectorUnitDescription.h"
#include"ShipModel.h"

SectorUnitDescription::SectorUnitDescription()
{
	Allegiance = enumAllegiance::aENEMY;
}

//int SectorUnitDescription::GetUnitAmount(int sectorLocationsAmount)
//{
//	int result = 0;
//
//	if (GetRandomValue() >= producedUnits)
//		return result;
//
//	//todo
//
//	//use sectorLocationsAmount with distributions
//
//	producedUnits += result;
//	return result;
//}