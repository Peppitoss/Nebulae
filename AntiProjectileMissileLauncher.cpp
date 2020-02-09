#include"AntiProjectileMissileLauncher.h"


AntiProjectileMissileLauncher::AntiProjectileMissileLauncher() : MissileLauncherEntity()
{
	Initialize();
}
AntiProjectileMissileLauncher::AntiProjectileMissileLauncher( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name) 
	: MissileLauncherEntity(ge, proj, name)
{
	proj->SetProjectileHitAnimation(enHitAnimation::SHRAPNEL);
	Initialize();
}
AntiProjectileMissileLauncher::AntiProjectileMissileLauncher( std::shared_ptr<FiguredGraphics> ge, primitives::vector dir, std::shared_ptr<Projectile> proj, Coord lPoint, string name)
	: MissileLauncherEntity(ge,dir, proj, lPoint, name)
{
	proj->SetProjectileHitAnimation(enHitAnimation::SHRAPNEL);
	Initialize();
}

std::shared_ptr<Projectile> AntiProjectileMissileLauncher::launch()
{
	auto launch = MissileLauncherEntity::launch();
	launch->SetCanTargetVessel(false);
	return launch;
}

void AntiProjectileMissileLauncher::Initialize()
{
	this->pTargetScanRange = 1000;
	this->pAutoFire = true; //should be false
}