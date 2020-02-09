#include "Widget.h"
#include"Debug.h"
#include<iostream>
const WIDGET_MODE defaultLayer = WL_INTERACTIVE;
const SDL_Rect emptyRect = sdlWrapper.CreateRect(0,0,0,0); 

Widget::Widget() :mState(WS_NOT_ENABLED),mMode(defaultLayer)
{
	pZoomModifier = 1;
	SetClipping(emptyRect);
	SetBoundaryBox(emptyRect);
	SetOrigoPosition( Coord(0,0) );
	mOffset = Coord(0);
	pUseToggleClick = false;
	pWidgetInteriorClicked = false;
	pStateDependOnBinding = false;
}

Widget::Widget(string id, SDL_Surface * surface_arg, SDL_Rect & rect) :mState(WS_OFF),mMode(defaultLayer)
{
	pZoomModifier = 1;
	mSurface = std::shared_ptr<SDL_Surface>(surface_arg, SDL_FreeSurface);
	SetClipping(emptyRect);
	SetBoundaryBox(rect);

	mClipping.w = rect.w;
	mClipping.h = rect.h;
	SetOrigoPosition( Coord(0,0) );
	mOffset = Coord(0);
	pID = id;
	pUseToggleClick = false;
	pWidgetInteriorClicked = false;
	pStateDependOnBinding = false;
}
Widget::Widget(string id, SDL_Surface * surface_arg, int x, int y, int w, int h ) :mState(WS_OFF), mMode(defaultLayer)
{
	pZoomModifier = 1;
	mSurface = std::shared_ptr<SDL_Surface>(surface_arg, SDL_FreeSurface);
	SDL_Rect rect = { x, y, w, h };
	SetClipping(emptyRect);
	SetBoundaryBox(rect);

	mClipping.w = rect.w;
	mClipping.h = rect.h;
	SetOrigoPosition( Coord(0,0) );
	mOffset = Coord(0);
	pID = id;
	pUseToggleClick = false;
	pWidgetInteriorClicked = false;
	pStateDependOnBinding = false;
}
// Move semantics and copy constructors
Widget::Widget(const Widget& src)
{
	this->mState	   = src.mState;
	this->mMode		   = src.mMode;
	this->mTexture     = src.mTexture;
	this->mSurface     = src.mSurface;
	this->mClipping    = src.mClipping;
    this->mBoundaryBox = src.mBoundaryBox;
	this->mOrigoPosition = src.mOrigoPosition;
	this->pZoomModifier = src.pZoomModifier;
	//this->mTextColor   = src.mTextColor;
	//this->mText        = src.mText;
	this->pUseToggleClick = src.pUseToggleClick;
	this->OnWidgetClicked = src.OnWidgetClicked;
	this->OnWidgetDisabled = src.OnWidgetDisabled;
	this->OnWidgetHovered = src.OnWidgetHovered;
	this->OnWidgetSelected = src.OnWidgetSelected;
	this->pWidgetInteriorClicked = src.pWidgetInteriorClicked;
	this->pStateDependOnBinding = src.pStateDependOnBinding;
	this->mOffset = src.mOffset;
}
Widget& Widget::operator=(const Widget& rhs)
{
	// check for self-assignment
	if ( this == &rhs )
	{
		return *this;
	}
	this->mState	   = rhs.mState;
	this->mMode		   = rhs.mMode;
	this->mTexture     = rhs.mTexture;
	this->mSurface     = rhs.mSurface;
	this->mClipping    = rhs.mClipping;
    this->mBoundaryBox = rhs.mBoundaryBox;
	this->mOrigoPosition = rhs.mOrigoPosition;
	this->pZoomModifier = rhs.pZoomModifier;
	this->pUseToggleClick = rhs.pUseToggleClick;
	this->OnWidgetClicked = rhs.OnWidgetClicked;
	this->OnWidgetDisabled = rhs.OnWidgetDisabled;
	this->OnWidgetHovered = rhs.OnWidgetHovered;
	this->OnWidgetSelected = rhs.OnWidgetSelected;
	this->pStateDependOnBinding = rhs.pStateDependOnBinding;
	//this->mTextColor   = rhs.mTextColor;
	//this->mText        = rhs.mText;
	this->mOffset	= rhs.mOffset;
	this->pWidgetInteriorClicked = rhs.pWidgetInteriorClicked;
	return *this;
}
Widget::Widget(Widget&& rhs)
{
	// Shallow copy of data
	this->mState	   = rhs.mState;
	this->mMode		   = rhs.mMode;
	this->mSoundByte   = rhs.mSoundByte;
	this->mTexture     = rhs.mTexture;
	this->mSurface     = rhs.mSurface;
	this->mClipping    = rhs.mClipping;
    this->mBoundaryBox = rhs.mBoundaryBox;
	this->mOrigoPosition = rhs.mOrigoPosition;
	this->pZoomModifier = rhs.pZoomModifier;
	this->mOffset	= rhs.mOffset;
	this->pUseToggleClick = rhs.pUseToggleClick;
	this->OnWidgetClicked = rhs.OnWidgetClicked;
	this->OnWidgetDisabled = rhs.OnWidgetDisabled;
	this->OnWidgetHovered = rhs.OnWidgetHovered;
	this->OnWidgetSelected = rhs.OnWidgetSelected;
	this->pWidgetInteriorClicked = rhs.pWidgetInteriorClicked;
	this->pStateDependOnBinding = rhs.pStateDependOnBinding;
	//this->mTextColor   = rhs.mTextColor;
	//this->mText        = rhs.mText;
	// Reset the source object - ownership has been moved
	rhs.mState		   = WS_OFF;
	rhs.mMode          = defaultLayer;
	//rhs.mTexture       = nullptr;
	//rhs.mSurface       = nullptr;
	rhs.mClipping	   = emptyRect;
    rhs.mBoundaryBox   = emptyRect;
	//rhs.mText          = std::vector<std::string>();
}
Widget& Widget::operator=(Widget&& rhs)
{
	// check for self-assignment
	if ( this == &rhs )
	{
		return *this;
	}
	FreeMemory();
	this->mState	   = rhs.mState;
	this->mMode		   = rhs.mMode;
	this->mTexture     = rhs.mTexture;
	this->mSurface     = rhs.mSurface;
	this->mClipping    = rhs.mClipping;
    this->mBoundaryBox = rhs.mBoundaryBox;
	this->mOrigoPosition = rhs.mOrigoPosition;
	this->mOffset	= rhs.mOffset;
	this->pZoomModifier = rhs.pZoomModifier;
	this->pUseToggleClick = rhs.pUseToggleClick;
	this->OnWidgetClicked = rhs.OnWidgetClicked;
	this->OnWidgetDisabled = rhs.OnWidgetDisabled;
	this->OnWidgetHovered = rhs.OnWidgetHovered;
	this->OnWidgetSelected = rhs.OnWidgetSelected;
	this->pWidgetInteriorClicked = rhs.pWidgetInteriorClicked;
	this->pStateDependOnBinding = rhs.pStateDependOnBinding;
	//this->mTextColor   = rhs.mTextColor;
	//this->mText        = rhs.mText;
	// Reset the source object - ownership has been moved
	rhs.mState		   = WS_OFF;
	rhs.mMode          = defaultLayer;
	rhs.mSoundByte     = NULL;
	//rhs.mTexture       = NULL;
	//rhs.mSurface       = NULL;
	rhs.mClipping	   = emptyRect;
    rhs.mBoundaryBox   = emptyRect;
	//rhs.mText = std::vector<std::string>();
	return *this;
}

