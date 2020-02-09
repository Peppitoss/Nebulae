#pragma once
#include"MissileLauncher.h"


class AntiProjectileMissileLauncher : public MissileLauncherEntity
{

public:

	AntiProjectileMissileLauncher();
	AntiProjectileMissileLauncher( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name);
	AntiProjectileMissileLauncher( std::shared_ptr<FiguredGraphics> ge, primitives::vector dir, std::shared_ptr<Projectile> proj, Coord lPoint, string name);

	std::shared_ptr<Projectile> launch() override;

protected:

	bool CanTargetVessel() override { return false; }

private:

	double pTargetScanRange;
	void Initialize();

};