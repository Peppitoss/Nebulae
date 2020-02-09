#include"Sector.h"
#include"StringBuilder.h"
#include<algorithm>

Sector::Sector(string name)
{
	pSectorName = name;
}

Sector::Sector(string name, const SectorRecipe& recipe)
{
	pSectorName = name;
	pLocationDataMap = CoordMap<Location>(10);
	this->GenerateSector(recipe);
	
}


Sector::Sector( const Sector& other)
{
	pSectorName = other.pSectorName;
	pType = other.pType;
	pLocationDataMap = other.pLocationDataMap;
}
Sector::Sector( Sector&& other)
{
	pSectorName = std::move(other.pSectorName);
	pType = std::move(other.pType);
	pLocationDataMap = std::move(other.pLocationDataMap);
}

Sector& Sector::operator=(const Sector& other)
{
	pSectorName = other.pSectorName;
	pType = other.pType;
	pLocationDataMap = other.pLocationDataMap;
	return *this;
}

Sector& Sector::operator=(const Sector&& other)
{
	pSectorName = std::move(other.pSectorName);
	pType = std::move(other.pType);
	pLocationDataMap = std::move(other.pLocationDataMap);
	return *this;
}

std::string Sector::Name()
{
	return pSectorName;
}

std::string Sector::RandomLocationName()
{
	static int callNumber = 0;
	return "L" + IntToString(callNumber++);
}

void Sector::GenerateSector(const SectorRecipe& recipe )
{
	auto coordinateList = RandomWalkGenerator::GenerateWalk(recipe.LocationTopology);
	if (coordinateList.size() == 0)
		return;

	Coord currentCoordinate;
	for (int i = 0; i < coordinateList.size()-1; )
	{
		currentCoordinate = coordinateList[i];

		if(pLocationDataMap.CoordIsFilled(currentCoordinate) == false)
			pLocationDataMap.InsertBox(std::shared_ptr<Location>(new Location(RandomLocationName(), Coord(1, 1), currentCoordinate)), currentCoordinate);

		Coord coordinateDifference = currentCoordinate - coordinateList[++i];
		if (pLocationDataMap.CoordIsFilled(coordinateList[i]) == false)
			pLocationDataMap.InsertBox(std::shared_ptr<Location>(new Location(RandomLocationName(), Coord(1, 1), coordinateList[i])), coordinateList[i]);

		if (coordinateDifference.x == 1 && coordinateDifference.y == 0)
		{
			pLocationDataMap[currentCoordinate]->LeftConnection = LocationConnector(1, coordinateList[i]);
			pLocationDataMap[coordinateList[i]]->RightConnection = LocationConnector(1, currentCoordinate);
		}
		else if (coordinateDifference.x == -1 && coordinateDifference.y == 0)
		{
			pLocationDataMap[currentCoordinate]->RightConnection = LocationConnector(1, coordinateList[i]);
			pLocationDataMap[coordinateList[i]]->LeftConnection = LocationConnector(1, currentCoordinate);
		}
		else if (coordinateDifference.y == 1 && coordinateDifference.x == 0)
		{
			pLocationDataMap[currentCoordinate]->BottomConnection = LocationConnector(1, coordinateList[i]);
			pLocationDataMap[coordinateList[i]]->TopConnection = LocationConnector(1, currentCoordinate);
		}
		else if (coordinateDifference.y == -1 && coordinateDifference.x == 0)
		{
			pLocationDataMap[currentCoordinate]->TopConnection = LocationConnector(1, coordinateList[i]);
			pLocationDataMap[coordinateList[i]]->BottomConnection = LocationConnector(1, currentCoordinate);
		}
	}
	GenerateVesselsForLocations(recipe, pLocationDataMap);
}
void Sector::GenerateVesselsForLocations(const SectorRecipe& recipe, CoordMap<Location>& locations)
{
	for (int i = 0; i < recipe.DescriptionAmount(); i++)
	{
		auto description = recipe.GetUnitDescription(i);
		int locationsToAdd = 1 + locations.GetInsertedBoxes() * description.SectorDistribution / 100;
		auto shuffledLocations = locations.GetRandomFilledCoords(locationsToAdd);
		double numbersOfUnitToAdd = description.LocationAmount.GetRandomValue();

		double toAddPerLocation = numbersOfUnitToAdd / locationsToAdd;
		double unitToAddAccumulated = 0;

		for (int j = 0; j < shuffledLocations.size() && j < locationsToAdd; j++)
		{
			unitToAddAccumulated += toAddPerLocation;
			int toAdd = (int)unitToAddAccumulated;
			if (toAdd > 0)
			{
				unitToAddAccumulated = unitToAddAccumulated - toAdd;
				auto location = locations[shuffledLocations[j]];
				auto description = recipe.GetUnitDescription(i);
				location->GetData().AddVessel(description.UnitNameSet, toAdd, description.Allegiance, location->GetDimension());
			}
		}
	}
}


//LocationData Sector::GetLocationUsingRecipe(SectorRecipe& recipe)
//{
//	auto dataToAdd = LocationData();
//	int locations = recipe.
//	
//	return dataToAdd;
//}
