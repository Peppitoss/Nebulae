//#pragma once
//#include"scrList.h"
//#include"Vessel.h"
//#include<string>
//
//using std::string;
//
//class scrHUDTargetList  : public ListFrame
//{
//public:
//
//	scrHUDTargetList();
//	scrHUDTargetList( const Coord& dimensions, const Coord& pos );
//
//	void RefreshVesselList();
//
//protected:
//
//	void HandleActiveWidget( SDL_Event& e, Widget* widget ) override;
//	void Setup( Coord dimensions, Coord pos );
//	void AddList(const Coord& pos) override;
//
//	//void HandleWidgetByState(SDL_Event& e) override;
//	//void SelectWidgetByIndex(SDL_Event& e, int index) override;
//	std::shared_ptr<SelectLink> AddItemToList( std::shared_ptr<SelectLink> item, const Coord& offset) override;
//
//private:
//
//	void init();
//	int SelectedVesselID;
//	std::shared_ptr<Widget> pCaption;
//
//	std::list<string> pVesselNameList;
//	std::list<string> GenerateVesselNameList( const std::vector<std::shared_ptr<UnitEntity>>& vessels);
//	int GetVesselIDFromText( std::string text );
//	void AddCaption();
//};