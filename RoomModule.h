#pragma once
#include"ModuleEntity.h"
#include"GridBoxTextureCollection.h"


class RoomModule : public ModuleEntity
{

public:

	RoomModule();
	RoomModule( std::shared_ptr<FiguredGraphics>, const GridObjectTexturePack& , const BoxList& );


private:

	void GenerateRoomLayout(const GridObjectTexturePack& , const BoxList&);


};