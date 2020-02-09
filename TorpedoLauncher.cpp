#include"TorpedoLauncher.h"
#include"DynamicEntity.h"
#include"StringBuilder.h"

TorpedoLauncherEntity::TorpedoLauncherEntity() : LauncherEntity()
{
	Initialize();
}
TorpedoLauncherEntity::TorpedoLauncherEntity( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name) 
	: LauncherEntity(ge, proj,name)
{
	Initialize();
}
TorpedoLauncherEntity::TorpedoLauncherEntity( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, Coord lPoint, string name) 
	: LauncherEntity(ge,primitives::vector(0,1), proj, lPoint, name)
{
	Initialize();
}

std::shared_ptr<Projectile> TorpedoLauncherEntity::launch()
{
	if( !CanLaunch() )
		return std::shared_ptr<Projectile>();

	if( DynamicEntity* mothership = dynamic_cast< DynamicEntity* >( this->pMothership ) )
	{
		primitives::vector velocity = mothership->getVelocity();
		double angleBetweenVectors = geometry::angleBetweenVectors(velocity,this->pDirection);
		if( angleBetweenVectors  < std::_Pi/2 )
			return this->launch(velocity.ABS());
		else
			return this->launch(-0.01);
	}
	else
		return this->launch(1);
}

std::shared_ptr<Projectile> TorpedoLauncherEntity::launch( double projectileSpeed )
{
	SDL_AtomicIncRef(&this->pLaunchedProjectiles);
	std::shared_ptr<Projectile> pLaunch = pProjectileTemplate->copyProjectile();
	pLaunch->setUnitHeading( this->getUnitHeading() );

	primitives::vector projectileVelocity = pDirection*projectileSpeed;
	pLaunch->setVelocity( geometry::rotateVector( projectileVelocity, this->getUnitHeading() ) );

	pLaunch->setCurrentThrust( geometry::rotateVector( pDirection*this->GetTorpedoThrust(), this->getUnitHeading() ) );
	pLaunch->setLaunchPoint( this->getLaunchPoint() );
	return pLaunch;
}

std::string TorpedoLauncherEntity::GetToolTip()
{
	string name = this->GetIDName();
	string type = "Torpedo Launcher\n";
	string thrust = "Torpedo thrust: " + IntToString(this->GetTorpedoThrust());
	string rateoffire = "Rate of fire: " + IntToString( this->GetRateOfFire());

	return name + "\n" + type + "\n" + rateoffire + "\n" + thrust;
}

void TorpedoLauncherEntity::Initialize()
{
	this->SetModuleType( enModuleType::mTorpedoLauncher );
}

