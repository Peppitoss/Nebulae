#pragma once
#include"ModuleEntity.h"

class EngineEntity : public ModuleEntity
{

public:

	EngineEntity();
	EngineEntity( std::shared_ptr<FiguredGraphics> ge, string name);

	primitives::vector GetDisturbanceForce();
	void SetDisturbanceForce(const primitives::vector& );
	void ResetDisturbanceForce();

	void AddDamageRecieved(const game::Damage& set) override;

	void SetThrust( int set );
	int GetThrust();

	double GetAngularThrust() const { return this->pAngularThrust; }
	void SetAngularThrust(double set) { this->pAngularThrust = set; }

	int GetThrustLoss();
	int GetAngularThrustLoss() { return 0; }

protected:

	void generateGridObjectDefaultToolTip() override;

private:

	primitives::vector pDisturbanceForce;

	int pThrust;
	double pAngularThrust;
	void initialize();
	std::string GetToolTip();

};