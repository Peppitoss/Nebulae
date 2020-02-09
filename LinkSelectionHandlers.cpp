#include"LinkSelectionHandlers.h"
#include"InstanceManager.h"


//make a class spesific callback function, to trigger player move to button center position, trigger store algorithm for player, when player close to point open store window
static void PlayerDockToLink( Widget* button)
{
	auto state = button->GetState();
	switch (button->GetState())
	{
	case WS_HOVER:
		break;
	case WS_ACTIVE:
		break;
	case WS_ON:
		ClickAction da = ClickAction::GetOpenStore("teststore");
		InstanceManager::GetCurrentPlayer()->DockToPoint(button->GetWidgetCenterPosition().yadd(9), geometry::PI_div2, da);
		break;
	}
}

//static void PlayerSelectTarget( Widget* button)
//{
//	InstanceManager::GetIntegrator()->RemoveTargetSelectionOfUnit(button);
//}

//static void LoadCommand(SDL_Event& e, Widget* button)
//{
//	if (button->GetState() == WS_ON)
//	{
//		ClickAction da = ClickAction::GetChangeLocation("command");
//		InstanceManager::getPlayer()->DockToPoint(button->GetWidgetCenterPosition().yadd(9), std::_Pi_div2, da);
//	}
//}
//static void LoadBar(SDL_Event& e, Widget* button)
//{
//	if (button->GetState() == WS_ON)
//	{
//		ClickAction da = ClickAction::GetChangeLocation("bar");
//		InstanceManager::getPlayer()->DockToPoint(button->GetWidgetCenterPosition().yadd(9), std::_Pi_div2, da);
//	}
//}

extern void LoadShop( Widget* button)
{
	if (button->GetState() == WS_ON)
	{
		InstanceManager::GetCurrentPlayer()->DockToPoint(button->GetWidgetCenterPosition().yadd(9), geometry::PI_div2, button->GetClickAction());
	}
}

extern void LoadLocation( Widget* button)
{
	if (button->GetState() == WS_ON)
	{
		InstanceManager::GetCurrentPlayer()->DockToPoint(button->GetWidgetCenterPosition().yadd(9), geometry::PI_div2, button->GetClickAction());
	}
}

extern void OpenTablet( Widget* button)
{
	if (button->GetState() == WS_ON)
	{
		InstanceManager::GetCurrentPlayer()->DockToPoint(button->GetWidgetCenterPosition().yadd(9), geometry::PI_div2, button->GetClickAction());
		//InstanceManager::getPlayerWalker()->DockToPoint(button->GetWidgetCenterPosition().yadd(9), std::_Pi_div2, button->GetClickAction());
	}
}

extern void StartDialogue( Widget* button)
{
	if (button->GetState() == WS_ON)
	{
		InstanceManager::GetCurrentPlayer()->DockToPoint(button->GetWidgetCenterPosition().yadd(9), geometry::PI_div2, button->GetClickAction());
	}
}

bool TypeIsSimpleTablet(string inputstr)
{
	if (inputstr.size() > 2)
		return inputstr[0] == 't' && inputstr[1] == '_';
	return false;
}
bool TypeIsShop(string inputstr)
{
	if (inputstr.size() > 2)
		return inputstr[0] == 's' && inputstr[1] == '_';
	return false;
}

bool TypeIsLocation(string inputstr)
{
	if (inputstr.size() > 2)
		return inputstr[0] == 'l' && inputstr[1] == '_';
	return false;
}

bool TypeIsShipSelect(string inputstr)
{
	if(inputstr.size() > 3)
		return inputstr[0] == 's' && inputstr[1] == 'p' && inputstr[2] == '_';
	return false;
}

bool TypeIsDialogue(string inputstr)
{
	if (inputstr.size() > 2)
		return inputstr[0] == 'd' && inputstr[1] == '_';
	return false;
}

WidgetCallback GetLinkFunctionType(string type)
{
	if (type == "dock" || type == "DOCK" || type == "Dock")
 		return PlayerDockToLink;
	//if (type == "target" || type == "TARGET" || type == "Target")
	//	return PlayerSelectTarget;
	if (TypeIsLocation(type))
		return LoadLocation;
	if (TypeIsSimpleTablet(type))
		return OpenTablet;
	else if (TypeIsShipSelect(type))
		return OpenTablet;
	if (TypeIsDialogue(type))
		return StartDialogue;
	if (TypeIsShop(type))
		return LoadShop;

	throw new std::exception("Error no function found for this type");
}

ClickAction GetClickActionFromType(string type)
{
	if (type == "dock" || type == "DOCK" || type == "Dock")
		return ClickAction();
	else if (type == "target" || type == "TARGET" || type == "Target")
		return ClickAction();
	if (TypeIsLocation(type))
		return  ClickAction::GetChangeLocation(ReaderLocationLinkName(type));
	if (TypeIsSimpleTablet(type))
		return  ClickAction::GetOpenTablet(ReaderLocationLinkName(type));
	if (TypeIsShipSelect(type))
		return  ClickAction::GetOpenShipSelect(ReaderScreenSelectName(type));
	if (TypeIsDialogue(type))
		return  ClickAction::GetDialogue(ReaderDialogueLinkName(type));
	if (TypeIsShop(type))
		return ClickAction::GetOpenStore(ReaderShopSelectName(type));

	throw new std::exception("Error no function found for this type");
}

string ReaderScreenSelectName(string location)
{
	string screenSelect = "";
	for (int i = 3; i < location.size(); i++)
		screenSelect += location[i];
	return screenSelect;
}

string ReaderLocationLinkName(string location)
{
	string locationName = "";
	for (int i = 2; i < location.size(); i++)
		locationName += location[i];
	return locationName;
}

string ReaderShopSelectName(string location)
{
	string shopName = "";
	for (int i = 2; i < location.size(); i++)
		shopName += location[i];
	return shopName;
}

string ReaderDialogueLinkName(string location)
{
	string dialogueName = "";
	for (int i = 2; i < location.size(); i++)
		dialogueName += location[i];
	return dialogueName;
}


string ReaderTabletLinkName(string tablet)
{
	string locationName = "";
	for (int i = 2; i < tablet.size(); i++)
		locationName += tablet[i];
	return locationName;
}

//WidgetCallback ReaderLocationLink(string location)
//{
//	string locationName = ReaderLocationLinkName(location);
//	WidgetCallback result = GetLocationCallback(locationName);
//	if (result == nullptr)
//		throw new std::exception("Error no function found for this type");
//	else
//		return result;
//}
//
//WidgetCallback GetLocationCallback(string locationname)
//{
//	return nullptr;
//}
//
//WidgetCallback GetDialogueCallback(string dialogue)
//{
//	return nullptr;
//}
