#pragma once
#include"LauncherEntity.h"
#include"Projectile.h"


class MissileLauncherEntity : public LauncherEntity
{

public:
	
	MissileLauncherEntity();
	MissileLauncherEntity( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name);
	MissileLauncherEntity( std::shared_ptr<FiguredGraphics> ge, primitives::vector dir, std::shared_ptr<Projectile> proj, Coord lPoint, string name);

	std::shared_ptr<Projectile> launch() override; // is a new functions, delete using clearProjectiles

	void SetAimTarget(std::shared_ptr<GenericEntity> target ) override;

protected:

	//void generateGridObjectDefaultToolTip() override;

private:

	void Initialize();
	std::string GetToolTip() override;

};


