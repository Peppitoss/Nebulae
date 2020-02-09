#pragma once
#include"LevelFinishStats.h"
#include"Mission.h"
//#include"LocationConnector.h"
#include"LocationData.h"

//class LocationConnector;

//struct LocationConnection
//{
//public:
//
//	LocationConnection();
//	Location ToLocation;
//	int Step;
//};

enum enLocationState
{
	enLocationNotVisited = 1,	//default start state
	//enLocationLocked = 2,	//default state after location entry if locked
	enLocationVisited = 3, //default state after location entry
	//enLocationFinished = 4,	//
};

enum enLocationType
{
	enRespawnAfterTime = 1, //regenerate location data after timer
	enNoRespawn = 2,
	enFriendly = 3,
	enBackgroundLocation = 4
};

class Location;

class LocationConnector
{

public:

	LocationConnector();
	LocationConnector(int entrance, Coord locationCoord);

	int NextLocationEntrance;
	Coord NextLocationCoord;
	bool ConnectionOutOfBounds;
	

};

class LocationProperties
{
public:
	LocationProperties();
	   
	Coord PositionOffset;
	Coord SolarSystemPosition;
	Coord Dimensions;
	string Name;

	static const int DimensionWidth = 129;
	static const int DimensionLenght = 328;

};

class Location
{

public:

	Location();
	Location(LocationProperties properties);
	Location(string backgroundLocationName, LocationData data);
	Location(string name, Coord dimension, Coord solarsystemposition);
	Location(string name, Coord dimension, Coord solarsystemposition, LocationData data);

	void SetName(string set) { this->pProperties.Name = set; }
	string GetName() const { return this->pProperties.Name; }

	void MakeVisited();

	Location CopyLocationWithoutData();

	LocationData& GetData();
	LocationData& GetData(Uint32 atTime);//Data retrieved from world manager, using its seed and location 
	LocationData GetDataCopy() const { return pData; }
	void SetData(const LocationData& set) { this->pData = set; } //Data retrieved from world manager, using its seed and location 

	Coord InnerDimension() { return Coord(LocationProperties::DimensionLenght * pProperties.Dimensions.x, LocationProperties::DimensionLenght * pProperties.Dimensions.y) + Coord(41 * 2); }
	Coord OuterDimension() { return InnerDimension() + Coord(170 * 2, 170 * 2) - Coord(41 * 2); }
	//void SetupLocationBorder();

	void RenderLocationBorder(SDL_Renderer*, const Coord& CameraPosition);

	void SetDimension(const Coord& set) { pProperties.Dimensions = set; } 
	Coord GetDimension() { return pProperties.Dimensions; }

	void SetSolarSystemPosition(const Coord& set) { pProperties.SolarSystemPosition = set; }
	Coord GetSolarSystemPosition()const { return pProperties.SolarSystemPosition; }

	void SetPositionOffset(const Coord& set) { pProperties.PositionOffset = set; }
	Coord GetPositionOffset()const { return pProperties.PositionOffset; }

	Coord GlobalPosition();

	LevelFinishStats GetLevelFinishStats();

	bool IsEmptyLocation() { return this->GetName() == EMPTYLOCATION().GetName(); }
	static Location EMPTYLOCATION() { return Location("EmptyLocation", Coord(0,0), Coord(-1,-1)); }

	LocationConnector GetConnector(const Coord& playerCurrentPosition);
	bool PlayerWithinLocation(const Coord& currentPlayerPosition);

	LocationProperties& Properties(){ return pProperties; }

	LocationConnector LeftConnection;
	LocationConnector TopConnection;
	LocationConnector BottomConnection;
	LocationConnector RightConnection;

	enLocationState State;
	enLocationType Type;

private:
	static std::vector<imagehandling::TextureObject_2> pBorderTextures;	//make these static in world manager, draw from world manager!! if not too many copies
	static LocationData pEmptyData;

	int respawnPeriodSecond;
	Uint32 pLastVisitedTime;
	LocationData pData;	//Data retrieved from world manager, using its seed and location , don't store data here 		//std::unique_ptr<Mission> pMission;
	LocationProperties pProperties;


	void Initialize();



};
