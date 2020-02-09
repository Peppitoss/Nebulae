#include"RadarEntity.h"
#include"StringBuilder.h"


RadarEntity::RadarEntity() : ModuleEntity()
{
	initialize();
}

RadarEntity::RadarEntity( std::shared_ptr<FiguredGraphics> ge, string name) : ModuleEntity(ge, name)
{
	initialize();
}

void RadarEntity::generateGridObjectDefaultToolTip()
{
	this->generateGridObjectToolTip( GetToolTip() );
}

std::string RadarEntity::GetToolTip()
{
	string name = this->GetIDName();
	string type = "Broad Frequency Transmission Radar\n";
	string targets = "Simultaneous tracking targets: " + IntToString(this->GetMaxTargetTrackingCount());
	string accuracy = "Track accuracy: " + IntToString(this->GetTrackingAccuracy());
	string fieldOfView = "Field of view: " + IntToString(this->GetFieldOfView());
	string starmaprange = "Starmap range: " + IntToString(this->GetStarmapRange());

	return name + "\n" + type + "\n" + targets + "\n" + accuracy + "\n" + fieldOfView + "\n" + starmaprange;
}

void RadarEntity::initialize()
{
	this->SetModuleType( enModuleType::mRadar );

	this->pAutoaim = false;

	this->pTrackingAccuracy = 50;
	this->pMaxTargetTrackingCount = 5;
	this->pFieldOfView = 360;
	this->pStarmapRange = 20;

	for( int i = 0; i < this->pMaxTargetTrackingCount; i++ )
	{
		pTrackedTargetsList.push_back( std::shared_ptr<RadarScreenSlider>( new RadarScreenSlider() ) ); 
	}
}
	
void RadarEntity::DisableTrackers()
{
	for( auto it = this->pTrackedTargetsList.begin(); it != this->pTrackedTargetsList.end(); it++ )
	{
		(*it)->Deactivate();
	}
}
bool RadarEntity::TrackVessel(std::shared_ptr<GenericEntity> reference )
{
	if (reference->isDestroyed())
		return false;

	std::shared_ptr<RadarScreenSlider> sliderToTrack = nullptr;

	for( auto it = this->pTrackedTargetsList.begin(); it != this->pTrackedTargetsList.end(); it++ )
	{
		if (!(*it)->IsActive())
		{
			sliderToTrack = (*it);
		}
		else if((*it)->GetTargetReference() == reference)
			return false;	//vessel already tracked by radar
	}
	if (sliderToTrack != nullptr)
	{
		sliderToTrack->SetTarget(reference);
		sliderToTrack->Activate();
		return true;
	}
	return false;
}

void RadarEntity::HighlightScreenSlider( GenericEntity* target )
{
	if( target == nullptr || target->GetID() == -100 )
		DisableHighlightOfSliders();
	else
	{
		for( std::vector<std::shared_ptr<RadarScreenSlider>>::iterator it = this->pTrackedTargetsList.begin(); it != this->pTrackedTargetsList.end(); ++it )
		{
			if( !(*it)->IsActive() )
				continue;

			int targetreferenceID = (*it).get()->GetTargetReference()->GetID();
			if( targetreferenceID != -100 &&  targetreferenceID == target->GetID() )
				(*it)->SetState( enRadarSlideState::enSliderTarget );
			else
				(*it)->SetState( enRadarSlideState::enSliderDefault );
		}
	}
}

void RadarEntity::DisableHighlightOfSliders()
{	
	for( std::vector<std::shared_ptr<RadarScreenSlider>>::iterator it = this->pTrackedTargetsList.begin(); it != this->pTrackedTargetsList.end(); ++it )
	{
		(*it)->SetState( enRadarSlideState::enSliderDefault );
	}
}

std::vector<std::shared_ptr<GenericEntity>> RadarEntity::GetTrackedTargets()
{
	std::vector<std::shared_ptr<GenericEntity>> result = std::vector<std::shared_ptr<GenericEntity>>();
	for (auto it = this->pTrackedTargetsList.begin(); it != this->pTrackedTargetsList.end(); ++it)
	{
		if ((*it)->IsActive())
			result.push_back((*it)->GetTargetReference());
	}
	return result;
}

std::vector<std::shared_ptr<RadarScreenSlider>> RadarEntity::GetInactiveSliders()
{
	std::vector<std::shared_ptr<RadarScreenSlider>> result = std::vector<std::shared_ptr<RadarScreenSlider>>();
	for (auto it = this->pTrackedTargetsList.begin(); it != this->pTrackedTargetsList.end(); ++it)
	{
		if ((*it)->IsActive() == false)
			result.push_back(*it);
	}
	return result;
}

bool RadarEntity::GetAutoAim() const
{
	return this->pAutoaim;
}
void RadarEntity::SetAutoAim( bool set )
{
	this->pAutoaim = set;
}

int RadarEntity::GetTrackingAccuracy() const
{
	return this->pTrackingAccuracy;
}
void RadarEntity::SetTrackingAccuracy( int set )
{
	this->pTrackingAccuracy = set;
}

void RadarEntity::SetMaxTargetTrackingCount( int set )
{
	this->pMaxTargetTrackingCount = set;
}

int RadarEntity::GetMaxTargetTrackingCount() const
{
	return this->pMaxTargetTrackingCount;
}

void RadarEntity::SetFieldOfView( int set )
{
	this->pFieldOfView = set;
}

int RadarEntity::GetFieldOfView() const
{
	return this->pFieldOfView;
}

void RadarEntity::SetDetached(bool set, int timed, const Coord& atPosition)
{
	ModuleEntity::SetDetached(set, timed, atPosition);
	this->DisableTrackers();
}

void RadarEntity::SetStarmapRange( int set )
{
	this->pStarmapRange = set;
}

int RadarEntity::GetStarmapRange() const
{
	return this->pStarmapRange;
}
