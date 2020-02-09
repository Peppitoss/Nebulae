#pragma once
#include"ModuleEntity.h"
#include"ItemAmmo.h"
#include"Projectile.h"


class LauncherEntity : public ModuleEntity
{
public:
	//std::unique_ptr<Mix_Chunk> blasterFire; //deletion causes exception ?? 
	//std::unique_ptr<Mix_Chunk> blasterCoolDown;
	char* blasterFire_path;
	char* blasterCoolDown_path;

	LauncherEntity();
	LauncherEntity( std::shared_ptr<FiguredGraphics> ge, std::shared_ptr<Projectile> proj, string name );
	LauncherEntity( std::shared_ptr<FiguredGraphics> ge, primitives::vector dir, std::shared_ptr<Projectile> proj, Coord lPoint, string name);
	void FireBlaster(bool);
	//
	void initializeSoundEffects();
	void initialize();
	virtual std::shared_ptr<Projectile> launch(); // is a new functions, delete using clearProjectiles

	void generateGridObjectForItem(const BoxList& gridPos) override;
	void generateGridObjectForModule(CoordMap<GriddedBox>& list, imagehandling::IMGToSurface* model, int shipSize) override;

	Coord getLaunchPoint();
	primitives::vector getDirection();
	primitives::vector getRefDirection();
	Coord getRelativeProjectileLaunchPoint();

	std::shared_ptr<Projectile> getProjectile() { return pProjectileTemplate; };
	//std::list<std::shared_ptr<Projectile>> getLaunchedProjectiles() { return launchedProjects; };
	bool ReadyToLaunch() { return pLaunch && this->CanLaunch(); };
	Uint32 getLastTimeFired() { return lastFireTime; };
	int GetRateOfFire();
	void setLastFire( Uint32 set );
	void setRateOfFire( int rof );
	void setLaunch( bool set );
	void setProjectile( std::shared_ptr<Projectile> set );
	void setDirection( primitives::vector dir );
	void setDirection( double dirAngle );
	void setRelativeLaunchPoint( Coord set  );
	void clearProjectiles();

	void SetDetached(bool set, int timed, const Coord& atPosition) override;

	void setLaunchPoint( Coord pos);
	//void reduceTimeOnProjectiles( double millisec );

	//std::list<std::shared_ptr<Projectile>>::iterator const pbegin() { return launchedProjects.begin(); };
	//std::list<std::shared_ptr<Projectile>>::iterator const pend() { return launchedProjects.end(); };

	std::string GetAmmoType() const { return this->pProjectileTemplate->GetProjectileName(); };
	
	bool AllowToHitMothership() const { return pAllowHitMotherShip; };
	void SetAllowToHitMothersip( bool set );

	bool IsAutoFire();
	void SetAutoFire( bool set );

	void ChangeMothership( GenericEntity* set );
	GenericEntity* GetMothership();

	void ChangeAngularVelocity( double set );
	double GetAngularVelocity();

	void SetTargetScanRange( double set );
	double GetTargetScanRange();

	bool IsPlayeAlly() { return this->pPlayeAlly; };
	void MakePlayerAlly( bool set ) { this->pPlayeAlly = set; };

	bool TargetWithinRange();
	bool TargetWithinRange( std::shared_ptr<GenericEntity> );

	bool HasTarget() const;
	void UpdateAim( int ms );
	virtual void SetAimTarget(std::shared_ptr<GenericEntity> target );
	std::weak_ptr<GenericEntity> GetAimTarget() const;
	
	SDL_Scancode GetHotKey() const { return this->pHotKey; }
	void SetHotKey(SDL_Scancode set);

	virtual bool CanTargetVessel();
	string GetHotkeyTooltip();

	int GetLaunchedProjectiles() { return SDL_AtomicGet(&this->pLaunchedProjectiles); };
	void ResetLaunchedProjectiles() {SDL_AtomicSet(&pLaunchedProjectiles,0); };

	~LauncherEntity();

protected:

	double pAngularVelocity;
	int rateOfFire; //milliseconds
	bool pLaunch;
	bool pAllowHitMotherShip;
	bool pAutoFire;
	double pTargetScanRange; // at what range does autofire respond?
	bool pPlayeAlly;
	Uint32 lastFireTime; //milliseconds

	std::shared_ptr<Projectile> pProjectileTemplate;
	//std::list<std::shared_ptr<Projectile>> launchedProjects; //must be cleared!!  no such function yet  exists
	
	game::LauncherAttributes pAttributes;

	primitives::vector pRefDirection;
	primitives::vector pDirection;			//pDirection is not needed now, pRefDirection used together with heading! is unless some magic code is wanted
	Coord pProjectileLaunchPoint;
	//Coord pRefProjectileLaunchPoint;
	Coord pRelativeProjectileLaunchPoint;

	void UpdateHotKeysFromGrid() override;

	LauncherEntity( const LauncherEntity & );
	LauncherEntity( LauncherEntity && );
	LauncherEntity operator=( const LauncherEntity& );
	LauncherEntity operator=( LauncherEntity && );

	GenericEntity* pMothership;
	std::weak_ptr<GenericEntity> pAimTarget;

	virtual bool CanLaunch();

	void generateGridObjectDefaultToolTip() override;

	SDL_atomic_t pLaunchedProjectiles;

private:
	
	
	//std::string pAmmoType;
	SDL_Scancode pHotKey;
	virtual std::string GetToolTip();

};
