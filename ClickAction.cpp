#include"ClickAction.h"


ClickAction::ClickAction()
{
	pType = ClickActionType::daUnknown;
}

ClickAction::ClickAction(ClickActionType type)
{
	pType = type;
}

ClickAction ClickAction::GetChangeLocation(string location)
{
	ClickAction result = ClickAction(ClickActionType::daChangeLocation);
	result.SetDataString(location);
	return result;
}

ClickAction ClickAction::GetOpenStore(string location)
{
	ClickAction result = ClickAction(ClickActionType::daOpenStore);
	result.SetDataString(location);
	return result;
}

ClickAction ClickAction::GetOpenShipSelect(string screenName)
{
	ClickAction result = ClickAction(ClickActionType::daSelectShip);
	result.SetDataString(screenName);
	return result;
}

ClickAction ClickAction::GetDialogue(string name)
{
	ClickAction result = ClickAction(ClickActionType::daDialogue);
	result.SetDataString(name);
	return result;
}

ClickAction ClickAction::GetOpenTablet(string tabletName)
{
	ClickAction result = ClickAction(ClickActionType::daOpenTablet);
	result.SetDataString(tabletName);
	return result;
}