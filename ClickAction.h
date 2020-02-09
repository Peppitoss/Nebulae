#pragma once
#include<string>

using std::string;

enum ClickActionType
{
	daUnknown = 0,
	daOpenScreen = 1,
	daOpenTablet = 2,
	daSelectShip = 3,
	daOpenStore = 4,
	daChangeLocation = 5,
	daDialogue = 6
};

class ClickAction
{

public:
	ClickAction();
	ClickAction(ClickActionType type);

	static ClickAction GetChangeLocation(string location);
	static ClickAction GetOpenStore(string storeName);
	static ClickAction GetOpenShipSelect(string screenName);
	static ClickAction GetOpenTablet(string tabletName);
	static ClickAction GetDialogue(string name);

	string GetDataString() const { return pData; }
	void SetDataString(string set) { this->pData = set; }

	string GetLocation() const  { return pData; }
	string GetStoreName() const { return pData; }
	string GetShipName() const { return pData; }
	string GetTabledName() const { return this->pData; }
	string GetDialogueName() const { return this->pData; }

	ClickActionType GetType() const { return this->pType; }

private:

	ClickActionType pType;
	string pData;

};