void Widget::FreeMemory()
{
	this->mState		   = WS_OFF;
	this->mMode          = defaultLayer;
	this->mSoundByte     = NULL;
	//this->mTexture       = NULL;
	//this->mSurface       = NULL;
	this->mClipping	   = emptyRect;
    this->mBoundaryBox   = emptyRect;
	this->OnWidgetClicked = nullptr;
	this->OnWidgetDisabled = nullptr;
	this->OnWidgetHovered = nullptr;
	this->OnWidgetSelected = nullptr;
	//this->mText          = std::vector<std::string>();
}
Widget::~Widget()
{
	SetClipping(emptyRect);
	SetBoundaryBox(emptyRect);
	SetState(WS_OFF);

	pBindingWidget.reset();
}

void Widget::SetClipping(const SDL_Rect & rect)
{
	mClipping.x = rect.x;
	mClipping.y = rect.y;
	mClipping.w = rect.w;
	mClipping.h = rect.h;
}

void Widget::SetBoundaryBox(const SDL_Rect & rect)
{
	mBoundaryBox.x = rect.x;
	mBoundaryBox.y = rect.y;
	mBoundaryBox.w = rect.w;
	mBoundaryBox.h = rect.h;
}

void Widget::SetPosition( Coord pos )
{
	mBoundaryBox.x = pos.x;
	mBoundaryBox.y = pos.y;
}
Coord Widget::GetWidgetCenterPosition()
{ 
	return Coord( mBoundaryBox.x + mBoundaryBox.w/2, -mBoundaryBox.y + mBoundaryBox.h/2);
};

