#pragma once
#include"ModuleEntity.h"

class PowerSupplyEntity : public ModuleEntity
{

public:

	PowerSupplyEntity();
	PowerSupplyEntity( std::shared_ptr<FiguredGraphics> ge, string name);


protected:

	void generateGridObjectDefaultToolTip() override;

private:

	void initialize();
	std::string GetToolTip();
};