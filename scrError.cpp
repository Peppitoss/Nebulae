#include"scrError.h"
#include"BackgroundCollection.h"
#include"Link.h"
#include"GameManager.h"

scrError::scrError(enErrorScreenType type, string ErrorMessage) : SingleButtonTablet(256 + 128,256, Coord(0), ImportImage_PlayerMenuScreen, GetButtonText(type))
{
	pType = type;
	Initialize(ErrorMessage);
}

void scrError::Initialize(string ErrorMessage )
{

	auto msg = std::shared_ptr<Link>(new Link(Text(ErrorMessage), standardLinkFont.c_str(), 12, this->GetPosition() + Coord(0, 64), 128, 256 + 128, ColorRGB::White(), enTextPosition::centerTxt, true));
	this->Control()->AddWidget(msg);
	msg->SetWidgetOff();

	this->AddBackgroundLayer(ImportImage_PlayerMenuBorder, 255);
}

string scrError::GetButtonText(enErrorScreenType type)
{
	switch (type)
	{
	case errAssert:
		return AbortTxt();
	case errException:
		return ExceptionTxt();
	}
}

void scrError::HandleActiveWidget(SDL_Event& e, Widget* widget)
{
	std::string txt = widget->ID();

	if (txt == AbortTxt())
	{
		GameManager::QuitGame();
	}
	else if (txt == ExceptionTxt())
	{
		GameManager::Screens.closeErrorScreen();
	}

}