bool Widget::isInside(int xMouse, int yMouse)
{
	bool inside = true;

	int xBoundary = mBoundaryBox.x + mOffset.x;
	int yBoundary = mBoundaryBox.y + mOffset.y;
	xMouse += mOrigoPosition.x;
	yMouse += +mOrigoPosition.y;

	if( xMouse < xBoundary)
		inside = false;
	else if( xMouse > xBoundary + mBoundaryBox.w/this->GetZoomModifier() )
		inside = false;
	else if( yMouse < yBoundary )
		inside = false;
	else if( yMouse > yBoundary + mBoundaryBox.h/this->GetZoomModifier())
		inside = false;

	return inside;
}


void Widget::AddLinkToBindingList(std::shared_ptr<Widget> lnk)
{
	pBindingWidget = lnk;
}

void Widget::UpdateStateToBindings()
{
	if (pBindingWidget.get())
	{
		pBindingWidget->pStateDependOnBinding = true;
		pBindingWidget->SetState(this->GetState());//, false);
	}
}

const std::shared_ptr<SDL_Surface> Widget::GetSurface()
{
	if(mState == WS_HIDDEN)
		return 0;
	return mSurface;
}

const SDL_Rect* Widget::GetClipping()
{
	return &mClipping;
}

const SDL_Rect* Widget::GetBoundaryBox()
{
	return &mBoundaryBox;
}

void Widget::SetZoomingModifier(double set)
{
	this->pZoomModifier = set;
	mBoundaryBox.w /= set;
	mBoundaryBox.h /= set;
}

 const std::shared_ptr<SDL_Texture> Widget::GetTexture()
{
	return mTexture;
}

  const std::shared_ptr<SDL_Texture> Widget::GetTexture(SDL_Renderer& renderer)
{
	if(!mTexture)
		generateTexture(renderer);
	return mTexture;
}

  void Widget::Render(SDL_Texture& windowTexture)
  {
	  if (mState == WS_HIDDEN) return;
	  if (!mSurface) return;
	  if (mBoundaryBox.w == 0 || mBoundaryBox.h == 0) return;
	  SDL_UpdateTexture(&windowTexture, &mBoundaryBox, mSurface->pixels, mSurface->pitch);
	  Debug::SDLErrorCheck();
  }

void Widget::Render(SDL_Renderer& renderer)
{
	if(mState == WS_HIDDEN)
		return;
	if(!mSurface) 
		return;
	if(mBoundaryBox.w == 0 || mBoundaryBox.h == 0)
		return;
	if(!mTexture)
		generateTexture(renderer);
	if(mTexture)
	{
		char interpolation = *SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);
		auto res = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		SDL_RenderCopy(&renderer, mTexture.get(), &mClipping, &mBoundaryBox);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, &interpolation);
		Debug::SDLErrorCheck();
		mOffset = Coord(0);
	}
	
}

void Widget::Render(SDL_Renderer& renderer, Coord offset)
{
	SDL_Rect temp = mBoundaryBox;
	temp.x += offset.x;
	temp.y += offset.y;

	mOffset = offset;

	if(mState == WS_HIDDEN)
		return;
	if(!mSurface) 
		return;
	if(mBoundaryBox.w == 0 || mBoundaryBox.h == 0)
		return;
	if(!mTexture)
		generateTexture(renderer);
	if (mTexture)
	{
		char interpolation = *SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);
		auto res = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		SDL_RenderCopy(&renderer, mTexture.get(), &mClipping, &temp);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, &interpolation);
		Debug::SDLErrorCheck();
	}
}

void Widget::Render(SDL_Renderer* renderer, const Coord& pos, const SDL_Rect& clip)
{
	SDL_Rect renderQuad = { pos.x, pos.y, clip.w, clip.h };
	char interpolation = *SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);
	auto res = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	SDL_RenderCopy(renderer, this->GetTexture(*renderer).get(), &clip, &renderQuad);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, &interpolation);
	Debug::SDLErrorCheck();
}

SDL_Texture* Widget::generateTexture(SDL_Renderer & renderer)
{
	char interpolation = *SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);
	auto res = SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	if (res == SDL_FALSE)
		res = SDL_FALSE;

	mTexture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(&renderer, mSurface.get()), Debug::DestroyTexture);
	Debug::SDLErrorCheck();

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, &interpolation);

	return mTexture.get();
}


void Widget::UpdateState(SDL_Event & event )
{
	if(mState == WS_NOT_ENABLED || mState == WS_HIDDEN || mMode == WL_NON_INTERACTIVE )
		return;

	if( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int xMouse, yMouse;
		SDL_GetMouseState( &xMouse, &yMouse );

		//Check if mouse is in boundary
		bool inside = isInside(xMouse, yMouse);

		if (inside)
			this->UpdateStateInside(event);
		else
			this->UpdateStateOutside(event);
	}
	//else
	//	mState = WS_OFF;
}

