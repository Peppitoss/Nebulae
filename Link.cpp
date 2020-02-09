#include"InstanceManager.h"
#include"SelectTargetButton.h"
#include"HotKeySelection.h"
#include"LinkSelectionHandlers.h"
#include"GameManager.h"
#include"GameCamera.h"
#include"Debug.h"
//#include<functional>
#include <SDL_ttf.h>

using namespace std::placeholders;




LinkAttachment::LinkAttachment(const LinkAttachment& other)
{
	link = other.link;
	RelativePosition = other.RelativePosition;
}

LinkAttachment::LinkAttachment(LinkAttachment&& other)
{
	link = std::move(other.link);
	RelativePosition = std::move(other.RelativePosition);
}

LinkAttachment& LinkAttachment::operator=(const LinkAttachment& other)
{   
	link = other.link;
	RelativePosition = other.RelativePosition;
	return *this;
}

LinkAttachment& LinkAttachment::operator=(const LinkAttachment&& other)
{   
	link = std::move(other.link);
	RelativePosition = std::move(other.RelativePosition);
	return *this;
}

std::shared_ptr<SurfaceButton> LinkAttachment::generateLink( Coord relativePosition, Coord globalPosition, std::string text, enLinkType type, string callbackname  )
{
	this->RelativePosition = relativePosition;
	int width = primitives::GetTextDimensions(primitives::GetLargestString(primitives::SplitNewLine(text)), 12, standardLinkFont).x;
	this->clearMemory();

	switch( type )
	{
	case( enLinkType::LINKSTANDARD ) :
		this->link = std::shared_ptr<Link>(new Link(Text(text), 14, globalPosition, 18, width ));
		break;
	case(enLinkType::LINKHUDBUTTON) : 
		this->link = std::shared_ptr<Link>(new TransparentButton(text.c_str(), globalPosition, 18, width, callbackname));
		this->link->SetClickAction(GetClickActionFromType(callbackname));
		this->link->OnWidgetClicked = GetLinkFunctionType(callbackname);
		break;
	case(enLinkType::LINKTARGETSELECTED):
		this->link = std::shared_ptr<ScreenButton>(new ScreenButton(globalPosition, StyleFactory::SelectTargetStyle(64, 64), "selecttarget"));
		this->link->SetUseToggleClick(true);
		this->link->OnWidgetClicked = [](Widget* button) {
			InstanceManager::GetIntegrator()->RemoveTargetSelectionOfUnit(button);
		};
		break;
	case(enLinkType::LINKDIALOG):
		this->link = std::shared_ptr<Link>(new TransparentButton(text.c_str(), globalPosition, 18, width,callbackname));
		this->link->SetZoomingModifier(GameCamera::GetZoomDrawPositionModifier(enZoomLevel::ZoomCLOSE));
		this->link->OnWidgetClicked = GetLinkFunctionType(callbackname);
		//this->link->SetBorderColor(enBorderFlags::BorderOnHover, ColorRGB(255, 193, 73));
		//this->link->SetBorderFlag(enBorderFlags::BorderOnHover | enBorderFlags::BorderOnSelect);
		//this->link->SetBorderStyle(enLinkBorderStyle::WholeBorder);
		this->link->SetClickAction(GetClickActionFromType(callbackname));
		break;
	case(enLinkType::LINKHOTKEYSELECT):
		this->link = std::shared_ptr<Link>(new HotKeySelection(enHKDisplay::HKSingleLetter, text, globalPosition));
		break;
	case(enLinkType::LINKDOCKBUTTON):
		this->link = std::shared_ptr<Link>(new TransparentButton(text.c_str(), globalPosition, 18, width, callbackname));
		this->link->SetClickAction(GetClickActionFromType(callbackname));
		this->link->OnWidgetClicked = GetLinkFunctionType(callbackname);
		break;
	}
	//if( autosize )
	//{
	//	this->link->Refresh();
	//}
	return this->link;
}
//
/////
//// TODO: Boundary box of widget must match style,  see autosize
//// TODO: style with only ONE text style (to avoid multiple surface refreshes, when it changes!!!)
//// TODO: handle link changes in code
//
/////
//
//
std::shared_ptr<SurfaceButton> LinkAttachment::generateLink( Coord relativePosition, Coord globalPosition, std::string text, enLinkType type )
{
	return generateLink(relativePosition, globalPosition, text, type, "");
}


