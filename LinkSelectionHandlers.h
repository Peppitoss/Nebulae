#pragma once
#include"Widget.h"
#include<SDL.h>
 

typedef void(*WidgetCallback)(Widget*);
WidgetCallback GetLinkFunctionType(string type);
//WidgetCallback ReaderLocationLink(string location);
//WidgetCallback GetLocationCallback(string locationname);
//WidgetCallback GetDialogueCallback(string dialogue);
ClickAction GetClickActionFromType(string type);
string ReaderLocationLinkName(string location);
string ReaderDialogueLinkName(string location);
string ReaderTabletLinkName(string location);
string ReaderScreenSelectName(string location);
string ReaderShopSelectName(string location);

bool TypeIsDialogue(string inputstr);
bool TypeIsShipSelect(string inputstr);
bool TypeIsSimpleTablet(string inputstr);
bool TypeIsShop(string inputstr);
bool TypeIsLocation(string inputstr);


extern void PlayerDockToLink( Widget* button);
//extern void PlayerSelectTarget( Widget* button);
extern void LoadLocation( Widget* button);
extern void LoadShop( Widget* button);
extern void OpenTablet( Widget* button);
extern void StartDialogue( Widget* button);


