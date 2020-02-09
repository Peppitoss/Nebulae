#include"scrList.h"
#include"ScreenButton.h"

ListFrame::ListFrame() : Screen()
{
	pEnableScrolling = true;
}

ListFrame::ListFrame( Coord dimensions, Coord pos ) //: Screen( pos, Coord(size.getIntValue()), nullptr )
{
	screenPosition = pos;
	pScreenWidth = dimensions.x;
	pScreenHeight = dimensions.y;
	pEnableScrolling = true;
	pTextHeight = 12;
	pWindowPosition = 0;

	pTopTextPlacement = pos.y;
	pBottomTextPlacement = pos.y + dimensions.y;
	pBottomListItemPlacement = 0;
}

void ListFrame::SetupBarPlacement( int topScrollBarPos, int bottomScrollBarPos )
{
	if( pEnableScrolling ) 
	{
		int rect_h = STANDARD_BUTTON_HEIGHT;
		int rect_w = pScreenWidth;

		int ScrollDownPlacement = bottomScrollBarPos-rect_h;
		int ScrollUpPlacement = topScrollBarPos;
		int ScrollButtonHeight = rect_h;

		pTopTextPlacement = ScrollUpPlacement + ScrollButtonHeight;
		pBottomTextPlacement = ScrollDownPlacement;
		pWindowPosition = 0;
		//pBottomListItemPlacement =  pTopTextPlacement + 8;
	}
}

void ListFrame::AddScrollBars()
{
	auto build = std::shared_ptr<Widget>(new ScreenButton(Coord(this->screenPosition.x, pBottomTextPlacement), pScreenWidth, STANDARD_BUTTON_HEIGHT, BtnText("Scroll Down")));//new BoxedButton("Scroll Down",12, Coord( this->screenPosition.x, pBottomTextPlacement), pScreenWidth, SurfaceButton::SURFACE_BUTTON_HEIGHT, SScrollDownClick));
	this->Control()->AddWidget(build);

	build = std::shared_ptr<Widget>(new ScreenButton(Coord(this->screenPosition.x, pTopTextPlacement - STANDARD_BUTTON_HEIGHT), pScreenWidth, STANDARD_BUTTON_HEIGHT, BtnText("Scroll Up")));
	this->Control()->AddWidget(build);
}

void ListFrame::renderBackground( SDL_Renderer& renderer )
{
	//if(mTexture != NULL)
	//{
		SDL_Rect renderQuad = { screenPosition.y, screenPosition.x, this->pScreenWidth, this->pScreenWidth};
		this->pBackground.Render(renderer,renderQuad );
	
		for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); ++it)
		{
			if(SelectLink *button = dynamic_cast<SelectLink*>((*it).get()))
			{
				if( VisibleInsideWindow(button->GetPosition().y ) )
				{
					this->RenderLink(button,renderer);
					//button->render(renderer, Coord(0, this->pWindowPosition));
				}
				continue;
			}
			
			//if( this->pEnableScrolling )
			//{
			//	std::string txt = (*it)->GetText();
			//	if(txt == "Scroll Down" || txt == "Scroll Up" )
			//	{
					(*it)->Render(renderer);
			//		continue;
			//	}
			//}
		}
	//}
	//else
	//	Screen::render(renderer);
	this->Control()->SetScreenState(enScreenState::SCREEN_ACTIVE, false);
}

void ListFrame::SelectNextItemInList(SDL_Event& e)
{
	if (this->pIndexOfActiveWidget < this->Control()->WidgetsCount())
		this->Control()->GetWidgetByIndex(this->pIndexOfActiveWidget)->SetWidgetClicked();
	int nextIndex = (this->pIndexOfActiveWidget + 1) % this->Control()->WidgetsCount();
	this->SelectWidgetByIndex(e,nextIndex);
}
void ListFrame::SelectPreviousItemInList(SDL_Event& e)
{
	if (this->pIndexOfActiveWidget < this->Control()->WidgetsCount())
		this->Control()->GetWidgetByIndex(this->pIndexOfActiveWidget)->SetWidgetClicked();
	int nextIndex = this->pIndexOfActiveWidget - 1;
	if (nextIndex < 0)
		nextIndex = this->Control()->WidgetsCount() - 1;
	this->SelectWidgetByIndex(e,nextIndex);
}

