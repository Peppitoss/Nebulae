#pragma once
#include"LauncherEntity.h"


class TorpedoLauncherEntity : public LauncherEntity
{

public:
	
	TorpedoLauncherEntity();
	TorpedoLauncherEntity( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name);
	TorpedoLauncherEntity( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, Coord lPoint, string name);

	std::shared_ptr<Projectile> launch() override; // is a new functions, delete using clearProjectiles
	std::shared_ptr<Projectile> launch( double projectileSpeed );
	
	double GetTorpedoThrust() { return this->pTorpedoThrust; };
	void SetTorpedoThrust( double set ) { this->pTorpedoThrust = set; };


protected:

	//void generateGridObjectDefaultToolTip() override;

private:

	double pTorpedoThrust;
	void Initialize();
	std::string GetToolTip() override;

};

