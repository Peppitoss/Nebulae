#include"SectorRecipe.h"


SectorRecipe::SectorRecipe(std::function<void(SectorRecipe*)> initializer)
{
	Initialize();
	initializer(this);

}

const SectorUnitDescription& SectorRecipe::GetUnitDescription(int index) const
{
	return UnitDescriptions[index];
}

void SectorRecipe::Initialize()
{
	FixedLocations = std::vector<Location>();
}

