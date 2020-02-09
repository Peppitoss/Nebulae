#pragma once 
#include "Location.h"
#include"SectorRecipe.h"
#include <list>

using std::string;
using std::list;

enum enSectorType
{
	enCustomSector,

};

template class CoordMap<Location>;
template class CoordMapIterator<Location>;

class Sector
{

public:

	Sector(string name);
	Sector(string name, const SectorRecipe& recipe);
	Sector( const Sector& );
	Sector( Sector&& );

	std::string RandomLocationName();

	Sector& operator=(const Sector& other);
	Sector& operator=(const Sector&& other);

	Location GetLocation( const Coord& );
	bool isLocationEmpty( const Coord& );

	std::string Name();

	CoordMapIterator<Location> const MapBegin() { return pLocationDataMap.Begin(); }
	CoordMapIterator<Location> const MapEnd() { return pLocationDataMap.End(); }

	void GenerateSector(const SectorRecipe& recipe);
	void GenerateVesselsForLocations(const SectorRecipe& recipe, CoordMap<Location>& locations);
	//LocationData GetLocationUsingRecipe(SectorRecipe& recipe);

private:

	string pSectorName;
	enSectorType pType;
	CoordMap<Location> pLocationDataMap;

};