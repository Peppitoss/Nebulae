#pragma once
#include<vector>
#include"Screen.h"
#include"ScreenButton.h"

using imagehandling::TextureObject_2;

class scrHUDDisplay : public Screen
{
public:

	scrHUDDisplay(const Coord& pos);

	void RenderWidget(std::shared_ptr<Widget> widget, SDL_Renderer&) override;

	std::shared_ptr<Widget> AddLine(string text, string lineID);
	std::shared_ptr<Widget> AddLine(string text, int lineID);
	void SelectNextItemInList();
	void SelectPreviousItemInList();

	void ResetLines();

	std::function<void(Widget* w)> LineClicked;

protected:



private:
	
	Widget* pSelectedLine;
	std::vector<std::shared_ptr<Widget>> pLines;

};


