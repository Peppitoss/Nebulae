//#include"SelectTargetButton.h"
//#include"LinkSelectionHandlers.h"
//#include"GameManager.h"
//
//
//SelectTargetButton::SelectTargetButton()
//	: TransparentButton()
//{
//
//
//}
//
//SelectTargetButton::SelectTargetButton(const char* text, Coord position, int h, int w)
//	:TransparentButton(text, position, h, w, "target" )
//{
//	this->OnWidgetClicked = GetLinkFunctionType("target");
//	this->Initialize();
//}
//
//void SelectTargetButton::Initialize()
//{
//	//this->SetBackgroundColor(colorFromEnum(EnumColor::INVISIBLE));
//	//this->SetSelectionColor(colorFromEnum(EnumColor::INVISIBLE));
//}
//
//enLinkType SelectTargetButton::GetType()
//{
//	return enLinkType::LINKTARGETSELECTED;
//}
//
////SDL_Surface* SelectTargetButton::GetSelectionSurface(SDL_Rect rect)
////{
////	SDL_Surface* buttonFill = sdlWrapper.CreateSquareBorder(sdlWrapper.GetSDLColor(COLOUR::C_WHITE), rect, 2);
////	return buttonFill;
////}
//
//void SelectTargetButton::UpdateState(SDL_Event & event)
//{
//	if (mState == WS_NOT_ENABLED || mState == WS_HIDDEN || mMode == WL_NON_INTERACTIVE)
//		return;
//
//	if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
//	{
//		//Get mouse position
//		int xMouse, yMouse;
//		SDL_GetMouseState(&xMouse, &yMouse);
//
//		//Check if mouse is in boundary
//		bool inside = IsInside(xMouse, yMouse, true, false);
//
//		//Set mouse over sprite
//		switch (event.type)
//		{
//		case SDL_MOUSEMOTION:
//			if (inside && mState == WS_OFF)
//			{
//				SetState(WS_HOVER);
//			}
//			else if (!inside && mState != WS_ACTIVE)
//			{
//				SetState(WS_OFF);
//			}
//			break;
//
//		case SDL_MOUSEBUTTONDOWN:
//			if (inside)
//			{
//				WidgetClicked(event);
//			}
//			break;
//		}
//	}
//}
//
//void SelectTargetButton::WidgetClicked(SDL_Event& e)
//{
//	if (mState != WS_ACTIVE)
//		this->SetSelected(true);
//}