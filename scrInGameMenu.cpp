#include"scrInGameMenu.h"
#include"SurfaceButton.h"
#include"GameManager.h"
#include"BackgroundCollection.h"
#include"ScreenButton.h"

InGameMenu::InGameMenu( int screen_height , int screen_width, Coord pos )
{
	pScreenWidth = screen_width;
	pScreenHeight = screen_height;

	pStretchBackgroundTextures = false;

	int spacing = screen_height/10;
	int x_coord = screen_width/3;
	int y_coord = screen_height/5+spacing;
	int rect_h = STANDARD_BUTTON_HEIGHT;
	int rect_w = x_coord;

	int y_shift = 128;
	int x_shift = 0;
	
	auto build = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord + pos.y + x_shift, y_coord + pos.x + y_shift), 128, STANDARD_BUTTON_HEIGHT, BtnText("Build")));//new BoxedButton("Build", ImportImage_ButtonBuild, Coord(x_coord+pos.y+x_shift, y_coord+pos.x+y_shift), SBuilderClick));
	this->Control()->AddWidget(build);
	y_coord += STANDARD_BUTTON_HEIGHT*1.5;

	auto restart = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord + pos.y + x_shift, y_coord + pos.x + y_shift), 128, STANDARD_BUTTON_HEIGHT, BtnText("Restart")));
	this->Control()->AddWidget(restart);
	y_coord += STANDARD_BUTTON_HEIGHT*1.5;

	auto close = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord + pos.y + x_shift, y_coord + pos.x + y_shift), 128, STANDARD_BUTTON_HEIGHT, BtnText("Close"))); //new BoxedButton("Close",16, Coord(x_coord+pos.y+x_shift, y_coord+pos.x+y_shift), rect_w, rect_h, CloseClick));
	this->Control()->AddWidget(close);
	y_coord += STANDARD_BUTTON_HEIGHT*1.5;

	auto quit = std::shared_ptr<Widget>(new ScreenButton(Coord(x_coord + pos.y + x_shift, y_coord + pos.x + y_shift), 128, STANDARD_BUTTON_HEIGHT, BtnText("Quit")));
	this->Control()->AddWidget(quit);
	y_coord += STANDARD_BUTTON_HEIGHT*1.5;

	//pParent = parent;

	this->SetBackground(ImportImage_PlayerMenuBorder, 255 );
	this->AddBackgroundLayer(ImportImage_PlayerMenuScreen, 200 );
	this->screenPosition = this->CenterScreenPositionBasedOnBackgroundImage();
	this->screenPosition.y -= 48;
}

//void PlayerMenu::render( SDL_Renderer& renderer )
//{
//	//if(mTexture != NULL)
//	//{
//	//	SDL_Rect renderQuad = { screenPosition.y, screenPosition.x, this->pScreenWidth, this->pScreenWidth};
//	//	SDL_RenderCopy(&renderer, mTexture.get(), NULL, &renderQuad);
//	//
//	//	// Then draw each widget associated with the screen
//	//	for (std::vector<std::shared_ptr<Widget>>::iterator it = _widgets.begin(); it != _widgets.end(); ++it)
//	//	{
//	//		(*it)->render(renderer);
//	//	}
//	//}
//	//else
//	//	Screen::render(renderer);
//	SDL_Rect renderQuad = { screenPosition.y, screenPosition.x, this->pScreenWidth, this->pScreenWidth};
//	this->_background.Render(renderer,renderQuad );
//	// Indicate that this is the active screen
//	mScreenState = SCREEN_ACTIVE;
//}

void InGameMenu::HandleActiveWidget( SDL_Event& e, Widget* widget )
{
	Screen::HandleActiveWidget(e,widget);
	std::string txt = widget->ID();
	
	if(txt == "Close" )
	{
		GameManager::Screens.closePlayerMenu();
		return;
	}

	if(txt == "Restart" )
	{
		GameManager::restart();
		return;
	}

	if(txt == "Quit" )
	{
		GameManager::Screens.quitToMenu(true);
		return;
	}

	if(txt == "Build")
	{
		GameManager::Screens.openSBuilder();
		return;
	}

}

InGameMenu::~InGameMenu()
{
}
