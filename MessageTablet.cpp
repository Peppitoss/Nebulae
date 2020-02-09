#include"MessageTablet.h"
#include"BackgroundCollection.h"
#include"Link.h"
#include"GameManager.h"

MessageTablet::MessageTablet(MessageTabletID id,  string message, string buttonText, bool unpauseOnExit) : SingleButtonTablet(256 + 128, 256, Coord(0), ImportImage_PlayerMenuScreen, buttonText)
{
	this->pID = id;
	this->pUnpauseOnExit = unpauseOnExit;
	pButtonText = buttonText;
	Initialize(message);
}

void MessageTablet::Initialize(string messagetext)
{
	auto msg = std::shared_ptr<Link>(new Link(Text(messagetext), standardLinkFont.c_str(), 12, this->GetPosition() + Coord(0, 64), 128, 256 + 128, ColorRGB::White(), enTextPosition::centerTxt, true));
	msg->Refresh();
	
	this->Control()->AddWidget(msg);
	msg->SetWidgetOff();

	this->AddBackgroundLayer(ImportImage_PlayerMenuBorder, 255);
}

void MessageTablet::HandleActiveWidget(SDL_Event& e, Widget* widget)
{
	std::string txt = widget->ID();

	if (txt == pButtonText)
	{
		GameManager::Screens.closeMessageScreen(this->pID, pUnpauseOnExit);
	}

}
