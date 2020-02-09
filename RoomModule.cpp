#include"RoomModule.h"



RoomModule::RoomModule( std::shared_ptr<FiguredGraphics> ge, const GridObjectTexturePack& pack, const BoxList& list) : ModuleEntity(ge, "noname")
{
	GenerateRoomLayout( pack, list );
};

void RoomModule::GenerateRoomLayout(const GridObjectTexturePack& pack, const BoxList& list)
{
	//if( pack.texture.expired() ) 
	//	return; // should never get here, since shared pointer from texturepack is global??
	//this->generateGridObjectForModule( list, pack, 64 ); //64 is ship size
}