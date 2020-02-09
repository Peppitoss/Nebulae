#pragma once
#include"BoxList.h"
#include"DynamicEntity.h"
#include"FiguredGraphics.h"
#include"SpriteGraphicalObject.h"
#include"GridObject.h"
#include"ParticleEngine.h"
#include <SDL_mixer.h>
#include"AnimationContainer.h"
#include"InventoryItem.h"
#include<functional>

#include<list>
class BoxList;

//class ModuleEntity;
//

enum enModuleType
{
	mNone = 0,
	mEngine = 1,
	mController = 2,
	mPowerSupply = 3,
	mLauncher = 4,
	mMissileLauncher = 5,
	mRadar = 6,
	mRoom = 7,
	mTorpedoLauncher = 8,
	mRailgunLauncher = 9,
	mAntiMLauncher = 10,
	mHitBox = 11,
	mChassis = 12,
	mWing = 13,
	mEngineBody = 14,
	mFrame = 15,
	mBridge = 16,
	mAmmoContainer = 17,
	mInterior = 18,
	mDecoration = 19,
	mDoor = 20
};


struct ModuleDefinition
{
	ModuleDefinition();
	enModuleType ModuleType;
};


class ModuleEntity : public DynamicEntity, public InventoryItem//, public ITransferObject
{
public:

	ModuleEntity();
	ModuleEntity( std::shared_ptr<GraphicalEntity>, string name );

	Coord getRelativePosition() const { return *pRelativePosition; };
	void setRelativePosition( std::shared_ptr<Coord> pos );

	virtual void refreshState();
	virtual void refreshHotKeys();
	virtual void RefreshGridObjectColour();

	/*virtual void generateGridObjectForModule( const BoxList& gridPos );*/
	//void generateGridObjectForModule( BoxList list, const GridObjectTexturePack&, int shipSize );
	//void generateGridObjectForModule( const BoxList& list, const imagehandling::IMGToSurface* model, int shipSize );
	//void generateGridObjectForModule( const BoxList& list, imagehandling::IMGToSurface* model, int shipSize );
	virtual void generateGridObjectForModule( CoordMap<GriddedBox>& list, imagehandling::IMGToSurface* model, int shipSize );
	void generateGridObjectToolTip(std::string tooltip) override;

	void updateVelocity(Uint32 millisecond) override;
	void setUnitHeading(double newHeading) override;
	void move(Uint32 milliseconds) override;

	void SetRelativeHeading(double value);//, bool updateUnitHeading );
	double GetRelativeHeading();

	primitives::vector ChangeInPosition(const primitives::vector& velocity, int milliseconds) override; //	virtual primitives::vector ChangeInPosition(const primitives::vector& velocity, int milliseconds);

	double GetEfficiency();
	void AddDamageRecieved(const game::Damage& set ) override;

	//void SetDamageRecieved(const game::Damage& set ) override;
	//void setGridObjectPosition( Coord pos) { this->pGridGraphics.get()->setRelativePosition( pos ); };
	void SetGridObjectRestriction( const BoxList& list );
	//std::shared_ptr<GridObject> getGridObject() { return pGridGraphics; };
	/*bool hasGridObject() const { return pGridGraphics.get(); }*/

	std::string GetIDName() const { return this->IDName; };
	void SetIDName(std::string set) { IDName = set; };

	EnumParticleEffect GetParticleEffect() const { return this->enParticleEffect; };
	void SetParticleEffect(EnumParticleEffect set) { this->enParticleEffect = set; };

	enModuleType GetModuleType() const { return mDefinition.ModuleType; };
	void SetModuleType( enModuleType type ) { mDefinition.ModuleType = type; };
	void SetFlipped(enObjectFlipped flipped);
	void SetRotation(enObjectDiscreteRotation rotation);
	void TriggerParticleEffect( int ms, const Coord offset );

	bool IsLauncherType() const;
	bool IsExternalType() const;

	~ModuleEntity();
	virtual void FreeMemory() override;

