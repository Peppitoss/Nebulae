#include"LauncherEntity.h"
#include"RandomNumberGenerator.h"
#include"Vessel.h"
#include"StringBuilder.h"
#include"SDLKeyboardSyntax.h"

LauncherEntity::LauncherEntity() : ModuleEntity()
{
	initialize();
	pRefDirection = primitives::vector(0,0);
	pDirection = pRefDirection;
	pRelativeProjectileLaunchPoint = Coord(0,0);
	//pRefProjectileLaunchPoint = pProjectileLaunchPoint;
}
LauncherEntity::LauncherEntity( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name) : ModuleEntity(ge, name)
{
	initialize();
	pProjectileTemplate = proj;
	pRefDirection = primitives::vector(0,0);
	pDirection = pRefDirection;
	proj->setVelocity( pDirection );
	pRelativeProjectileLaunchPoint = Coord(0,0);
	//pRefProjectileLaunchPoint = pProjectileLaunchPoint;
}

LauncherEntity::LauncherEntity(std::shared_ptr<FiguredGraphics> ge, primitives::vector dir, std::shared_ptr<Projectile> proj, Coord Lpoint, string name) : ModuleEntity(ge, name)
{
	initialize();
	pProjectileTemplate = proj;
	pRefDirection = dir;
	pDirection = pRefDirection;
	pProjectileLaunchPoint = Coord(0,0); 
	//pRefProjectileLaunchPoint = pProjectileLaunchPoint;
	pRelativeProjectileLaunchPoint = Lpoint;
}

LauncherEntity::LauncherEntity( const LauncherEntity & other )
{
	initializeSoundEffects();
	//pRelativeRotation = other.pRelativeRotation;
	//pProjectileTemplate = other.pProjectileTemplate;
	//pDirection = other.pDirection;
	//pProjectileLaunchPoint = other.pProjectileLaunchPoint;

}
void LauncherEntity::initialize()
{
	pLaunch = false;
	pAllowHitMotherShip = false;
	lastFireTime = 0;
	initializeSoundEffects();
	pAttributes = game::LauncherAttributes();
	pAttributes.SetRateOfFire( 0 );
	this->SetRelativeHeading(0);
	//this->pAimTarget.reset();
	this->pAimTarget.reset();
	this->pMothership = nullptr;
	this->pAngularVelocity = 0;
	this->pAutoFire = false;
	this->pTargetScanRange = -1;
	this->pPlayeAlly = false;
	this->SetModuleType( enModuleType::mLauncher );
	this->pHotKey = SDLKeyBible::GetNumberAsScancode(0);
	this->pLaunchedProjectiles = SDL_atomic_t{ 0 };
	//
}

void LauncherEntity::generateGridObjectForItem(const BoxList& gridPos)
{
	ModuleEntity::generateGridObjectForItem(gridPos);
	this->getGridObject()->GenerateHotKeyButton(SDLKeyBible::GetKeyFromScancode(this->pHotKey).c_str()[0]); //
}

void LauncherEntity::generateGridObjectForModule(CoordMap<GriddedBox>& list, imagehandling::IMGToSurface* model, int shipSize)
{
	ModuleEntity::generateGridObjectForModule(list, model, shipSize);
	this->getGridObject()->GenerateHotKeyButton(SDLKeyBible::GetKeyFromScancode(this->pHotKey).c_str()[0]);
}

void LauncherEntity::initializeSoundEffects()
{
	// Should not be hard-coded, implement a configuration system where paths etc are stored.

	blasterFire_path = "../SoundEffects/laser_blast_fire.wav";
	blasterCoolDown_path = "../SoundEffects/laser_blast_cool_down.wav";
	
	//temporary commented away, since deletion of mix_chunk causes error
	//blasterFire = std::unique_ptr<Mix_Chunk>(Mix_LoadWAV( blasterFire_path ));
	//blasterCoolDown = std::unique_ptr<Mix_Chunk>(Mix_LoadWAV( blasterCoolDown_path ));
	//if ( blasterFire == NULL || blasterCoolDown == NULL )
	//{
	//	throw std::runtime_error("Error loading sound-effects..");
	//}
}

int LauncherEntity::GetRateOfFire() 
{ 
	return rateOfFire* (2-this->GetEfficiency()); 
};

void LauncherEntity::setLaunch( bool set )
{
	pLaunch = set;
}

Coord LauncherEntity::getLaunchPoint() { return pProjectileLaunchPoint; };
primitives::vector LauncherEntity::getDirection() { return pDirection; };
primitives::vector LauncherEntity::getRefDirection() { return pRefDirection; };

void LauncherEntity::setLastFire( Uint32 set )
{ 
	lastFireTime = set; 
};

