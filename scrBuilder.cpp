#include"scrBuilder.h"
#include"scrShipBuilder.h"
#include"scrVesselSelector.h"
#include"VesselGlobalList.h"
#include"ModuleInterfaceGlobalList.h"
#include"Globals.h"



scrBuilder::scrBuilder()
{
	Init();
	this->AddGridScreen( std::shared_ptr<GridObjectFrame>( new ShipBuilderFrame( spriteSize( spriteSize(640) ), Coord(DEFAULT_SCREEN_WIDTH/2 -640/2,DEFAULT_SCREEN_HEIGHT/2 - 640/2) ) ), strMain);
	this->AddGridScreen( std::shared_ptr<GridObjectFrame>( new GridObjectViewerFrame( spriteSize( spriteSize::GetSize256() ), Coord(DEFAULT_SCREEN_WIDTH-640/2 + 4,DEFAULT_SCREEN_HEIGHT/2 - 640/2) ) ), strViewer);
	this->AddScreen( std::shared_ptr<Screen>( new GridObjectSelector( Coord(256,384), Coord(DEFAULT_SCREEN_WIDTH-640/2 + 4,DEFAULT_SCREEN_HEIGHT/2-256/2 + 64) ) ), strModuleSelector);
	this->AddScreen( std::shared_ptr<Screen>( new VesselSelector( Coord(256,384), Coord(32,DEFAULT_SCREEN_HEIGHT/2 -640/2) ) ), strVesselSelector);

	this->GetGridViewerFrame()->SetRegenerateObjects(true);
	this->GetGridViewerFrame()->SetAccepTransfers(false);
}

void scrBuilder::Init()
{
	strMain = "main";
	strViewer = "viewer";
	strModuleSelector = "selector";
	strVesselSelector = "vselector";
}

void scrBuilder::openBuilder()
{
	this->GetGridScreen(strMain)->OpenScreen();
	this->GetGridScreen(strViewer)->OpenScreen();
	this->GetScreenByName(strModuleSelector)->OpenScreen();
	this->GetBuildFrame()->RefreshVessel();
}

void scrBuilder::closeBuilder()
{
	this->GetGridScreen(strMain)->CloseScreen();;
	this->GetGridScreen(strViewer)->CloseScreen();;
	this->GetScreenByName(strModuleSelector)->CloseScreen();;
	this->GetScreenByName(strVesselSelector)->CloseScreen();;
}

void scrBuilder::openImport()
{
	this->GetGridVesselSelector()->SetTemplateSelection(true);
	this->GetScreenByName(strVesselSelector)->OpenScreen();
}

void scrBuilder::closeImport()
{
	this->GetScreenByName(strVesselSelector)->CloseScreen();;
}

void scrBuilder::ImportVessel( std::shared_ptr<Vessel> vessel )
{
	this->GetBuildFrame()->GetGrid()->RemoveAllGridObjects();
	GetBuildFrame()->GetGrid()->SetShowVesselOutside(false);
	GetBuildFrame()->LoadVessel(vessel);	//why is this run twice? ?? bug!
	this->pVesselWriter.SetSurfaceName(vessel->GetUnitName());
}

void scrBuilder::OpenVesselLoad()
{
	this->GetGridVesselSelector()->SetTemplateSelection(false);
	this->GetScreenByName(strVesselSelector)->OpenScreen();
}

void scrBuilder::LoadVessel(std::shared_ptr<Vessel> newVessel)
{
	this->GetBuildFrame()->GetGrid()->RemoveAllGridObjects();
	GetBuildFrame()->GetGrid()->SetShowVesselOutside(false);
	GetBuildFrame()->LoadVessel(newVessel);
	//this->pVesselWriter.SetSurfaceName( VesselGlobalList::GetVesselInterface(newVessel->GetShipName()).GetSurfaceName() );
}

void scrBuilder::SaveVessel(string name)
{
	Vessel* writeVessel = GetBuildFrame()->GetGrid()->GetModelVessel();
	if( writeVessel != nullptr )
	{
		ExchangeRealObjects();
		pVesselWriter.LoadFile("../Data/Vessel/", name + ".txt" );
		pVesselWriter.ModelVessel = GetBuildFrame()->GetGrid()->GetModelVessel();
		pVesselWriter.WriteVessel();

		//this is important so that gridobjects are refreshed, see exchangerealobjects, (the newly made modules do not have the same gridobjects as the ones found in the ship grid)
		VesselGlobalList::ClearInterface( name );
		//this->GetGridVesselSelector()->LoadVessel( name );
	}
}

