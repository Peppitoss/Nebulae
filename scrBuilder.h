#pragma once
#include"GridScrHandler.h"
#include"scrShip.h"
#include"ScrGridObjectViewer.h"
#include"scrGridObjectSelector.h"
#include"scrVesselSelector.h"
#include"VesselWriter.h"
#include"OkCancelForm.h"

class ShipBuilderFrame;
class GridObjectViewerFrame;
//class OkCancelForm;

class scrBuilder : public GridScrHandler
{
public:

	scrBuilder();

	void openBuilder();
	void closeBuilder();

	void openImport();
	void closeImport();

	void LoadModule( string name );

	void ImportVessel( std::shared_ptr<Vessel> name );
	void OpenVesselLoad();
	void LoadVessel(std::shared_ptr<Vessel> name);
	void SaveVessel(string name);

	void HandleEvent( SDL_Event& e ) override;

	GridObjectSelector* GetGridSelector();
	GridObjectViewerFrame* GetGridViewerFrame();
	VesselSelector* GetGridVesselSelector();
	ShipBuilderFrame* GetBuildFrame();
	//OkCancelForm* GetOkCancelForm();


protected:

	void FillGlobalObjectList() override;

private:

	string strModuleSelector;
	string strVesselSelector;
	string strMain;
	string strViewer;
	void Init();

	void ExchangeRealObjects();
	bool UpdateShipObjects( GridObjectContainer* container );
	//bool UpdateLootObjects( GridObjectContainer* container );

	VesselWriter pVesselWriter;

};