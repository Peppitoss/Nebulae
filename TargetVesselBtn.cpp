//#include"TargetVesselBtn.h"
//#include"Wrapper.h"
//
//TargetVesselBtn::TargetVesselBtn()
//{
//
//}
//
//TargetVesselBtn::TargetVesselBtn( const char* text, TextSize size, Coord position, int h, int w ) : SelectButton(text, size, position, h,w )
//{
//
//}
//
//TargetVesselBtn::TargetVesselBtn( const char* text, TextSize size, Coord pos, int h, int w, void (*callback_arg)(SDL_Event & event, Widget*) ) : SelectButton(text, size, pos, h,w, callback_arg )
//{
//
//}
//
//SDL_Surface* TargetVesselBtn::GetSelectionSurface(SDL_Rect rect)
//{
//	SDL_Surface* buttonFill = sdlWrapper.CreateSurface(rect.w, rect.h);
//	SDL_Surface* square = Wrapper::CreateSquare(COLOUR::C_WHITE, rect);
//	SDL_PixelFormat * pixelFormat = buttonFill->format;
//	SDL_FillRect(buttonFill, NULL,SDL_MapRGBA(pixelFormat,this->GetSelectionColor().red(),this->GetSelectionColor().green(),this->GetSelectionColor().blue(),this->GetSelectionColor().alpha()) );
//	SDL_BlitSurface(square, NULL, buttonFill, NULL);
//	SDL_FreeSurface(square);
//	return buttonFill;
//}