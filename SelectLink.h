#pragma once
#include"Link.h"
#include"GridObject.h"

class SelectLink : public Link
{

public:

	SelectLink();
	SelectLink( const char* text, TextSize, Coord position, int h, int w );

	void SetSecondaryText(string value);
	bool isSelected();
	void DisableSelection();
	void MultipleSelectAllowed( bool set ) { this->pMultipleSelect = set; };

protected:

	void UpdateState( SDL_Event& e) override;
	//SDL_Surface* GetSelectionSurface(SDL_Rect rect) override;
	void WidgetClicked(SDL_Event& e) override;

private:

	void Initialize();
	bool pMultipleSelect;

};


