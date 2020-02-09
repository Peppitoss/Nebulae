#include "Screen.h"
#include"Globals.h"
#include"GameManager.h"
#include"Debug.h"


//extern void CloseTablet(SDL_Event& e, Widget* button)
//{
//	std::string txt = button->GetText();
//	if (txt == "Close")
//	{
//		GameManager::closeOpenTablet();
//	}
//}

Screen::Screen()
{
	this->screenPosition = Coord(0,0);
	Initialize();
}

Screen::Screen( const imagehandling::IMGToSurface* surf ) : pBackground( surf)
{
	this->screenPosition = Coord(0,0);
	Initialize();
}

Screen::Screen( Coord pos, Coord size, const imagehandling::IMGToSurface* surf) : pBackground( surf)
{
	this->screenPosition = pos;
	this->pScreenHeight = size.y;
	this->pScreenWidth = size.x;
	Initialize();
}

Screen::Screen( Coord pos, Coord size )
{
	this->screenPosition = pos;
	this->pScreenHeight = size.y;
	this->pScreenWidth = size.x;
	Initialize();
}

void Screen::Initialize()
{
	this->pStretchBackgroundTextures = true;
	this->pIndexOfActiveWidget = 0;
}

void Screen::SetMusic( std::string music )
{
	std::string SOUND_RESOURCE = "../SoundEffects/";
	SOUND_RESOURCE += music;
	_music = std::shared_ptr<Mix_Music>(Mix_LoadMUS ( SOUND_RESOURCE.c_str()),Mix_FreeMusic);
}

void Screen::ClearMusic()
{
	_music = nullptr;
}





void Screen::OnOpenScreen()
{

}

void Screen::SetBackground( std::string image, EnumColorKey colorKey)
{
	SetBackground( image, colorKey, 255 );
}

void Screen::SetBackground( std::string image, EnumColorKey colorKey, double opacity)
{
	std::string IMAGES = "../PictureFiles/PlayerMenu/";
	IMAGES += image;
	imagehandling::IMGToSurface* tempSurf = new imagehandling::IMGToSurface( IMAGES.c_str(), colorKey );

	pBackground.ResetTextures();
	pBackground.AddTexture( tempSurf );
	delete tempSurf;
}

void Screen::SetBackground( const imagehandling::IMGToSurface* surf, double opacity )
{
	SDL_Rect surfacedimensions = surf->GetRealSurfaceDimensions();
	pBackground.SetDimension(surfacedimensions);
	pBackground.ResetTextures();
	AddBackgroundLayer(surf,opacity);
}


void Screen::AddBackgroundLayer( std::string image, EnumColorKey colorKey, double opacity )
{
	std::string IMAGES = "../PictureFiles/PlayerMenu/";
	IMAGES += image;
	imagehandling::IMGToSurface* tempSurf = new imagehandling::IMGToSurface( IMAGES.c_str(), colorKey );
	pBackground.AddTexture( tempSurf, opacity );
	delete tempSurf;
}

void Screen::AddBackgroundLayer( const imagehandling::IMGToSurface* surf, double opacity )
{
	this->pBackground.AddTexture( surf, opacity );
}

void Screen::AddBackgroundLayer(const imagehandling::IMGToSurface* surf, const Coord& position, double opacity)
{
	this->pBackground.AddTexture(surf, position, opacity);
}

void Screen::HandleEvent (SDL_Event& e )
{
	this->Control()->HandleEvent(e);
	HandleWidgetByState(e);
}



void Screen::SetSelectedWidgetIndex(int index)
{
	this->pIndexOfActiveWidget = index;
}

void Screen::SelectWidgetByIndex(SDL_Event& e, int index)
{
	SetSelectedWidgetIndex(index);
}

Coord Screen::CenterScreenPositionBasedOnBackgroundImage()
{
	SDL_Rect dim = this->pBackground.GetDimension();
	return CenterScreenPositionBasedOnBackgroundImage(Coord(dim.w, dim.h));
}

Coord Screen::CenterScreenPositionBasedOnBackgroundImage( const Coord& compareSize )
{
	int widthDiff = DEFAULT_SCREEN_WIDTH - compareSize.x;
	int heightDiff = DEFAULT_SCREEN_HEIGHT - compareSize.y;
	auto newPosition = Coord(widthDiff / 2, heightDiff / 2);
	this->screenPosition = newPosition;

	return screenPosition;

}

void Screen::HandleActiveWidget(SDL_Event& e, Widget* widget )
{
	//if(widget->GetState() != WIDGET_STATE::WS_ACTIVE )
	//	return;

	//callback( this->mScreenState, this );
}

void Screen::HandleWidgetByState(SDL_Event& e)
{
	for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); ++it)
	{
		if ((*it)->GetState() == WIDGET_STATE::WS_ACTIVE)
		{
			HandleActiveWidget(e, (*it).get());
			break;
		}
	}
}

void Screen::RenderBackground(SDL_Renderer& renderer)
{
	RenderBackground(renderer, Coord(this->screenPosition.x, this->screenPosition.y));
}

void Screen::RenderBackground(SDL_Renderer& renderer, const Coord& position)
{
	if (this->pStretchBackgroundTextures)
	{
		SDL_Rect renderQuad = { position.x, position.y, pScreenWidth, pScreenHeight };
		pBackground.Render(renderer, renderQuad);
	}
	else
	{
		pBackground.Render(renderer, position.x, position.y);
	}
}

void Screen::render(SDL_Renderer& renderer)
{
	RenderBackground(renderer);
	// Then draw each widget associated with the screen
	for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); ++it)
	{
		RenderWidget(*it, renderer);
	}
	// Indicate that this is the active screen
	this->Control()->SetScreenState(SCREEN_ACTIVE,false);
}

void Screen::RenderWidget(std::shared_ptr<Widget> widget, SDL_Renderer& renderer)
{
	widget->Render(renderer);
}

void Screen::RenderLetter(char letter, Coord position, SDL_Renderer& renderer)
{
	SDL_Texture* txt = sdlWrapper.GetLetterTexture(letter, 12, standardLinkFont, renderer);
	RenderTexture(txt, position, renderer);
}

void Screen::RenderLetter(std::string letter, Coord position, SDL_Renderer& renderer)
{
	for (int i = 0; i < letter.size(); i++)
	{
		SDL_Texture* txt = sdlWrapper.GetLetterTexture(letter[i], 12, standardLinkFont, renderer);
		RenderTexture(txt, position.xadd(8), renderer);
	}
}

void Screen::RenderTexture(SDL_Texture* txt, Coord position, SDL_Renderer& renderer)
{
	int w, h;
	SDL_QueryTexture(txt, NULL, NULL, &w, &h);
	Debug::SDLErrorCheck();

	SDL_Rect destination = { position.x, position.y,w,h };
	SDL_RenderCopy(&renderer, txt, NULL, &destination);
	Debug::SDLErrorCheck();
}

bool Screen::PointWithinScreen( const Coord point )
{
	return geometry::pointWithinRectangle_inclusive(point,this->screenPosition,this->pScreenWidth,this->pScreenHeight);
}

bool Screen::PointWithinScreen(const Coord point, bool RequireThatScreenIsActive)
{
	if (RequireThatScreenIsActive && this->isActive())
		return PointWithinScreen(point);
}

//void Screen::
Screen::~Screen(void)
{
	this->Control()->ResetData();
}
