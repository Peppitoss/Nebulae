#pragma once
#include"DockingEntity.h"
#include"LauncherEntity.h"


//Obsolete
//Obsolete
//Obsolete
//Obsolete
class GunPlatform : public DockingEntity
{

public:

	GunPlatform();
	GunPlatform(const Coord pos);

	void SetTarget( DynamicEntity* );
	void AddLauncher( std::shared_ptr<LauncherEntity> launcher, Coord pos, Coord anchorPos );

	void fire();
	void deFire();
	

	void UpdateBehavior();
	bool isDestroyed() override;
	void setGlobalHeading( double newHeading ) override;
	void Disable();

	//void driveUpdateForDynamicPositioningHeading( int milliseconds ) override;

	std::list<std::shared_ptr<LauncherEntity>>::const_iterator const LE_begin() const { return wplatforms.begin(); };
	std::list<std::shared_ptr<LauncherEntity>>::const_iterator const LE_end() const { return wplatforms.end(); };

	~GunPlatform();

private:

	bool pDestroyed;
	double pAccuracy;
	double pPrecision;

	DynamicEntity* pTarget;
	std::list<std::shared_ptr<LauncherEntity>> wplatforms;

	void Initialize();
	

};