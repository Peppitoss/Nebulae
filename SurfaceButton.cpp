#include"SurfaceButton.h"
#include"Wrapper.h"
#include"Debug.h"
#include <stdexcept>

SurfaceButton::SurfaceButton()
{
	Initialize();
}

SurfaceButton::SurfaceButton( string id, int x, int y, ButtonStyle style)
	: Widget( id, nullptr, x, y, style.Dimension().x, style.Dimension().y)
{
	pButtonStyle = style;
	Initialize();
}

SurfaceButton::SurfaceButton(string id, Coord pos, ButtonStyle style)
	: Widget(id, nullptr, pos.x, pos.y, style.Dimension().x, style.Dimension().y)
{
	pButtonStyle = style;
	Initialize();
}

void SurfaceButton::Initialize()
{
	pCurrentlyRefreshing = false;
	pRefreshOnlyText = false;
}

std::shared_ptr<Mix_Chunk> SurfaceButton::GetHoverEffect()
{
	return _buttonHover;
}
std::shared_ptr<Mix_Chunk> SurfaceButton::GetClickEffect()
{
	return _buttonClick;
}

void SurfaceButton::FreeMemory()
{
	Widget::FreeMemory();
	_buttonClick = nullptr;
	_buttonHover = nullptr;
}
SurfaceButton::~SurfaceButton()
{
	FreeMemory();
}

void SurfaceButton::SetState(WIDGET_STATE state)
{
	Widget::SetState(state);
	mClipping.y = mBoundaryBox.h * this->pButtonStyle.GetStatePermutation(state);
}

const SDL_Rect * SurfaceButton::GetClipping()
{
	mClipping.y = mBoundaryBox.h * this->pButtonStyle.GetStatePermutation(mState);
	return & mClipping;
}
void SurfaceButton::HandleEvent(SDL_Event & event)
{
	UpdateState(event);
}
void SurfaceButton::InitializeSoundEffects()
{
	char* hoverEffect = "../SoundEffects/hover.wav";
	char* clickEffect = "../SoundEffects/click.wav";
	_buttonHover = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV( hoverEffect ), Mix_FreeChunk);
	_buttonClick = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV( clickEffect ), Mix_FreeChunk);
	if ( _buttonHover == NULL || _buttonClick == NULL )
	{
		throw std::runtime_error("Error loading sound-effects..");
	}
}

void SurfaceButton::Refresh()
{
	mTexture.reset();
	mSurface.reset();
	GenerateSurface(false);
}

void SurfaceButton::RefreshText()
{
	mTexture.reset();
	mSurface.reset();
	GenerateSurface(true);
}

void SurfaceButton::RefreshText(const string& newText)
{
	if( ChangeText(newText) )
		Refresh();	//might need to change size, so RefreshText() is not sufficient
}

void SurfaceButton::Resize(Coord size)
{
	//this->pButtonStyle.SetAutoSize(false);
	this->pButtonStyle.ChangeDimension(size);
}

string SurfaceButton::GetText()
{
	try {
		return this->pButtonStyle.GetTextProperties(this->mState,0).pText.GetCurrentValue();
	}
	catch (int e)
	{
		return "";
	}
	
}

ColorRGB SurfaceButton::GetTextColor()
{
	return this->pButtonStyle.GetTextProperties(this->mState,0).TextColor;
}

bool SurfaceButton::ChangeText(const Text& newText)
{
	return this->pButtonStyle.ChangeTextStyle(newText);
}
bool SurfaceButton::ChangeText(const string& newText)
{
	return this->pButtonStyle.ChangeTextStyle(newText);
}

bool SurfaceButton::ChangeText(const ColorRGB& color)
{
	return this->pButtonStyle.ChangeTextStyle(color);
}


