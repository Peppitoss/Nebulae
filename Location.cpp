#include"Location.h"
#include"BackgroundCollection.h"
#include"Rect.h"

LocationProperties::LocationProperties()
{
	PositionOffset = Coord(0);

}


LocationConnector Location::GetConnector(const Coord& playerCurrentPosition)
{
	if (this->pProperties.Dimensions.x == 0 || this->pProperties.Dimensions.y == 0)
		return LocationConnector();

	SDL_Rect size = { -this->InnerDimension().x / 2, this->InnerDimension().y / 2, this->InnerDimension().x, this->InnerDimension().y };
	SDL_Rect translated = size;
	Coord difference = (this->pProperties.PositionOffset + this->OuterDimension() / 2).DotProduct(this->pProperties.SolarSystemPosition);
	translated.y += difference.y;
	translated.x += difference.x;

	//translated = Rect::IncreaseDimension(size, pPositionOffset.x, pPositionOffset.y, 0, 0);

	int startPosition = 129;
	int step = startPosition * 2;

	SDL_Rect verticalOpening = SDL_Rect{ 0,0,129,71 };
	SDL_Rect horizontalOpening = SDL_Rect{ 0,0,71,129 };

	//test left connector
	if (playerCurrentPosition.x - translated.x < 0)
	{
		int entry = this->LeftConnection.NextLocationEntrance;
		if (entry != 0)
		{
			SDL_Rect leftRegion = { translated.x - verticalOpening.w * 2,startPosition + (translated.y) - entry * step, verticalOpening.w, -verticalOpening.h };
			if (geometry::CoordWIthinRectangle(leftRegion, playerCurrentPosition))
				return LeftConnection;
		}
	}
	//test right connector
	if ((translated.x + translated.w) - playerCurrentPosition.x < 0)
	{
		int entry = this->RightConnection.NextLocationEntrance;
		if (entry != 0)
		{
			SDL_Rect rightRegion = { translated.x + translated.h + verticalOpening.w,startPosition + translated.y - entry * step, verticalOpening.w, -verticalOpening.h };
			if (geometry::CoordWIthinRectangle(rightRegion, playerCurrentPosition))
				return RightConnection;
		}
	}
	//test top connector
	if (playerCurrentPosition.y - translated.y > 0)
	{
		int entry = this->TopConnection.NextLocationEntrance;
		if (entry != 0)
		{
			SDL_Rect topRegion = { -startPosition + translated.x + entry * step, translated.y + horizontalOpening.h * 2, horizontalOpening.w, -horizontalOpening.h };
			if (geometry::CoordWIthinRectangle(topRegion, playerCurrentPosition))
				return TopConnection;
		}
	}
	//test bottom connector
	if ((translated.y - translated.h) > playerCurrentPosition.y)
	{
		int entry = this->BottomConnection.NextLocationEntrance;
		if (entry != 0)
		{
			SDL_Rect bottomRegion = { -startPosition + translated.x + entry * step, translated.y - translated.h - horizontalOpening.h, horizontalOpening.w, -horizontalOpening.h };
			if (geometry::CoordWIthinRectangle(bottomRegion, playerCurrentPosition))
				return BottomConnection;
		}
	}

	bool PlayerInCurrentLocation = geometry::CoordWIthinRectangle(Rect::GetSDLRect(translated.x, translated.y, translated.w, -translated.h), playerCurrentPosition);
	if (PlayerInCurrentLocation)
		return LocationConnector(0, this->pProperties.SolarSystemPosition);
	return LocationConnector();
}

LocationConnector::LocationConnector()
{
	this->NextLocationEntrance = 0;
	this->NextLocationCoord = Coord(0, 0);
	this->ConnectionOutOfBounds = true;
}

LocationConnector::LocationConnector(int entrance, Coord locationCoord)
{
	if (entrance <= 0)
		entrance = 1;
	this->NextLocationEntrance = entrance;
	this->NextLocationCoord = locationCoord;
	this->ConnectionOutOfBounds = false;
}

LocationData Location::pEmptyData = LocationData([](LocationData * location) 
	{
		location->SetIncludePlayerInLocation(true);
}, true);


