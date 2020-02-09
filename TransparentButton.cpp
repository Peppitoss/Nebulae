#include"TransparentButton.h"

TransparentButton::TransparentButton() : Link()
{

}

TransparentButton::TransparentButton( const char* text, Coord position, int h, int w, std::string callbackname) : Link(Text(text), 12, position, h, w, ColorRGB::White(), ColorRGB::Grey(200,50))
{
	Initialize();
	this->SetCallbackFunctionName(callbackname);
}

//enLinkType TransparentButton::GetType()
//{
//	return enLinkType::LINKHUDBUTTON;
//}

void TransparentButton::UseZoomModifier(bool value)
{
	this->pUseZoomModifiers = value;
}

bool TransparentButton::isInside(int mouseX, int mouseY)
{
	return IsInside(mouseX, mouseY, this->pUseZoomModifiers, false);
}

void TransparentButton::Initialize()
{
	//this->SetBackgroundColor( ColorRGB(100,100,100,100) );
	//this->SetSelectionColor( ColorRGB(255,255,255,200) );
	this->Refresh();
	this->pUseZoomModifiers = true;
	this->SetState( WIDGET_STATE::WS_OFF );

}