	void Enable() 
	{ 
		this->pDisabled = false; 
		if (OnChangedState)
			OnChangedState(this);
	};
	void Disable() 
	{ 
		this->pDisabled = true; 
		if (OnChangedState)
			OnChangedState(this);
	};
	void ToggleEnability() { this->pDisabled = !this->pDisabled; };
	bool IsEnabled() { return this->pDisabled == false; };

	virtual void SetDetached(bool set, int timed, const Coord& atPosition );
	void SetDetached(bool set);
	bool IsDetached() const { return this->pDetached == true; };
	std::shared_ptr<FiguredGraphics> GetModuleGraphics() const;
	const std::shared_ptr<PixelFigure> GetFigure() const;
	int GetGridBoxesInFigure();

	std::string ToWriteString() const;

	void ApplyNormalModuleImage();
	void ApplyDamagedModuleImage();
	void ApplyDestroyedModuleImage();
	Coord TransformPositionToModuleFrame(const Coord& pos, const Coord& unitCenter) const;
	Coord ReverseTransformPositionToModuleFrame(const Coord& pos, const Coord& unitCenter) const;

	void ExecuteLocomotion(int milliseconds, const primitives::vector& changeinPos);
	void SetStepLocomotionSpeed(int set) { this->pStepLocomotion.SetSpeed(set); };
	bool LocomotionCanExecute() const { return this->pStepLocomotion.CanExecute(); }
	double GetStepLocomotionSpeed() const { return pStepLocomotion.GetSpeed(); };
	void ResetLocomotionCounter() { this->pStepLocomotion.ResetCounter(); }
	 
	void AddLocomotionFrame(const Coord Coord);
	void ClearStepLocomotionFrames() { this->pStepLocomotion.GetFrames().clear(); }
	//int AnimationFrames() const { return this->pStepLocomotion.GetFrameCount(); }



	AnimationContainer& GetAnimationContainer() { return this->pGraphics->getGraphicalObject()->getMainObject()->GetAnimationContainer(); };
	/*std::shared_ptr<FrameStrip> GetAnimation(int index) const;
	std::shared_ptr<FrameStrip> GetAnimation(string ID) const;
	bool DrawAnimations() const { return this->pGraphics->getGraphicalObject()->getMainObject()->DrawAnimations(); };
	void SetAnimationDrawing(bool set) { this->pGraphics->getGraphicalObject()->getMainObject()->SetAnimationDrawing(set); };
	bool HasAnimations() { return this->pGraphics->getGraphicalObject()->getMainObject()->HasAnimations(); };
	bool HasAnimationID(string ID) const;
	void AddAnimation(std::shared_ptr<FrameStrip> strip) { this->pGraphics->getGraphicalObject()->getMainObject()->AddAnimation(strip); }
	void RunAnimations(int ms);*/

	//std::vector<std::shared_ptr<FrameStrip>>::const_iterator const AniFrame_begin() const { return pModuleAnimations.begin(); };
	//std::vector<std::shared_ptr<FrameStrip>>::const_iterator const AniFrame_end() const { return pModuleAnimations.end(); };

	//void IncludeSellValueInTooltip(bool value) override;
	//void IncludePriceValueInTooltip(bool value) override;

	void IncreaseLocomotionFrame();
	void GenerateModuleGridBox(int gridsize);
	//EntityPrice GetPrice() const { return this->pPrice; }
	//void SetPrice(statistics::RandomNumber newPrice) { this->pPrice.SetNewPrice(newPrice); }
	
	std::function<void(ModuleEntity* w)> OnChangedState;

protected:

	virtual void SetEfficiency( double set );
	void UpdateRelativePositionFromGrid();
	virtual void UpdateHotKeysFromGrid();
	ModuleDefinition mDefinition;

private:
	
	int particleTriggerTime;
	EnumParticleEffect enParticleEffect;
	std::string IDName;
	void Initialize();
	double pEfficiency; //100% effective at zero damage, 0% effective at destruction
	//std::shared_ptr<GridObject> pGridGraphics;
	std::shared_ptr<Coord> pRelativePosition;



	EntityStepLocomotion pStepLocomotion;
	bool pDisabled;
	bool pDetached;

	bool pIncludeHPInTooltip;
	//bool pIncludePriceInTooltip;
	//bool pIncludeSellValueInTooltip;
	
	
};


 