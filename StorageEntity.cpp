#include"StorageEntity.h"


StorageEntity::StorageEntity(std::shared_ptr<FiguredGraphics> ge, const GridObjectTexturePack& pack, const BoxList& list) : RoomModule( ge, pack, list )
{
	this->SetModuleType( enModuleType::mRoom );
}

void StorageEntity::generateGridObjectDefaultToolTip()
{
	this->generateGridObjectToolTip( GetToolTip() );
}

std::string StorageEntity::GetToolTip()
{
	return this->GetIDName();
}