void LinkAttachment::Resize( Coord size )
{
	if (link)
		this->link->Resize( size ); //std::shared_ptr<Link>(new Link(this->link->GetText().c_str(), 14, this->link->GetPosition(), size.y, size.x ));
	//this->link->Autosize = false;
	//this->link->Refresh();

}

void LinkAttachment::ChangeText( std::string text )
{
	if (link)
		this->link->ChangeText( text.c_str() );
}

void LinkAttachment::setRelativePosition(const Coord& set)  //OPS doesn't set a new position for link, in that case use setAttachmentposition :)
{
	RelativePosition = set;
}; 

void LinkAttachment::setAttachmentposition(const Coord& set)
{
	if(link)
		link->SetPosition(set + RelativePosition); 
};


void LinkAttachment::clearMemory()
{
	this->link.reset();
	//if(link != NULL) 
	//{ 
	//	delete link; 
	//}
}

Link::Link() : SurfaceButton()
{
	//Autosize = false;
}

Link::Link(Text text, TextSize size, Coord pos, int h, int w, ColorRGB& textColor, ColorRGB& backColor) :
	SurfaceButton( text.GetCurrentValue(), pos.x, pos.y, StyleFactory::LinkStyle(w,h, standardLinkFont,size, text, textColor) )
{
	Initialize();
}

Link::Link(Text text, TextSize size, Coord pos, int h, int w ) :
	SurfaceButton(text.GetCurrentValue(), pos.x, pos.y, StyleFactory::LinkStyle(w, h, standardLinkFont, size, text))
{
	Initialize();
}

Link::Link(Text text, const char* fontName, TextSize size, Coord pos, int h, int w ) :
	SurfaceButton(text.GetCurrentValue(), pos.x, pos.y, StyleFactory::LinkStyle(w, h, fontName, size, text))
{
	Initialize();
}

Link::Link(Text text, const char* fontName, TextSize size, Coord pos, int h, int w, const ColorRGB& textColor, enTextPosition enPos, bool OnlyOneState) :
	SurfaceButton(text.GetCurrentValue(), pos.x, pos.y, StyleFactory::TextBoxStyle(w, h, fontName, size, text, textColor, enPos, OnlyOneState))
{
	Initialize();
}

Link::Link( Link& other) : SurfaceButton( other )
{
	//this->Autosize = other.Autosize;
	this->pSelected = other.pSelected;
	this->pLinkNumericState = other.pLinkNumericState;
}
Link::Link( Link&& other) : SurfaceButton( other )
{
	//this->Autosize = std::move(other.Autosize);
	this->pSelected = std::move(other.pSelected);
	this->pLinkNumericState = std::move(other.pLinkNumericState);
}

Link& Link::operator=(Link& src)
{
	SurfaceButton::operator=(src);
	//this->Autosize = src.Autosize;
	this->pSelected = src.pSelected;
	this->pLinkNumericState = src.pLinkNumericState;
	return *this;
}

Link& Link::operator=(Link&& src)
{
	SurfaceButton::operator=(src);
	//this->Autosize = std::move(src.Autosize);
	this->pSelected = std::move(src.pSelected);
	this->pLinkNumericState = std::move(src.pLinkNumericState);
	return *this;
}

void Link::Deactivate(SDL_Event& e)
{
	if (mState != DeactiveState())
	{
		this->SetSelected(false);
	}
};

void Link::Activate(SDL_Event& e)
{
	if (mState != ActiveState())
	{
		this->SetSelected(true);
	}
}

void Link::SetState(WIDGET_STATE state)
{
	if (state == this->ActiveState())
		this->SetSelected(true);
	else if (state == this->DeactiveState())
		this->SetSelected(false);
	else
		Widget::SetState(state);
}

void Link::SetSelected( bool value )
{
	if( this->pSelected != value ) //|| this->GetState() == WIDGET_STATE::WS_HOVER)
	{
		if (value)
		{
			Widget::SetState(ActiveState());
		}
		else
		{
			Widget::SetState(DeactiveState());
		}
		this->pSelected = value;
	}
}

bool Link::CheckIfLinkIsSelected()
{
	if (this->pSelected)
	{
		this->SetState(ActiveState());
		return true;
	}
	return false;
}