std::vector<imagehandling::TextureObject_2> Location::pBorderTextures = std::vector<imagehandling::TextureObject_2>
{
	imagehandling::TextureObject_2(ImportImage_HUDCornerBorder),
	imagehandling::TextureObject_2(ImportImage_HUDHorizontalBorder),
	imagehandling::TextureObject_2(ImportImage_HUDVerticalBorder),

	imagehandling::TextureObject_2(ImportImage_HUDHorizontalBorderOpening),
	imagehandling::TextureObject_2(ImportImage_HUDVerticalBorderOpening)
};


Location::Location() : Location(EMPTYLOCATION())
{
}

Location::Location(LocationProperties properties)
{
	LocationData pData = LocationData();
	pProperties = properties;
}

Location::Location(string backgroundLocationName, LocationData data) : Location(backgroundLocationName, Coord(0, 0), Coord(0, 0), data)
{
	Type = enLocationType::enBackgroundLocation;
}

Location::Location(string name, Coord dimension, Coord solarsystemposition)
{
	Initialize();
	pProperties.Dimensions = dimension;
	pProperties.SolarSystemPosition = solarsystemposition;
	pProperties.Name = name;

	if (pProperties.Dimensions.x > 0 && pProperties.Dimensions.y > 0)
	{
		LeftConnection = LocationConnector();
		TopConnection = LocationConnector();
		BottomConnection = LocationConnector();
		RightConnection = LocationConnector();
	}
}

Location::Location(string name, Coord dimension, Coord solarsystemposition, LocationData data) : Location(name, dimension, solarsystemposition)
{
	this->SetData(data);
}

void Location::Initialize()
{
	LocationData pData = LocationData();
	pProperties = LocationProperties();
	State = enLocationState::enLocationNotVisited;
	Type = enLocationType::enNoRespawn;
	pLastVisitedTime = 0;
	respawnPeriodSecond = 10;

}

Location Location::CopyLocationWithoutData()
{
	Location result = Location(pProperties);
	result.TopConnection = TopConnection;
	result.BottomConnection = BottomConnection;
	result.LeftConnection = LeftConnection;
	result.RightConnection = RightConnection;
	result.State = State;
	result.Type = Type;
	result.pLastVisitedTime = pLastVisitedTime;
	return result;
}

bool Location::PlayerWithinLocation(const Coord& currentPlayerPosition)
{
	SDL_Rect size = { -InnerDimension().x / 2, InnerDimension().y / 2, InnerDimension().x, InnerDimension().y };
	SDL_Rect translated = size;
	translated.y += (GetPositionOffset().y + OuterDimension().y / 2)*this->GetSolarSystemPosition().y;
	translated.x += (GetPositionOffset().x + OuterDimension().x / 2)*this->GetSolarSystemPosition().x;
	return !geometry::CoordWIthinRectangle(translated, currentPlayerPosition);
}

Coord Location::GlobalPosition()
{
	Coord result = Coord(0);
	Coord outerDimension = OuterDimension();
	result.y -= (GetPositionOffset().y + outerDimension.y / 2)*this->GetSolarSystemPosition().y;
	result.x += (GetPositionOffset().x + outerDimension.x / 2)*this->GetSolarSystemPosition().x;
	return result;
}


LocationData& Location::GetData(Uint32 atTime)
{
	if (this->pLastVisitedTime == 0 || (this->Type == enLocationType::enRespawnAfterTime && SDL_TICKS_PASSED(atTime, this->pLastVisitedTime + this->respawnPeriodSecond * 1000)))
	{
		return pData;
	}
	return pEmptyData;
}

LocationData& Location::GetData()
{ 
	return pData;
}

void Location::MakeVisited()
{
	this->pLastVisitedTime = SDL_GetTicks();
	this->State = enLocationState::enLocationVisited;
}