void ListFrame::SelectWidgetByIndex(SDL_Event& e, int index)
{
	this->Control()->GetWidgetByIndex(this->pIndexOfActiveWidget%this->Control()->WidgetsCount())->Deactivate(e);
	this->Control()->GetWidgetByIndex(index)->Activate(e);
	this->pIndexOfActiveWidget = index;
	//Screen::SelectWidgetByIndex(e, index);
}

bool ListFrame::VisibleInsideWindow( int verticalPosition )
{
	return verticalPosition > this->pTopTextPlacement - this->pWindowPosition && verticalPosition < this->pBottomTextPlacement - this->pWindowPosition;
}

void ListFrame::HandleActiveWidget( SDL_Event& e, Widget* widget )
{
	if( pEnableScrolling )
	{
		std::string txt = widget->ID();
		bool limitScroll = false;

		if(txt == "Scroll Up" )
		{
			int nextWindowPosition = this->pWindowPosition + this->pTextHeight;

			if (nextWindowPosition > 0)
			{

			}
			else
			{
				if (this->Control()->WidgetsCount() > 10)
					nextWindowPosition += this->pTextHeight;
				this->pWindowPosition = nextWindowPosition;
			}

			//if (this->pWindowPosition =  ? this->pWindowPosition : nextWindowPosition) //(nextWindowPosition + this->pTopTextPlacement + this->pTextHeight) > this->pBottomTextPlacement ? this->pWindowPosition : nextWindowPosition;
			//{
			//}
			//else
			//{
			//	
			//}
			return;
		}
		if(txt == "Scroll Down" )
		{
			int nextWindowPosition = this->pWindowPosition - this->pTextHeight;

			if (nextWindowPosition < (this->pBottomTextPlacement - this->pBottomListItemPlacement))
			{
			}
			else
			{
				if (this->Control()->WidgetsCount() > 10)
					nextWindowPosition -= this->pTextHeight;
				this->pWindowPosition = nextWindowPosition;
			}
			return;
		}
	}
}

void ListFrame::render( SDL_Renderer& renderer )
{
	renderBackground(renderer);
}

SelectLink* ListFrame::GetSelectedButton()
{
	for (auto it = this->Control()->Widget_begin(); it != this->Control()->Widget_end(); ++it)
	{
		if( VisibleInsideWindow( (*it)->GetPosition().y ) )
		{
			if(SelectLink *button = dynamic_cast<SelectLink*>((*it).get()))
			{
				if(button->isSelected())
					return button;
			}
		}
	}
}

void ListFrame::RenderLink(SelectLink * link, SDL_Renderer& renderer)
{
	link->Render(renderer, Coord(0, this->pWindowPosition));
}

std::shared_ptr<SelectLink> ListFrame::AddItemToList( std::shared_ptr<SelectLink> item, const Coord& offset)
{
	//item->autosize = true;
	this->Control()->AddWidget(item);
	pBottomListItemPlacement += pTextHeight + 8;
	return item;
}

std::shared_ptr<SelectLink> ListFrame::AddItemToList( std::string itemname, const Coord& offset )
{
	int rect_h = pTextHeight;
	int rect_w = pScreenWidth - 32;
	int x_coord = this->GetPosition().x + 32 + offset.x;
	auto test = std::shared_ptr<SelectLink>(new SelectLink(itemname.c_str() ,14, Coord(x_coord, pBottomListItemPlacement+offset.y), rect_h, rect_w ));
	return AddItemToList( test, offset );
}

void ListFrame::AddList()
{
	int x_coord = 0;//this->GetPosition().x + 32;
	int y_coord = 0;//pTopTextPlacement + 8;

	pBottomListItemPlacement =  pTopTextPlacement + 8;
	this->AddList(Coord(x_coord, y_coord));
}
