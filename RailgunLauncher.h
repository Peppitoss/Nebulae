#pragma once
#include"LauncherEntity.h"


class RailgunLauncher : public LauncherEntity
{

public:

	RailgunLauncher();
	RailgunLauncher( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name );
	RailgunLauncher( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, Coord lPoint, string name);

	std::shared_ptr<Projectile> launch() override;

	int GetMissChance() { return this->pMissChance; };
	void SetMissChance( int set ) { this->pMissChance = set; };

	int GetRecoil() { return this->pRecoilForce; };
	void SetRecoil( int set ) { this->pRecoilForce = set; };

protected:

	//void generateGridObjectDefaultToolTip() override;

private:

	int pRecoilForce;
	int pMissChance;
	void Initialize();
	std::string GetToolTip() override;

};