void scrBuilder::LoadModule( string name )
{
	this->GetGridViewerFrame()->AddNewViewedGridObject(name);
}

void scrBuilder::HandleEvent( SDL_Event& e )
{
	GridScrHandler::HandleEvent(e);
}

ShipBuilderFrame* scrBuilder::GetBuildFrame()
{
	return (ShipBuilderFrame*)this->GetGridScreen(strMain);
}

GridObjectViewerFrame* scrBuilder::GetGridViewerFrame()
{
	return (GridObjectViewerFrame*)this->GetGridScreen(strViewer);
}

GridObjectSelector* scrBuilder::GetGridSelector()
{
	return (GridObjectSelector*)(this->GetScreenByName(strModuleSelector).get());
}

VesselSelector* scrBuilder::GetGridVesselSelector()
{
	return (VesselSelector*)(this->GetScreenByName(strVesselSelector).get());
}


void scrBuilder::FillGlobalObjectList()
{
	GlobalObjectList.splice(GlobalObjectList.end(), this->GetBuildFrame()->GetGridObjectContainerFromFrame() );
}

//ONLY USE THIS FUNCTION ON EXITING THE GRAPH!
//After saving, reload the name!
void scrBuilder::ExchangeRealObjects()
{
	for( auto it = this->GlobalObjectList.cbegin(); it != this->GlobalObjectList.cend(); ++it )
	{
		//check ship/inventory for orignial objects
		UpdateShipObjects( (*it).get() );
	}

	//GlobalObjectList.clear();
	//FillGlobalObjectList();

	//continue check all grid objects in vessel, add them as modules (if they are not already added)
	Vessel* vessel = this->GetBuildFrame()->GetGrid()->GetModelVessel();
	for( auto it = this->GetBuildFrame()->GetGrid()->gridObjectBegin(); it != this->GetBuildFrame()->GetGrid()->gridObjectEnd(); ++it )
	{
		if( !vessel->ContainsGridObject( (*it) , false ))
		{
			std::shared_ptr<ModuleEntity> moduleentity = std::shared_ptr<ModuleEntity>( ModuleGlobalList::GetModule( (*it)->GetIDName() ) );
			moduleentity->SetFlipped((*it)->GetFlipped());
			moduleentity->SetRotation((*it)->GetDiscreteRotation());
			vessel->AddModuleEntity( moduleentity, (*it)->getRelativePosition(), Coord(vessel->getSize()/2 - moduleentity->getSize()/2));

			//P.S
			//or use ?? : ModuleGlobalList::GetModule( argument here is grid object, so that the object is reused ?? and thus comparable to the grid object already in grame?? )
		}
	}
}

bool scrBuilder::UpdateShipObjects( GridObjectContainer* container )
{
	bool result = false;
	if( !this->GetBuildFrame()->GetGrid()->ContainsGridObject( container->GetGridObject() ) )
	{
		//ship had grid object, but doesn't anymore (remove it)
		Vessel* vessel = this->GetBuildFrame()->GetGrid()->GetModelVessel();
		switch( container->GetType() )
		{
		case( enGOContainerType::Pure ) :
			break;	
		case( enGOContainerType::Item ) :
			break;
		case( enGOContainerType::Module ) :
			std::shared_ptr<ModuleEntity> moduleentity = ((GridObjectModuleContainer*) container)->GetModule();
			vessel->RemoveModuleEntity( moduleentity );
			result = true;
			break;
		}	
		
	}
		
	return result;
}

//bool scrBuilder::UpdateLootObjects( GridObjectContainer* container )
//{
//	bool result = false;
//	Vessel* vessel = this->GetBuildFrame()->GetGrid()->GetModelVessel();
//	if( this->GetGridViewerFrame()->GetGrid()->ContainsGridObject( container->GetGridObject() ) )
//	{
//		result = true;
//		switch( container->GetType() )
//		{
//		case( enGOContainerType::Pure ) :
//			break;	
//		case( enGOContainerType::Item ) :
//			break;
//		case( enGOContainerType::Module ) :
//			std::shared_ptr<ModuleEntity> moduleentity = ((GridObjectModuleContainer*) container)->GetModule();
//			if( vessel->ContainsModule( moduleentity ) )
//			{
//				//vessel had grid module (but its not there anymore)
//				vessel->RemoveModuleEntity( moduleentity );
//			}
//			else
//			{
//				//vessel has not module, and it wasn't there anyway
//			}
//			break;
//		}	
//	}
//	return result;
//}