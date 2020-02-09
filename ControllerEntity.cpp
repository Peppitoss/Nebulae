#include"ControllerEntity.h"



ControllerEntity::ControllerEntity() : ModuleEntity()
{
	initialize();
}

ControllerEntity::ControllerEntity( std::shared_ptr<FiguredGraphics> ge, string name) : ModuleEntity(ge, name)
{
	initialize();
}


void ControllerEntity::initialize()
{
	this->SetModuleType( enModuleType::mController );
}

void ControllerEntity::generateGridObjectDefaultToolTip()
{
	this->generateGridObjectToolTip( GetToolTip() );
}

std::string ControllerEntity::GetToolTip()
{
	return this->GetIDName();
}