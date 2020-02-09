//#include"scrHUDTargetList.h"
//#include"Graphics.h"
//#include"StringBuilder.h"
//#include"GameManager.h"
//
//scrHUDTargetList::scrHUDTargetList()
//{
//	init();
//}
//
//scrHUDTargetList::scrHUDTargetList( const Coord& dimensions, const Coord& Coord) : ListFrame( dimensions, Coord )
//{
//	init();
//	Setup(dimensions, Coord);
//}
//
//void scrHUDTargetList::RefreshVesselList()
//{
//	this->init();
//	this->Control()->ClearWidgets();
//	this->AddCaption();
//	std::vector<std::shared_ptr<UnitEntity>> list1 = GameManager::CurrentInstanceManager()->GetCurrentPlayer()->GetTrackedTargets();
//	std::vector<std::shared_ptr<UnitEntity>> list2 = GameManager::CurrentInstanceManager()->GetIntegrator()->GetUnitEntities(false);
//		for (auto it = list2.begin(); it != list2.end(); ++it)
//			list1.push_back(*it);
//	GenerateVesselNameList(list1);
//	this->pBottomListItemPlacement = this->pTopTextPlacement + 8;
//	this->AddList(Coord(0,0));
//}
//
//void scrHUDTargetList::Setup( Coord dimensions, Coord pos )
//{
//	//this->SetupBarPlacement( pos.y, pos.y + dimensions.y );
//	this->AddCaption();
//	ListFrame::AddList();
//}
//
////void scrHUDTargetList::SelectWidgetByIndex(SDL_Event& e, int index)
////{
////	this->_widgets[index]->Activate(e);
////	this->pIndexOfActiveWidget = index;
////}
//
//std::list<string> scrHUDTargetList::GenerateVesselNameList( const std::vector<std::shared_ptr<UnitEntity>>& vessels)
//{
//	std::list<string> result = std::list<string>();
//	for( auto it = vessels.begin(); it != vessels.end(); ++it )
//	{
//		if((*it)->isDestroyed() == false && (*it)->IsSpawned() == true)
//			result.push_back( (*it)->GetUnitName() + " : " + IntToString( (*it)->GetID() ) );
//	}
//	this->pVesselNameList = result;
//	return result;
//}
//
////void scrHUDTargetList::HandleWidgetByState(SDL_Event& e)
////{
////	for (int i = 0; i < _widgets.size(); i++)
////	{
////		if (_widgets[i]->GetState() == WIDGET_STATE::WS_ACTIVE)
////		{
////			SelectWidgetByIndex(e, i);
////			break;
////		}
////	}
////}
//
//void scrHUDTargetList::HandleActiveWidget( SDL_Event& e, Widget* widget )
//{
//	//ListFrame::HandleActiveWidget(e,widget); //for scrolling of list
//
//	std::string txt = widget->ID();
//	int newSelectedID = GetVesselIDFromText(txt);
//
//	if(this->SelectedVesselID != newSelectedID)
//	{
//		if(SelectLink *button = dynamic_cast<SelectLink*>(widget))
//		{
//			if (VisibleInsideWindow(button->GetPosition().y))
//			{
//				this->SelectedVesselID = newSelectedID;
//				std::shared_ptr<UnitEntity> targetVessel = GameManager::CurrentInstanceManager()->GetIntegrator()->GetUnitEntityFromID(newSelectedID);
//				GameManager::CurrentInstanceManager()->GetCurrentPlayer()->AsUnitEntity()->TargetVessel(targetVessel);
//			}
//		}
//	}
//	else
//	{
//	}
//}
//
//void scrHUDTargetList::AddCaption()
//{
//	pCaption = std::shared_ptr<Widget>( new Link( BtnText("Nearby vessels:"), 14, this->GetPosition(), 24, 64+16 ));
//	pCaption.get()->SetWidgetMode( WIDGET_MODE::WL_NON_INTERACTIVE );
//	//((Link*)pCaption.get())->autosize = true;
//	//((Link*)pCaption.get())->SetBackgroundColor( ColorRGB(150,150,150,150) );
//	((Link*)pCaption.get())->Refresh();
//	this->Control()->AddWidget( pCaption );
//
//}
//
//int scrHUDTargetList::GetVesselIDFromText( std::string text )
//{
//	char currentletter = text[0];
//	int counter = 0;
//
//	while( currentletter != ':' )
//	{
//		currentletter = text[counter++];
//	}
//
//	string numbertext = "";
//
//	for( int i = counter; i < text.size(); i++ )
//	{
//		numbertext += text[i];
//	}
//
//	int result = std::atoi( numbertext.c_str() );
//	return result;
//}
//
//void scrHUDTargetList::init()
//{
//	SelectedVesselID = -100;
//}
//
//void scrHUDTargetList::AddList(const Coord& pos)
//{
//	for( auto it = pVesselNameList.begin(); it != pVesselNameList.end(); ++it )
//	{
//		ListFrame::AddItemToList(*it, Coord(-32,16));
//	}
//}
//
//std::shared_ptr<SelectLink> scrHUDTargetList::AddItemToList( std::shared_ptr<SelectLink> item, const Coord& offset)
//{
//	//item->autosize = true;
//	//item->SetBackgroundColor( ColorRGB(0,225,0,80) );
//	//item->SetSelectionColor( ColorRGB(0,225,0,250) );
//	item->Refresh();
//	this->Control()->AddWidget(item);
//	pBottomListItemPlacement += pTextHeight + 8;
//
//	std::string txt = item->GetText();
//	int newSelectedID = GetVesselIDFromText(txt);
//	std::shared_ptr<UnitEntity> targetVessel = GameManager::CurrentInstanceManager()->GetIntegrator()->GetUnitEntityFromID(newSelectedID);
//
//	auto targetSelectLink = targetVessel->getLink(enLinkType::LINKTARGETSELECTED);
//	if (targetSelectLink != nullptr)
//	{
//		targetSelectLink->AddLinkToBindingList(item);
//		item->AddLinkToBindingList(targetSelectLink);
//	}
//	return item;
//}