void SurfaceButton::GenerateSurface( bool refreshTextOnly )
{
	pCurrentlyRefreshing = true;
	pRefreshOnlyText = refreshTextOnly;

	int height = this->pButtonStyle.Dimension().y;
	int width = this->pButtonStyle.Dimension().x;
	SDL_Surface* image = Wrapper::CreateSurface(width, height*this->pButtonStyle.GetStatesCount(), sdlWrapper.GetSDLColor(COLOUR::C_DEFAULT));
	//SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, this->pButtonStyle.GetColorKey().red(), this->pButtonStyle.GetColorKey().green(), this->pButtonStyle.GetColorKey().blue()));
	//SDL_SetSurfaceAlphaMod(image, 0);
	//pStatePermutation = std::map < WIDGET_STATE, WIDGET_STATE>();

	//std::set<WIDGET_STATE>::iterator const UsedStatesBegin() { return pUsedStates.begin(); };
	//std::set<WIDGET_STATE>::iterator const UsedStatesEnd() { return pUsedStates.end(); };

	for (auto it = pButtonStyle.UsedStatesBegin(); it != pButtonStyle.UsedStatesEnd(); ++it)
	{
		SDL_Rect boundary = sdlWrapper.CreateRect(0, 0, width, height);
		SDL_Surface* surf = this->GetSurfaceFromState(*it, boundary);

		//SDL_SetSurfaceAlphaMod(surf, 100);
		//int stateResult = this->pButtonStyle.GetStatePermutation()[(WIDGET_STATE)i];
		SDL_Rect clip = sdlWrapper.CreateRect(0, (*it)*height, width, height);
		SDL_BlitSurface(surf, &boundary, image, &clip);
		Debug::SDLErrorCheck();
		SDL_FreeSurface(surf);
	}

	pCurrentlyRefreshing = false;
	pRefreshOnlyText = false;

	SDL_Rect newBoundary = { mBoundaryBox.x, mBoundaryBox.y, width, height };
	SDL_Rect newClip = { 0, 0, width, height };
	this->SetBoundaryBox(newBoundary);
	this->SetClipping(newClip);
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, this->pButtonStyle.GetColorKey().red(), this->pButtonStyle.GetColorKey().green(), this->pButtonStyle.GetColorKey().blue()));
	Debug::SDLErrorCheck();
	//SDL_SetSurfaceAlphaMod(image, 100);
	mSurface = std::shared_ptr<SDL_Surface>(image, SDL_FreeSurface);
}

SDL_Surface* SurfaceButton::GetSurfaceFromState( int i, SDL_Rect rect)
{
	switch( i )
	{
	case(0) :
		return Get_WS_OFF_Surface(rect);
	case(1) :
		return Get_WS_HOVER_Surface(rect);
	case(2) :
		return Get_WS_ACTIVE_Surface(rect);
	case(3) :
		return Get_WS_ON_Surface(rect);
	case(4) :
		return Get_WS_NOT_ENABLED_Surface(rect);
	case(5) :
		return Get_WS_HIDDEN_Surface(rect);
	default :
		return nullptr;
	}
}


SDL_Surface* SurfaceButton::Get_WS_OFF_Surface(SDL_Rect rect)
{
	return GetSurfaceFromState(WS_OFF);
}

SDL_Surface* SurfaceButton::Get_WS_HOVER_Surface(SDL_Rect rect)
{
	return GetSurfaceFromState(WS_HOVER);
}

SDL_Surface* SurfaceButton::Get_WS_ACTIVE_Surface(SDL_Rect rect)
{
	return GetSurfaceFromState(WS_ACTIVE);
}

SDL_Surface* SurfaceButton::Get_WS_ON_Surface(SDL_Rect rect)
{
	return GetSurfaceFromState(WS_ON);
}

SDL_Surface* SurfaceButton::Get_WS_NOT_ENABLED_Surface(SDL_Rect rect)
{
	return GetSurfaceFromState(WS_NOT_ENABLED);
}

SDL_Surface* SurfaceButton::Get_WS_HIDDEN_Surface(SDL_Rect rect)
{
	return GetSurfaceFromState(WS_HIDDEN);
}

SDL_Surface* SurfaceButton::GetSurfaceFromState(WIDGET_STATE state)
{
	return GetSurfaceFromState(state, pRefreshOnlyText);
}

SDL_Surface* SurfaceButton::GetSurfaceFromState(WIDGET_STATE state, bool refreshTextOnly)
{
	WIDGET_STATE currentState = this->pButtonStyle.GetState_Safe(state);
	//this->pStatePermutation[state] = currentState;

	if (this->pCurrentlyRefreshing)
		return this->pButtonStyle.GetNewSurface(currentState, true, !refreshTextOnly);
	return nullptr;
}