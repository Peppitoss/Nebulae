//#include"ToggleButton.h"
//
//
//ToggleButton::ToggleButton(const char* text, const imagehandling::IMGToSurface* image, Coord pos, void (*callback_arg)(SDL_Event&, Widget*) ) 
//	: SurfaceButton( Text(text), image, pos, callback_arg ) 
//{
//
//	
//
//}
//
//
//
////void ToggleButton::UpdateState( SDL_Event& event)
////{
////	UpdateStateToggle(event);
////}
//
//bool ToggleButton::isSelected()
//{
//	return mState == WS_ACTIVE;
//}
//
//void ToggleButton::DisableSelection()
//{
//	this->mState = WS_OFF;
//}