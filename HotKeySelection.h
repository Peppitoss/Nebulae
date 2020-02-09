#pragma once
#include"Link.h"
#include"KeyReader.h"

enum enHKDisplay
{
	HKSingleLetter = 1,
	HKWide = 2,
};

enum enHKIcon	//not in use 
{
	NoIcon = 0,
	ArrowUp = 1,
	ArrowDown = 2,
	ArrowToRight = 4,
	ArrowToLeft = 8
};

class HotKeySelection : public Link
{

public:

	HotKeySelection(enHKDisplay type, string key, Coord globalPos);
	//HotKeySelection(enHKIcon Icon, char key, Coord globalPos);

	void SetKeyFiler(enKeyReaderFilter filter) { this->pKeyReader.SetFilter(filter); };
	void HandleLinkIsSelected(SDL_Event & event) override;
	void SetHotKey(string);
	void RemoveHotKey();
	string GetHotKey();

	//enLinkType GetType() override;

protected:

	//SDL_Surface* GetStandardSurface(SDL_Rect rect) override;
	//SDL_Surface* GetSelectionSurface(SDL_Rect rect) override;
	//SDL_Surface* Get_WS_ACTIVE_Surface(SDL_Rect rect) override;
	//SDL_Surface* Get_WS_HOVER_Surface(SDL_Rect rect) override;

	Coord TextDrawPosition() override;

private:

	KeyReader pKeyReader;

	enHKDisplay pDisplayType;
	enHKIcon pIcon;

	SDL_Surface* GetDrawSurfaceFromType();
	SDL_Surface* GetIconFromType();

	SDL_Surface* CropSurface(SDL_Rect* dim, SDL_Surface* Surface);
	SDL_Surface* CropIconToEntireImage(int height, SDL_Surface* Surface);

	//void SetupImage();
	void Initialize();

};