#include"PowerSupplyEntity.h"



PowerSupplyEntity::PowerSupplyEntity() : ModuleEntity()
{
	initialize();
}

PowerSupplyEntity::PowerSupplyEntity( std::shared_ptr<FiguredGraphics> ge, string name) : ModuleEntity(ge, name)
{
	initialize();
}

void PowerSupplyEntity::generateGridObjectDefaultToolTip()
{
	this->generateGridObjectToolTip( GetToolTip() );
}

std::string PowerSupplyEntity::GetToolTip()
{
	return this->GetIDName();
}

void PowerSupplyEntity::initialize()
{
	this->SetModuleType( enModuleType::mPowerSupply );
}