void LauncherEntity::setRateOfFire( int rof )
{ 
	rateOfFire = rof; 
};

void LauncherEntity::setProjectile( std::shared_ptr<Projectile> set )
{
	pProjectileTemplate = set;
}

void LauncherEntity::setRelativeLaunchPoint( Coord set  )
{
	pRelativeProjectileLaunchPoint = set;
}

void LauncherEntity::setDirection( primitives::vector dir ) 
{ 
	pDirection = dir; 
}

void LauncherEntity::setDirection( double newDirection ) 
{ 
	if( this->GetRelativeHeading() != 0 )
		setDirection( geometry::rotateVector( this->getRefDirection(), this->getGlobalHeading() ) );
	else
		setDirection( geometry::rotateVector( this->getRefDirection(), newDirection ) );
}

void LauncherEntity::setLaunchPoint( Coord pos ) 
{
	this->pProjectileLaunchPoint = pos;
}



Coord LauncherEntity::getRelativeProjectileLaunchPoint()
{ 
	if( this->GetRelativeHeading() != 0 )
		return geometry::rotateCoord( this->pRelativeProjectileLaunchPoint, this->GetRelativeHeading() );
	else
		return this->pRelativeProjectileLaunchPoint;
}

void LauncherEntity::SetAllowToHitMothersip( bool set )
{
	pAllowHitMotherShip = set;
}


//void LauncherEntity::reduceTimeOnProjectiles( double millisec )
//{
//	bool active = true;
//	for( auto it = pbegin(); it != pend();  )
//	{
//		active = (*it)->getGraphics()->reduceTimer( millisec );
//
//		if( active == false || (*it)->isDestroyed() )
//		{
//			//Projectile* tempPointer = *it;
//			(*it).reset();
//			it = this->launchedProjects.erase(it); //potential for bug here
//			//delete tempPointer;
//		}
//		else
//			++it;
// 	}
//}

bool LauncherEntity::CanLaunch()
{
	return  this->GetEfficiency() >= 0.1 && this->IsEnabled();
}

std::shared_ptr<Projectile> LauncherEntity::launch()
{
	if( !CanLaunch() )
		return std::shared_ptr<Projectile>();

	SDL_AtomicIncRef(&this->pLaunchedProjectiles);
	std::shared_ptr<Projectile> pLaunch = pProjectileTemplate->copyProjectile();
	double projectileInitVelocity = std::abs(pLaunch->getVelocity().x); // x and y should be same, trying to avoid using abs which has square root :)
	//launch->setVelocity(pDirection);
	primitives::vector projectileVelocity;
	primitives::NormalDistribution(pRefDirection.x, pRefDirection.y, pLaunch->getSpread(), pLaunch->getSpread(), projectileVelocity, RANDGENERATOR() );

	pLaunch->setUnitHeading( this->getUnitHeading() ); //( geometry::vectorHeading( projectileVelocity ) );
	pLaunch->setVelocity( geometry::rotateVector( projectileVelocity*projectileInitVelocity, this->getUnitHeading() ));
	pLaunch->setLaunchPoint( this->getLaunchPoint() );
	return pLaunch;
 	//launchedProjects.push_back( launch );
	//FireBlaster(true);
	
}
void LauncherEntity::FireBlaster(bool set)
{
	//temporary commented away, since deletion of mix_chunk causes error
	//if (set == true)
	//{
	//	 Start the blasterFire effect in an infinite loop
	//	if (Mix_PlayChannel(-1, blasterFire.get(), -1) < 0)
	//	{
	//		throw std::runtime_error("Error playing sound-effect!");
	//	}
	//}
	//else
	//{
	//	 Stop the blasterFire..
	//	if (Mix_HaltChannel(-1) < 0)
	//	{
	//		throw std::runtime_error("Error stopping sound-effect!");
	//	}
	//	if (Mix_PlayChannel(-1, blasterCoolDown.get(), 0) < 0)
	//	{
	//		throw std::runtime_error("Error playing sound-effect!");
	//	}
	//}
}

bool LauncherEntity::HasTarget() const
{
	if( this->pAimTarget.expired() )
		return false;
	
	if( this->pAimTarget.lock()->isDestroyed() )
		return false;

	return true;
}

