#pragma once
#include"Screen.h"
#include"SelectLink.h"

class ListFrame : public Screen
{
public:

	ListFrame();
	ListFrame( Coord dimensions, Coord );


	void SelectNextItemInList(SDL_Event& e);
	void SelectPreviousItemInList(SDL_Event& e);


	void renderBackground( SDL_Renderer& renderer );
	void render( SDL_Renderer& renderer ) override;

protected:

	void HandleActiveWidget( SDL_Event& e, Widget* widget ) override;
	virtual void AddList(const Coord& pos) = 0;
	virtual void AddList();

	SelectLink* GetSelectedButton();
	virtual void SetupBarPlacement( int topScrollBarPos, int bottomScrollBarPos );
	void AddScrollBars();
	virtual void RenderLink(SelectLink *, SDL_Renderer& renderer);

	int pTextHeight;
	bool pEnableScrolling;

	int pTopTextPlacement;
	int pBottomTextPlacement;
	int pWindowPosition;
	int pBottomListItemPlacement;

	bool VisibleInsideWindow( int verticalPosition );

	virtual std::shared_ptr<SelectLink> AddItemToList( std::shared_ptr<SelectLink> item, const Coord& offset);
	virtual std::shared_ptr<SelectLink> AddItemToList( std::string item, const Coord& offset);

	void SelectWidgetByIndex(SDL_Event& e, int index) override;

private:

	
	

};