void Link::UpdateState(SDL_Event & event)
{
	if (CheckIfLinkIsSelected())
	{
		HandleLinkIsSelected(event);
		return;
	}
	
	UpdateStateAddition(event);
}

void Link::HandleLinkIsSelected(SDL_Event & event)
{
	return;
}



//enLinkType Link::GetType()
//{
//	return enLinkType::LINKSTANDARD;
//}

void Link::UpdateStateAddition(SDL_Event & event)
{
	Widget::UpdateState(event);
}

//void Link::Refresh()
//{
//	Refresh(false);
//}


//void Link::Refresh(bool onlyText)
//{
//	mTexture.reset();
//	mSurface.reset();
//	if(!onlyText)
//		mBackground.reset();
//
//	auto previousBoundary = mBoundaryBox;
//	bool boundaryChanged = false;
//	//auto previousClipping = mClipping;
//
//	if (this->autosize)
//	{
//		this->AutoResize();
//		boundaryChanged = !primitives::RectAEqualsToB(previousBoundary, mBoundaryBox);
//	}
//	
//	mSurface = std::shared_ptr<SDL_Surface>(sdlWrapper.CreateSurface(mBoundaryBox.w, WS_COUNT * mBoundaryBox.h), SDL_FreeSurface);
//	SDL_SetColorKey( mSurface.get(), SDL_TRUE, Wrapper::GetColor(mSurface.get(),C_DEFAULT));
//	if( this->GetBackgroundColor().alpha() > 0)
//		if (!onlyText || boundaryChanged)
//			mBackground = this->CreateButtonBackground(mBoundaryBox);
//	//DrawText();
//}

void Link::Initialize()
{
	//this->SetBackgroundColor(colorFromEnum(EnumColor::INVISIBLE));
	//this->pSelectionColor = ColorRGB(100,100,100,100);
	//Autosize = false;
	//centerTextOnBoundary = false;
	pSelected = false;
	//pNewLineDistance = 0;
	//centerTextHorizontally = false;
	Refresh();
	//pTextMargin = 6;
	pBorderStyle = enLinkBorderStyle::NoBorder;
	pBorderFlag = enBorderFlags::BorderFlagOff;
	//this->pBorderColor = std::map<enBorderFlags, ColorRGB>(); 
	//this->pBorderColor[enBorderFlags::BorderOnHover] = ColorRGB::White();
	//this->pBorderColor[enBorderFlags::BorderOnSelect] = ColorRGB::White();
	//this->pBorderColor[enBorderFlags::BorderOnStandard] = ColorRGB::White();
	this->SetState( WIDGET_STATE::WS_OFF );
	this->pCallbackFunctionName = "";
	
}

//void Link::SetText( enumLinkText text  ) //make generic
//{
//	if( this->enText == text )
//		return;
//
//	this->enText = text;
//
//	switch( text )
//	{
//	case(enumLinkText::CLICKME):
//		RenewText(" ");
//		break;
//	case(enumLinkText::DOCKINGINITIATED) :
//		RenewText("Docking initiated");
//		break;
//	case(enumLinkText::DOCKEDDONE) :
//		RenewText("Docked");
//		break;
//	};
//	
//}

void Link::FadeText(double Opacity)
{
	auto col = this->GetTextColor();
	col.newColor(col.red(), col.green(), col.blue(), 255 * Opacity);
	RenewText(col.ToSDL_Color());
}

void Link::RenewText(std::string text)
{
	if( text.compare( this->GetText() ) != 0 )
	{
		this->ChangeText(text);
		RefreshText();
	}
}

void Link::RenewText(std::string text, SDL_Color newColor)
{
	int t = 0;
	if (text.compare(this->GetText()) != 0)
	{
		this->ChangeText(text);
		t++;
	}

	if (this->GetTextColor() != newColor)
	{
		this->ChangeText(ColorRGB(newColor));
		t++;
	}
	if(t == 2)
		RefreshText();
}

void Link::RenewText(SDL_Color newColor)
{
	if (this->GetTextColor() != newColor)
	{
		this->ChangeText(ColorRGB(newColor));
		RefreshText();
	}
}

