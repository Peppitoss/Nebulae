#include"scrTransaction.h"
#include"BackgroundCollection.h"
#include"Link.h"
#include"GameManager.h"
#include"ScreenButton.h"

scrTransaction::scrTransaction(enTransactionType type, string ErrorMessage) : SingleButtonTablet(256 + 128, 256, Coord(0), ImportImage_PlayerMenuScreen, GetButtonText(type))
{
	pType = type;
	Initialize(ErrorMessage);
}

void scrTransaction::Initialize(string ErrorMessage)
{

	auto msg = std::shared_ptr<Link>(new Link(Text(ErrorMessage), standardLinkFont.c_str(), 12, this->GetPosition() + Coord(0, 64), 128, 256 + 128, ColorRGB::White(), enTextPosition::centerTxt, true));
	this->Control()->AddWidget(msg);
	msg->SetWidgetOff();

	if (pType == enTransactionType::trRevert)
	{
		Coord pos = this->GetPosition();
		Coord bottomRight = pos + Coord(this->Width(), this->Height());

		//int bHeight = 0;
		//int bWidth = 220;
		Coord btnSize = Coord(128, STANDARD_BUTTON_HEIGHT);
		Widget* close = new ScreenButton(Coord(bottomRight) - btnSize.xadd(128), btnSize.x, btnSize.y, BtnText("Cancel"));
		this->Control()->AddWidget(close);
	}

	


	this->AddBackgroundLayer(ImportImage_PlayerMenuBorder, 255);
}

string scrTransaction::GetButtonText(enTransactionType type)
{
	switch (type)
	{
	case trRevert:
		return RevertText();
	case trFailed:
		return AbortTxt();
	}
}

void scrTransaction::HandleActiveWidget(SDL_Event& e, Widget* widget)
{
	std::string txt = widget->ID();

	if (txt == AbortTxt())
	{
		GameManager::Screens.closeTransactionScreen();
	}
	else if (txt == RevertText())
	{
		GameManager::revertTransaction();
		this->CloseScreen();
		GameManager::Screens.closeStore();
	}
	else if (txt == Text("Cancel").GetCurrentValue())
	{
		GameManager::Screens.closeOpenTablet();
	}

}