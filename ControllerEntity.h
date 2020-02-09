#pragma once
#include"ModuleEntity.h"

class ControllerEntity : public ModuleEntity
{

public:

	ControllerEntity();
	ControllerEntity( std::shared_ptr<FiguredGraphics> ge, string name);

protected:

	void generateGridObjectDefaultToolTip() override;

private:

	void initialize();
	std::string GetToolTip();

};