//Coord Link::GetTextDimensions( std::string text )
//{
//	SDL_Rect dimension = sdlWrapper.GetTextSize( this->mFontSize, this->fontName, text ,TTF_STYLE_BOLD );
//	return Coord( dimension.w, dimension.h );
//}
SDL_Texture* Link::generateTexture(SDL_Renderer & renderer, double alpha)
{
	SDL_SetTextureAlphaMod(Widget::generateTexture(renderer), alpha);
	Debug::SDLErrorCheck();
	return this->mTexture.get();
}

//void Link::AutoResize()
//{
//	this->mBoundaryBox.h = 0;
//	this->mBoundaryBox.w = 0;
//	this->mClipping.h = 0;
//	this->mClipping.w = 0;
//
// 	std::list<std::string> newLines = primitives::SplitNewLine(GetFullText());
//
//	for( auto it = newLines.begin(); it != newLines.end(); ++it )
//	{
//		Coord dimension = GetTextDimensions(GetFullText()).xadd(pTextMargin);
//		this->mBoundaryBox.h += dimension.y + this->GetNewLineDistance();
//		this->mBoundaryBox.w = std::fmax(this->mBoundaryBox.w, dimension.x);
//		this->mClipping.h += dimension.y + this->GetNewLineDistance();
//		this->mClipping.w = std::fmax(this->mClipping.w, dimension.x);
//	}
//}

Coord Link::TextDrawPosition()
{
	return Coord(0, 0);
}

//void Link::DrawText()
//{
//	/*int width = mBoundaryBox.w;
//	int height = mBoundaryBox.h;
//
//	SDL_Rect rect;
//	SDL_Rect clip;
//
//	SDL_BlitSurface(mBackground.get(), NULL, mSurface.get(), NULL);
//
//	std::list<std::string> newLines = primitives::SplitNewLine(GetFullText());
//	
//	int newLineCounter = 0;
//	int lineHeight = height/newLines.size();
//	int x_shift = 0, y_shift = 0;
//	
//	if( centerTextOnBoundary )
//	{
//		x_shift = this->GetTextDimensions( GetLongestString( newLines ) ).x / 2;
//		y_shift = newLines.size()*lineHeight / 2;
//	}
//	else if(centerTextHorizontally)
//	{
//		x_shift = this->TextDrawPosition().x - this->GetTextDimensions(GetLongestString(newLines)).x / 2;
//		y_shift = this->TextDrawPosition().y;
//	}
//	else
//	{
//		x_shift = this->TextDrawPosition().x;
//		y_shift = this->TextDrawPosition().y;
//	}
//
//	SDL_Surface * textSurface = sdlWrapper.CreateSurface(width, height, sdlWrapper.GetSDLColor(COLOUR::C_DEFAULT) );
//
//	for( auto it = newLines.begin(); it != newLines.end(); ++it )
//	{
//		SDL_Surface * textlineSurface = sdlWrapper.CreateTextSurface((*it).c_str(), mFontSize,fontName, mTextColor, TTF_STYLE_BOLD );
//		SDL_BlitSurface(textlineSurface, NULL, textSurface, &sdlWrapper.CreateRect(x_shift, y_shift + newLineCounter*lineHeight + pNewLineDistance, width, lineHeight ));
//		newLineCounter++;
//		SDL_FreeSurface(textlineSurface);
//	}
//
//	int yPosText = (height - textSurface->h)/2;
//	int xPosText = (width - textSurface->w)/2;
//	if(xPosText < 2) xPosText = 2;*/
//
//	for(int i = 0; i < WS_COUNT; i++)
//	{
//		rect = sdlWrapper.CreateRect(xPosText, yPosText, width, height);
//		clip = sdlWrapper.CreateRect(xPosText, yPosText + i*height, width, height );
//		SDL_BlitSurface(textSurface, NULL, mSurface.get(), &clip);
//	}
//	SDL_FreeSurface(textSurface);
//
//	if(mTexture) SDL_DestroyTexture(mTexture.get());
//}

//std::string Link::GetLongestString( const std::list<std::string>& strList)
//{
//	int longest = 0;
//	std::string result = "";
//	for( auto it = strList.begin(); it != strList.end(); ++it )
//	{
//		int length = this->GetTextDimensions(*it).x;
//		if(length > longest )
//		{
//			longest = length;
//			result = *it;
//		}
//	}
//	return result;
//}

