#include"SelectLink.h"


SelectLink::SelectLink() : Link()
{
	Initialize();
}
SelectLink::SelectLink( const char* text, TextSize size, Coord position, int h, int w ) : Link(Text(text), size, position, h,w )
{
	Initialize();
}

void SelectLink::Initialize()
{
	this->pMultipleSelect = false;
}

//enLinkType TransparentButton::GetType()
//{
//	return enLinkType::LINKHUDBUTTON;
//}

void SelectLink::UpdateState( SDL_Event& event)
{
	if (mState == WS_NOT_ENABLED || mState == WS_HIDDEN || mMode == WL_NON_INTERACTIVE)
		return;

	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int xMouse, yMouse;
		SDL_GetMouseState(&xMouse, &yMouse);

		//Check if mouse is in boundary
		bool inside = isInside(xMouse + mOrigoPosition.x, yMouse + mOrigoPosition.y);

		//Set mouse over sprite
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			if (inside && mState == WS_OFF)
			{
				SetState(WS_HOVER);
			}
			else if (!inside && mState != WS_ACTIVE)
			{
				this->SetSelected(false);
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (inside)
			{
				WidgetClicked(event);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(!inside && !pMultipleSelect )
			{
				this->SetSelected(false);
			}
			break;
		}
	}
}

void SelectLink::WidgetClicked(SDL_Event& e)
{
	if (mState == WS_ACTIVE)
		this->SetSelected(false);
	else
		this->SetSelected(true);
}

bool SelectLink::isSelected()
{
	return mState == WS_ACTIVE;
}

void SelectLink::SetSecondaryText(string value)
{
	//if (this->mText.size() <= 1)
	//	this->mText.push_back(value);
	//else
	//	this->mText[1] = value;
}

void SelectLink::DisableSelection()
{
	this->SetState(WS_OFF);
}


//SDL_Surface* SelectLink::GetSelectionSurface(SDL_Rect rect)
//{
//	SDL_Surface* buttonFill = sdlWrapper.CreateSurface(rect.w, rect.h);
//	SDL_PixelFormat * pixelFormat = buttonFill->format;
//	SDL_FillRect(buttonFill, NULL,SDL_MapRGBA(pixelFormat,this->GetSelectionColor().red(),this->GetSelectionColor().green(),this->GetSelectionColor().blue(),this->GetSelectionColor().alpha()) );
	
	//if( this->pUseBorder )
	//{
	//	SDL_Surface* square = Wrapper::CreateSquare(Wrapper::GetSDLColor( COLOUR::C_BLUE), rect);	//square here should be hollow!
	//	SDL_BlitSurface(buttonFill, NULL, square, NULL);
	//	SDL_FreeSurface(square);
//	//}
//	return buttonFill;
//}