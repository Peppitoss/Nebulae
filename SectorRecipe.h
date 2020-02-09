#pragma once
#include"Location.h"
#include"RandomWalk.h"
#include"SectorUnitDescription.h"
#include<vector>

class SectorRecipe
{
public:
	
	SectorRecipe(std::function<void(SectorRecipe*)>);

	std::vector<Location> FixedLocations;
	RandomWalk LocationTopology;
	const SectorUnitDescription& GetUnitDescription(int index) const;
	void AddUnitDescription(SectorUnitDescription toAdd) { this->UnitDescriptions.push_back(toAdd); }


	int DescriptionAmount() const {return UnitDescriptions.size();}

private:

	std::vector<SectorUnitDescription> UnitDescriptions;
	void Initialize();

};