#include"scrGrid.h"


GridFrame::GridFrame( spriteSize size, Coord pos )
{
	screenPosition = pos;
	pScreenWidth = size.width();
	pScreenHeight = size.height();
	
	//pParent = parent;

	//this->Setup( size, txtCollection ); //refactor later

}

void GridFrame::Setup( spriteSize size, spriteSize gridBoxPerSide )
{
	pGrid = std::unique_ptr<Grid>(new Grid( size, screenPosition,gridBoxPerSide )); //could be placed a bit different than the screen position, depending on screen layout
	pGrid->generateGrid( size.GetSize() );
	pGrid->SetOrigo(Coord(0));
}

void GridFrame::renderBackground( SDL_Renderer& renderer )
{
	//if(mTexture != NULL)
	//{
	SDL_Rect renderQuad = { screenPosition.x, screenPosition.y, this->pScreenWidth, this->pScreenHeight};
		this->pBackground.Render(renderer,renderQuad );
	
		for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); ++it)
		{
			(*it)->Render(renderer);
		}
	//}
	//else
	//	Screen::render(renderer);
	this->Control()->SetScreenState(enScreenState::SCREEN_ACTIVE, false);
}

void GridFrame::render( SDL_Renderer& renderer )
{
	renderBackground(renderer);
	this->pGrid->Render( &renderer );
}

void GridFrame::HandleActiveWidget( SDL_Event& e, Widget* widget )
{
	Screen::HandleActiveWidget(e,widget);
}

void GridFrame::HandleEvent( SDL_Event& e)
{
	Screen::HandleEvent(e);
}


