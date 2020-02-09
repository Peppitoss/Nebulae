#include"MissileLauncher.h"
#include"RandomNumberGenerator.h"
#include"MissileProjectile.h"
#include"StringBuilder.h"

MissileLauncherEntity::MissileLauncherEntity() : LauncherEntity()
{
	Initialize();
}

MissileLauncherEntity::MissileLauncherEntity( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name) 
	: LauncherEntity(ge, proj, name)
{
	Initialize();
}

MissileLauncherEntity::MissileLauncherEntity( std::shared_ptr<FiguredGraphics> ge, primitives::vector dir, std::shared_ptr<Projectile> proj, Coord lPoint, string name)
	: LauncherEntity(ge,dir,proj,lPoint, name)
{
	Initialize();
}
std::shared_ptr<Projectile> MissileLauncherEntity::launch()
{
	if( !CanLaunch() )
		return std::shared_ptr<Projectile>();

	SDL_AtomicIncRef(&this->pLaunchedProjectiles);
	std::shared_ptr<Projectile> launch = pProjectileTemplate->copyProjectile(); // thread here?
	launch->setUnitHeading( this->getUnitHeading() );

	primitives::vector projectileVelocity = geometry::rotateVector( pDirection, this->getUnitHeading() );//launch->getVelocity();
	//primitives::NormalDistribution(launch->getVelocity().x, launch->getVelocity().y, launch->getSpread(), launch->getSpread(), projectileVelocity, RANDGENERATOR() );

	launch->setCurrentThrust( projectileVelocity );
	launch->setVelocity( projectileVelocity );
	launch->setLaunchPoint( this->getLaunchPoint() );
    //launchedProjects.push_back( launch );
	return launch;


	//FireBlaster(true);
	
}

void MissileLauncherEntity::SetAimTarget(std::shared_ptr<GenericEntity> target )
{
	LauncherEntity::SetAimTarget( target );
	if(std::shared_ptr<DynamicEntity> derived = std::dynamic_pointer_cast<DynamicEntity>(target))
	{
		((MissileProjectile*)this->pProjectileTemplate.get())->setTarget( derived );
	}
	
}

std::string MissileLauncherEntity::GetToolTip()
{
	string name = this->GetIDName();
	string type = "Missile Launcher\n";
	string rateoffire = "Rate of fire: " + IntToString( this->GetRateOfFire());

	return name + "\n" + type + "\n" + rateoffire;
}

void MissileLauncherEntity::Initialize()
{
	this->SetModuleType( enModuleType::mMissileLauncher );
}

