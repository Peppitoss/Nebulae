#include"HotKeySelection.h"
#include"BackgroundCollection.h"
#include"IconCollection.h"
#include"SDLKeyboardSyntax.h"
#include"Debug.h"

HotKeySelection::HotKeySelection(enHKDisplay type, string key, Coord globalPos)
	: Link(Text(key.c_str()), 12, globalPos, 24,24)
{
	pDisplayType = type;
	if (pDisplayType == enHKDisplay::HKSingleLetter)
		this->pIcon = enHKIcon::NoIcon;
	else
	{
		this->Resize(Coord(48, 24));
		this->pIcon = enHKIcon::ArrowDown;
	}
	Initialize();
}

//HotKeySelection::HotKeySelection(enHKIcon Icon, char key, Coord globalPos)
//	: Link(&key, 12, globalPos, 24, 24)
//{
//	this->pIcon = Icon;
//	if (this->pIcon != enHKIcon::NoIcon)
//	{
//		pDisplayType = enHKDisplay::HKImage;
//		this->Resize(Coord(48, 24));
//	}
//	Initialize();
//}

//enLinkType HotKeySelection::GetType()
//{
//	return enLinkType::LINKHOTKEYSELECT;
//}

string HotKeySelection::GetHotKey()
{
	return pKeyReader.GetText();
}

void HotKeySelection::SetHotKey(string set)
{
	if (set != "" && this->GetText() != set) 
	{
		pKeyReader.AssignText(set);
		this->RenewText(pKeyReader.GetText());
	}
}

void HotKeySelection::RemoveHotKey()
{
	pKeyReader.ResetText();
	this->RenewText(pKeyReader.GetText());
}

void HotKeySelection::HandleLinkIsSelected(SDL_Event & event)
{
	pKeyReader.HandleEvent(event);
	string text = pKeyReader.GetText();
	if (this->GetText() != text)
	{
		this->SetSelected(false);
		this->RenewText(pKeyReader.GetText());
	}
	else if(SDLKeyBible::IsEscape(event.key.keysym.scancode))
		this->SetSelected(false);
}

SDL_Surface* HotKeySelection::GetDrawSurfaceFromType()
{
	SDL_Surface* result = nullptr;
	switch (pDisplayType)
	{
	case(enHKDisplay::HKWide):
		return ImportImage_HotKeyDual->GetSurface().get();
	case(enHKDisplay::HKSingleLetter):
		return ImportImage_HotKeySingle->GetSurface().get();
	default:
		break;
	}
}
//
//SDL_Surface* HotKeySelection::GetStandardSurface(SDL_Rect rect)
//{
//	SDL_Surface* entireImage = GetDrawSurfaceFromType();
//	auto dim = ImportImage_HotKeySingle->GetSpriteSize().GetDimension();
//	SDL_Rect crop = { 0, dim.y / 3, this->GetBoundaryBox()->w, this->GetBoundaryBox()->h };
//	entireImage = CropIconToEntireImage(dim.y / 3, entireImage);
//
//	return CropSurface(&crop, entireImage);
//}
//
//SDL_Surface* HotKeySelection::GetSelectionSurface(SDL_Rect rect)
//{
//	SDL_Surface* entireImage = GetDrawSurfaceFromType();
//	auto dim = ImportImage_HotKeySingle->GetSpriteSize().GetDimension();
//	SDL_Rect crop = { 0, dim.y / 6, this->GetBoundaryBox()->w, this->GetBoundaryBox()->h };
//	entireImage = CropIconToEntireImage(dim.y / 6, entireImage);
//
//	return CropSurface(&crop, entireImage);
//}
//
//SDL_Surface* HotKeySelection::Get_WS_HOVER_Surface(SDL_Rect rect)
//{
//	SDL_Surface* entireImage = GetDrawSurfaceFromType();
//	auto dim = ImportImage_HotKeySingle->GetSpriteSize().GetDimension();
//	SDL_Rect crop = { 0, 2 * dim.y / 3, this->GetBoundaryBox()->w, this->GetBoundaryBox()->h };
//	entireImage = CropIconToEntireImage(dim.y / 3, entireImage);
//
//	return CropSurface(&crop, entireImage);
//}
//
//SDL_Surface* HotKeySelection::Get_WS_ACTIVE_Surface(SDL_Rect rect)
//{
//	SDL_Surface* entireImage = GetDrawSurfaceFromType();
//	auto dim = ImportImage_HotKeySingle->GetSpriteSize().GetDimension();
//	SDL_Rect crop = { 0, dim.y / 2, this->GetBoundaryBox()->w, this->GetBoundaryBox()->h };
//	entireImage = CropIconToEntireImage(dim.y / 2, entireImage);
//
//	return CropSurface(&crop, entireImage);
//}

Coord HotKeySelection::TextDrawPosition()
{
	switch (pDisplayType)
	{
	case(enHKDisplay::HKWide):
		return Coord(10 + 12, 4);
	case(enHKDisplay::HKSingleLetter):
		return Coord(10, 4);
	}
}

SDL_Surface* HotKeySelection::CropIconToEntireImage(int height, SDL_Surface* Surface)
{
	if (this->pIcon != enHKIcon::NoIcon)
	{
		SDL_Surface* icon = GetIconFromType();
		SDL_Rect iconTranslation = { 0, 0, 5, 5 + height };
		SDL_BlitSurface(icon, NULL, Surface, &iconTranslation);

		Debug::SDLErrorCheck();
	}
	return Surface;
}

SDL_Surface* HotKeySelection::GetIconFromType()
{
	switch (this->pIcon)
	{
	case(enHKIcon::ArrowDown):
		return IconTextures::ImportIcon_ArrowDown->GetSurface().get();
	case(enHKIcon::ArrowUp):
		return IconTextures::ImportIcon_ArrowDown->GetSurface().get();	//rotate this . . somehow :)
	default:
		break;
	}
}

SDL_Surface* HotKeySelection::CropSurface(SDL_Rect* dim, SDL_Surface* Surface)
{
	SDL_Surface* buttonFill = sdlWrapper.CreateSurface(dim->w, dim->h);
	SDL_PixelFormat * pixelFormat = Surface->format;
	SDL_BlitSurface(Surface, dim, buttonFill, NULL);
	Debug::SDLErrorCheck();

	return buttonFill;
}

//void HotKeySelection::SetupImage()
//{
//	switch (pDisplayType)
//	{
//	case(enHKDisplay::HKImage):
//		break;
//	case(enHKDisplay::HKNoImage):
//		break;
//	default:
//		break;
//	}
//}

void HotKeySelection::Initialize()
{
	pKeyReader = KeyReader();
	pKeyReader.SetFilter(enKeyReaderFilter::keyNumbers);
	pKeyReader.SetType(enKeyReaderType::keySingleLetterOverride);
	pKeyReader.AssignText(this->GetText());
	this->RenewText(pKeyReader.GetText());
	//centerTextHorizontally = true;
	Refresh();
	this->SetState(WIDGET_STATE::WS_ON);
}