void Widget::UpdateStateInside(SDL_Event & event)
{
	this->pStateDependOnBinding = false;
	//Set mouse over sprite
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		if (mState == WS_ACTIVE)
			break;
		if (UseToggleClick() && mState == WS_ON)
			break;
		if (mState == WS_OFF)
		{
			SetState(WS_HOVER);
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		WidgetClicked(event);
		break;
	case SDL_MOUSEBUTTONUP:
		if (pWidgetInteriorClicked == false)
			break;

		if (mState == WS_ACTIVE)
		{
			if (OnWidgetClicked)
				OnWidgetClicked(this);
			if (this->UseToggleClick())
			{
				SetState(WS_ON);
			}
			else
			{
				SetState(WS_HOVER);
			}
		}
		else if (mState == WS_ON)
		{
			if (OnWidgetClicked)
				OnWidgetClicked(this);
			SetState(WS_HOVER);
		}
		pWidgetInteriorClicked = false;
		break;
	}
}
void Widget::UpdateStateOutside(SDL_Event & event)
{
	if (this->pStateDependOnBinding)
		return;

	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		if (mState == WS_ACTIVE)
			break;
		if (UseToggleClick())
		{
			if (mState == WS_ON)
				break;
			//if (this->pBindingWidget.get() && this->pBindingWidget->GetState() != WS_OFF)
			//	break;
		}
		SetState(WS_OFF);
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		if (pWidgetInteriorClicked == false)
			break;

		if(UseToggleClick() == false || mState != WS_ON)
			SetState(WS_OFF);
		pWidgetInteriorClicked = false;
		break;
	}
}

void Widget::WidgetClicked(SDL_Event & e)
{
	pWidgetInteriorClicked = true;
	if (mState != WS_ON)
		SetState(WS_ACTIVE);
}

void Widget::HandleEvent(SDL_Event & event)
{
	UpdateState(event);
}

void Widget::SetState(WIDGET_STATE state, bool notinUse)//updateToBinding)
{
	if (this->mState != state)
	{
		/*this->pStateDependOnBinding = false;*/

		switch (state)
		{
		case(WIDGET_STATE::WS_ACTIVE):
			if (OnWidgetClick)
				OnWidgetClick(this);
			break;
		case(WIDGET_STATE::WS_HOVER):
			if (OnWidgetHovered)
				OnWidgetHovered(this);
			break;
		case(WIDGET_STATE::WS_NOT_ENABLED):
			if (OnWidgetDisabled)
				OnWidgetDisabled(this);
			break;
		case(WIDGET_STATE::WS_ON):
			if (OnWidgetSelected)
				OnWidgetSelected(this);
			break;
		case(WIDGET_STATE::WS_OFF):
			if (OnWidgetDisabled)
				OnWidgetDisabled(this);
			break;
		}
		this->mState = state;
		if(this->pStateDependOnBinding == false) //if(updateToBinding)
			this->UpdateStateToBindings();
	}
}

void Widget::SetState(WIDGET_STATE state)
{
	SetState(state, true);
}

void Widget::TriggerOnClickedAction()
{
	if (OnWidgetClicked)
		OnWidgetClicked(this);
}


WIDGET_STATE Widget::GetState()
{
	return mState;
}

void Widget::enable()
{
	SetState(WS_OFF);
}

void Widget::enableON()
{
	SetState(WS_ON);
}

void Widget::disable()
{
	SetState(WS_NOT_ENABLED);
}

//void Widget::SetText(const std::string text) 
//{ 
//	this->mText.clear();
//	this->mText.push_back(text);
//}

void Widget::show()
{
	SetState(WS_OFF);
}

void Widget::sethovered()
{
	SetState(WS_HOVER);
}

void Widget::hide()
{
	SetState(WS_HIDDEN);
}

void Widget::Activate(SDL_Event& e)
{

}
void Widget::Deactivate(SDL_Event& e)
{

}

void Widget::Resize( Coord size )
{
	SDL_Rect rect = { this->mBoundaryBox.x, this->mBoundaryBox.y, size.x, size.y };
	SetClipping(emptyRect);
	SetBoundaryBox(rect);

	mClipping.w = rect.w;
	mClipping.h = rect.h;
}

string StateToString(WIDGET_STATE enstate)
{
	switch (enstate)
	{
	case(WIDGET_STATE::WS_ACTIVE):
		return "WS_ACTIVE";
	case(WIDGET_STATE::WS_HOVER):
		return "WS_HOVER";
	case(WIDGET_STATE::WS_NOT_ENABLED):
		return "WS_NOT_ENABLED";
	case(WIDGET_STATE::WS_ON):
		return "WS_ON";
	case(WIDGET_STATE::WS_OFF):
		return "WS_OFF";
	case(WIDGET_STATE::WS_HIDDEN):
		return "WS_HIDDEN";
	case(WIDGET_STATE::WS_COUNT):
		return "WS_COUNT";
	}
}