void Location::RenderLocationBorder(SDL_Renderer* renderer, const Coord& CameraPosition)
{
	if (pProperties.Dimensions.x == 0 || pProperties.Dimensions.y == 0)
		return;

	int cornerSize = 170;

	Coord translation = GlobalPosition();

	Coord outerDimension = OuterDimension();

	Coord topLeft = outerDimension / -2						 + translation;
	Coord topRight = topLeft.xadd(outerDimension.x);
	Coord bottomRight = outerDimension / 2					 + translation;
	Coord bottomLeft = bottomRight.xadd(-outerDimension.x);

	bool BordersUseTextureFiltering = true;
	int s = BordersUseTextureFiltering;


	SDL_Rect topLeftCorner = { s,s,cornerSize,cornerSize };
	SDL_Rect topRightCorner = { cornerSize+s,s,cornerSize,cornerSize };
	SDL_Rect bottomLeftCorner = { cornerSize *2+s,s,cornerSize,cornerSize };
	SDL_Rect bottomRightCorner = { cornerSize *3+s,s,cornerSize,cornerSize };

	pBorderTextures[0].RenderTexture(renderer, topLeftCorner, topLeft - CameraPosition);
	pBorderTextures[0].RenderTexture(renderer, topRightCorner, topRight.xadd(-cornerSize) - CameraPosition);
	pBorderTextures[0].RenderTexture(renderer, bottomRightCorner, bottomRight.xadd(-cornerSize).yadd(-cornerSize) - CameraPosition);
	pBorderTextures[0].RenderTexture(renderer, bottomLeftCorner, bottomLeft.yadd(-cornerSize) - CameraPosition);

	SDL_Rect horizontalDimension = { s,s,LocationProperties::DimensionLenght,LocationProperties::DimensionWidth };
	SDL_Rect verticalDimension = { s,s,horizontalDimension.h,horizontalDimension.w };

	for (int i = 1; i < pProperties.Dimensions.x+1; i++)
	{
		if (TopConnection.NextLocationEntrance == i)
			pBorderTextures[3].RenderTexture(renderer, horizontalDimension, topLeft.xadd(cornerSize + (i - 1)* LocationProperties::DimensionLenght) - CameraPosition);
		else
			pBorderTextures[1].RenderTexture(renderer, horizontalDimension, topLeft.xadd(cornerSize + (i-1)* LocationProperties::DimensionLenght) - CameraPosition);

		if (BottomConnection.NextLocationEntrance == i)
			pBorderTextures[3].RenderTexture(renderer, Rect::Translate(horizontalDimension, 0, LocationProperties::DimensionWidth), bottomLeft.yadd(-pProperties.DimensionWidth).xadd(cornerSize + (i - 1) * LocationProperties::DimensionLenght) - CameraPosition);
		else
			pBorderTextures[1].RenderTexture(renderer, Rect::Translate(horizontalDimension, 0, LocationProperties::DimensionWidth), bottomLeft.yadd(-pProperties.DimensionWidth).xadd(cornerSize + (i - 1) * LocationProperties::DimensionLenght) - CameraPosition);
	}

	for (int i = 1; i < pProperties.Dimensions.y+1; i++)
	{
		if (LeftConnection.NextLocationEntrance == i)
			pBorderTextures[4].RenderTexture(renderer, verticalDimension, topLeft.yadd(cornerSize + (i - 1) * LocationProperties::DimensionLenght) - CameraPosition);
		else
			pBorderTextures[2].RenderTexture(renderer, verticalDimension, topLeft.yadd(cornerSize + (i - 1) * LocationProperties::DimensionLenght) - CameraPosition);

		if (RightConnection.NextLocationEntrance == i)
			pBorderTextures[4].RenderTexture(renderer, Rect::Translate(verticalDimension, LocationProperties::DimensionWidth, 0), topRight.xadd(-pProperties.DimensionWidth).yadd(cornerSize + (i - 1) * LocationProperties::DimensionLenght) - CameraPosition);
		else
			pBorderTextures[2].RenderTexture(renderer, Rect::Translate(verticalDimension, LocationProperties::DimensionWidth, 0), topRight.xadd(-pProperties.DimensionWidth).yadd(cornerSize + (i - 1) * LocationProperties::DimensionLenght) - CameraPosition);
	}
}


LevelFinishStats Location::GetLevelFinishStats()
{
	return LevelFinishStats();
}