Link::~Link()
{

}
//
//SDL_Surface* Link::GetStandardSurface(SDL_Rect rect)
//{
//	SDL_Surface* buttonFill = sdlWrapper.CreateSurface(rect.w, rect.h);
//	SDL_PixelFormat * pixelFormat = buttonFill->format;
//	SDL_FillRect(buttonFill, NULL, SDL_MapRGBA(pixelFormat, this->GetBackgroundColor().red(), this->GetBackgroundColor().green(),this->GetBackgroundColor().blue(), this->GetBackgroundColor().alpha() ));
//	if(this->pBorderFlag & enBorderFlags::BorderOnStandard)
//		return AddBorder(buttonFill, enBorderFlags::BorderOnStandard, rect);
//	return buttonFill;
//	
//}
//
//SDL_Surface* Link::GetSelectionSurface(SDL_Rect rect)
//{
//	SDL_Surface* buttonFill = sdlWrapper.CreateSurface(rect.w, rect.h );
//	SDL_PixelFormat * pixelFormat = buttonFill->format;
//	SDL_FillRect(buttonFill, NULL, SDL_MapRGBA(pixelFormat, this->pSelectionColor.red(),this->pSelectionColor.green(),this->pSelectionColor.blue(),this->pSelectionColor.alpha() ));
//	if (this->pBorderFlag & enBorderFlags::BorderOnSelect)
//		return AddBorder(buttonFill, enBorderFlags::BorderOnSelect, rect);
//	return buttonFill;
//}
//
//SDL_Surface* Link::GetHoverSurface(SDL_Rect rect)
//{
//	auto result = GetSelectionSurface(rect);
//	if (this->pBorderFlag & enBorderFlags::BorderOnHover)
//		return AddBorder(result, enBorderFlags::BorderOnHover, rect);
//	return result;
//}
//
//SDL_Surface* Link::AddBorder(SDL_Surface* surface, enBorderFlags flag, SDL_Rect& rect)
//{
//	SDL_Surface* buttonFill;
//	switch (this->pBorderStyle)
//	{
//	case(enLinkBorderStyle::EdgeBorders):
//		break;
//	case(enLinkBorderStyle::WholeBorder):
//		buttonFill = sdlWrapper.CreateSquareBorder(GetColorFromBorder(flag).ToSDL_Color(), rect, 2);
//		SDL_BlitSurface(buttonFill, NULL, surface, NULL);
//		break;
//	case(enLinkBorderStyle::NoBorder):
//		break;
//	}
//	return surface;
//}
//
//ColorRGB Link::GetColorFromBorder(enBorderFlags flag)
//{
//	for (auto it = pBorderColor.begin(); it != pBorderColor.end(); ++it)
//		if ((*it).first & flag)
//			return (*it).second;
//	return ColorRGB();
//}
//
//SDL_Surface* Link::Get_WS_OFF_Surface(SDL_Rect rect)
//{
//	return GetStandardSurface(rect);
//}
//
//SDL_Surface* Link::Get_WS_HOVER_Surface(SDL_Rect rect)
//{
//	return GetHoverSurface(rect);
//}
//
//SDL_Surface* Link::Get_WS_ACTIVE_Surface(SDL_Rect rect)
//{
//	return GetSelectionSurface(rect);
//}
//
//SDL_Surface* Link::Get_WS_ON_Surface(SDL_Rect rect)
//{
//	return GetSelectionSurface(rect);
//}
//
//SDL_Surface* Link::Get_WS_NOT_ENABLED_Surface(SDL_Rect rect)
//{
//	return GetStandardSurface(rect);
//}
//
//SDL_Surface* Link::Get_WS_HIDDEN_Surface(SDL_Rect rect)
//{
//	return GetStandardSurface(rect);
//}

bool Link::IsInside(int mouseX, int mouseY, bool includeZoomModifier, bool IncludeOrigoPosition)
{
	if (includeZoomModifier)
	{
		Coord zoomedMouse = GameManager::GetMousePositionFromZoomLevel(Coord(mouseX, mouseY));
		return Widget::isInside(zoomedMouse.x - mOrigoPosition.x*IncludeOrigoPosition, zoomedMouse.y - mOrigoPosition.y*IncludeOrigoPosition);
	}
	return Widget::isInside(mouseX, mouseY);
}