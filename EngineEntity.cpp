#include"EngineEntity.h"
#include"Statistics.h"
#include"StringBuilder.h"

EngineEntity::EngineEntity() : ModuleEntity()
{
	initialize();
}

EngineEntity::EngineEntity( std::shared_ptr<FiguredGraphics> ge, string name) : ModuleEntity(ge, name)
{
	initialize();
}


void EngineEntity::initialize()
{
	this->pThrust = 10;
	this->pAngularThrust = 10;
	this->pDisturbanceForce = primitives::vector(0);
	this->SetModuleType( enModuleType::mEngine );
}

void EngineEntity::generateGridObjectDefaultToolTip()
{
	this->generateGridObjectToolTip( GetToolTip() );
}

std::string EngineEntity::GetToolTip()
{
	string name = this->GetIDName();
	string type = "Engine\n";
	string thrust = "Thrust: " + IntToString(this->pThrust);

	return name + "\n" + type + "\n" + thrust;
}

void EngineEntity::SetThrust( int set )
{
	this->pThrust = set;
}
int EngineEntity::GetThrust()
{
	return this->pThrust*this->GetEfficiency();
}

int EngineEntity::GetThrustLoss()
{
	return this->pThrust*(1-this->GetEfficiency());
}

void EngineEntity::AddDamageRecieved(const game::Damage& set)
{
	ModuleEntity::AddDamageRecieved(set);
	double disturbanceFactor = 1;
	SetDisturbanceForce(statistics::GetRandomDirection()*disturbanceFactor*this->GetThrust());
}

void EngineEntity::SetDisturbanceForce(const primitives::vector& set)
{
	pDisturbanceForce = set;
}

primitives::vector EngineEntity::GetDisturbanceForce()
{
	return pDisturbanceForce;
}

void EngineEntity::ResetDisturbanceForce()
{
	pDisturbanceForce = primitives::vector(0);
}