void LauncherEntity::UpdateAim( int milliseconds )
{
	if( !HasTarget() )
	{
		this->setLaunch(false);
		return;
	}
	//based on simple step regulator mechanics, No P, I or D :)
	
	double newHeading = geometry::angleBetweenTwoPoints( this->getLaunchPoint(), this->pAimTarget.lock()->getPosition() );
	double boundary = 0.1;

	double test = this->getGlobalHeading();
	double shortestanglediff = geometry::shortestAngleDifference(newHeading, this->getGlobalHeading() );

      if( ( abs(shortestanglediff) < boundary ) && TargetWithinRange() )//geometry::pointWithinBoundary_exclusive( this->getHeading(), newHeading-boundary, newHeading+boundary ) )
	{
		this->setLaunch(true);
	}
	else
	{
		//if( this->IsAutoFire())
		this->setLaunch(false);
		
	}
	this->SetRelativeHeading( GetRelativeHeading() + geometry::Sign(shortestanglediff)*this->pAngularVelocity*(double)milliseconds/1000);

}

bool LauncherEntity::TargetWithinRange()
{
	if (!HasTarget())
		return false;
	return TargetWithinRange(this->pAimTarget.lock());
}

bool LauncherEntity::TargetWithinRange(std::shared_ptr<GenericEntity> entity)
{
	if (this->pTargetScanRange < 0)
		return true;

	if (geometry::PointWithinRange(this->getPosition(), entity->getPosition(), this->GetTargetScanRange()))
		return true;

	return false;
}

void LauncherEntity::SetAimTarget( std::shared_ptr<GenericEntity> target )
{
	if(!CanTargetVessel())
		if( std::shared_ptr<Vessel> derived = std::dynamic_pointer_cast<Vessel>(target) )
			return;

	if( this->GetAngularVelocity() == 0 )
		return;

	this->pAimTarget = target;
}

bool LauncherEntity::CanTargetVessel()
{
	return true;
}

std::weak_ptr<GenericEntity> LauncherEntity::GetAimTarget() const
{
	return this->pAimTarget;
}

void LauncherEntity::ChangeMothership( GenericEntity* set )
{
	this->pMothership = set;
}
GenericEntity* LauncherEntity::GetMothership()
{
	return this->pMothership;
}

bool LauncherEntity::IsAutoFire()
{
	return this->pAutoFire;
}
void LauncherEntity::SetAutoFire( bool set )
{
	this->pAutoFire = set;
}

void LauncherEntity::ChangeAngularVelocity( double set )
{
	this->pAngularVelocity = set;
}
double LauncherEntity::GetAngularVelocity()
{
	return this->pAngularVelocity;
}

void LauncherEntity::SetTargetScanRange( double set )
{
	pTargetScanRange = set;
}
double LauncherEntity::GetTargetScanRange()
{
	return pTargetScanRange;
}

void LauncherEntity::clearProjectiles() 
{ 
	//for (auto it = this->launchedProjects.begin(); it != this->launchedProjects.end(); ++it)
	//	(*it).reset();

	//this->launchedProjects.clear(); 
	this->pProjectileTemplate.reset();
}

void LauncherEntity::SetDetached(bool set, int timed, const Coord& atPosition)
{
	ModuleEntity::SetDetached(set, timed, atPosition);
	this->setLaunch(false);
}

void LauncherEntity::generateGridObjectDefaultToolTip()
{
	this->generateGridObjectToolTip( GetToolTip() + "\n" + GetHotkeyTooltip());
}

void LauncherEntity::UpdateHotKeysFromGrid()
{
	if(this->hasGridObject())
		SetHotKey(SDLKeyBible::GetLetterAsScancode( this->getGridObject()->GetSelectedHotKey().c_str() ));
}

void LauncherEntity::SetHotKey(SDL_Scancode set)
{
	if (SDLKeyBible::IsNumber(set) || SDLKeyBible::IsEmptyString(set))
	{
		this->pHotKey = set;
		if (this->hasGridObject())
		{
			this->getGridObject()->SetHotKeyForDisplay(SDLKeyBible::GetKeyFromScancode(set));
			this->generateGridObjectDefaultToolTip();
		}
	}
}

std::string LauncherEntity::GetToolTip()
{
	string name = this->GetIDName();
	string type = "Cannon\n";
	string rateoffire = "Rate of fire: " + IntToString(this->GetRateOfFire());
	string rotating = this->GetAngularVelocity() == 0 ? "Non rotating" : "Rotation speed: " + IntToString(this->GetAngularVelocity());
	string velocity = "Fire velocity: " + IntToString( this->getProjectile()->GetAbsVelocity());

	return name + "\n" + type + "\n" + rateoffire + "\n" + velocity + "\n" + rotating;
}

string LauncherEntity::GetHotkeyTooltip()
{
	string hotkeyChar = SDL_GetScancodeName(this->pHotKey);
	string hotkey = "Hotkey: '" + hotkeyChar + "'";
	return hotkey;
}

LauncherEntity::~LauncherEntity()
{
	//Mix_FreeChunk(blasterFire);
	//Mix_FreeChunk(blasterCoolDown);
	clearProjectiles();
	ModuleEntity::FreeMemory();
}

