#include"RailgunLauncher.h"
#include"Statistics.h"
#include"StringBuilder.h"

RailgunLauncher::RailgunLauncher() : LauncherEntity()
{
	Initialize();
}
RailgunLauncher::RailgunLauncher( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name) : LauncherEntity(ge, proj, name)
{
	Initialize();
}
RailgunLauncher::RailgunLauncher( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, Coord lPoint, string name)  : LauncherEntity(ge,primitives::vector(0,1), proj, lPoint, name)
{
	Initialize();
}

std::shared_ptr<Projectile> RailgunLauncher::launch()
{
	if (!CanLaunch())
		return std::shared_ptr<Projectile>();

	auto toLaunch = LauncherEntity::launch();

	if( statistics::Roll_100() <= this->GetMissChance() )
		toLaunch->setHitChance(0);
	return toLaunch;
}

std::string RailgunLauncher::GetToolTip()
{
	string name = this->GetIDName();
	string type = "Railgun cannon\n";
	string misschance = "Miss chance: " + IntToString(this->GetMissChance()) + "%";
	string recoil = "Recoil: " + IntToString(this->GetRecoil());
	string rateoffire = "Rate of fire: " + IntToString( this->GetRateOfFire());

	return name + "\n" + type + "\n" + rateoffire + "\n" + misschance +"\n" + recoil;
}

void RailgunLauncher::Initialize()
{
	pRecoilForce = 1000;
	pMissChance = 25;
	this->pAngularVelocity = 0.5;
	this->pAutoFire = true;	//should be true
	this->SetModuleType( enModuleType::mRailgunLauncher );
}
