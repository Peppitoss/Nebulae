#pragma once
#include"ModuleEntity.h"
#include"RadarScreenSlider.h"

class RadarEntity : public ModuleEntity
{

public:

	RadarEntity();
	RadarEntity( std::shared_ptr<FiguredGraphics> ge, string name);

	bool GetAutoAim() const;
	void SetAutoAim( bool set );

	int GetTrackingAccuracy() const;
	void SetTrackingAccuracy( int set );

	void SetMaxTargetTrackingCount( int set );
	int GetMaxTargetTrackingCount() const;

	void SetFieldOfView( int set );
	int GetFieldOfView() const;

	void SetDetached(bool set, int timed, const Coord& atPosition);

	void SetStarmapRange( int set );
	int GetStarmapRange() const;

	void DisableTrackers();
	bool TrackVessel( std::shared_ptr<GenericEntity> reference );	//return false if all trackers are used and the reference vessel can't be tracked by radar

	void HighlightScreenSlider( GenericEntity* target );
	void DisableHighlightOfSliders();

	std::vector<std::shared_ptr<GenericEntity>> GetTrackedTargets();

	std::vector<std::shared_ptr<RadarScreenSlider>>::const_iterator const pbegin() const { return pTrackedTargetsList.begin(); };
	std::vector<std::shared_ptr<RadarScreenSlider>>::const_iterator const pend() const { return pTrackedTargetsList.end(); };

	std::vector<std::shared_ptr<RadarScreenSlider>> GetInactiveSliders();

protected:

	void generateGridObjectDefaultToolTip() override;

private:

	void initialize();
	std::string GetToolTip();


	std::vector<std::shared_ptr<RadarScreenSlider>> pTrackedTargetsList;

	//radar will compensate starmap range versus field of view and max tracking count
	//radar tracking level is also relevant, where the best radars will offer an autoaim ( crosshair ) to help the player aim

	bool pAutoaim;

	int pTrackingAccuracy;		//100 best, 0 awful
	int pMaxTargetTrackingCount;
	int pFieldOfView; //in degrees
	int pStarmapRange;

};