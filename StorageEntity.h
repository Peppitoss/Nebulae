#pragma once
#include"RoomModule.h"

class StorageEntity : public RoomModule
{

public:

	StorageEntity(std::shared_ptr<FiguredGraphics>, const GridObjectTexturePack& , const BoxList&);

protected:

	void generateGridObjectDefaultToolTip() override;

private:

	std::string GetToolTip();

};