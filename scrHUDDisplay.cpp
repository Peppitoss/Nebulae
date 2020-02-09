#pragma once
#include"scrHUDDisplay.h"
#include"BackgroundCollection.h"
#include"StringBuilder.h"

scrHUDDisplay::scrHUDDisplay(const Coord& pos) : Screen(pos, Coord(243,126))
{
	this->SetBackground(ImportImage_HUDDisplay, 200);
}


void scrHUDDisplay::RenderWidget(std::shared_ptr<Widget> widget, SDL_Renderer& renderer)
{
	Screen::RenderWidget(widget, renderer);
}

std::shared_ptr<Widget> scrHUDDisplay::AddLine(string text, int lineID)
{
	return AddLine(text, IntToString(lineID));
}

std::shared_ptr<Widget> scrHUDDisplay::AddLine(string text, string lineID)
{
	int lineNumber = this->pLines.size();

	int height = 5 + 24 * lineNumber;

	auto newLine = std::shared_ptr<Widget>(new ScreenButton(Coord(2 + this->screenPosition.x, height + this->screenPosition.y), text, 10, ImportImage_HUDDisplayLine ));
	newLine->SetID(lineID);
	newLine->SetUseToggleClick(true);
	newLine->OnWidgetClicked = [&](Widget* w)
	{
		pSelectedLine = w;
		for (int i = 0; i < this->pLines.size(); i++)
		{
			if (this->pLines[i].get() != w)
				this->pLines[i]->SetWidgetOff();
		}
		LineClicked(w);
	};
	this->Control()->AddWidget(newLine);
	this->pLines.push_back(newLine);
	return newLine;
}

void scrHUDDisplay::SelectNextItemInList()
{
	if (pLines.size() == 0)
		return;
		
	for (int i = 0; i < this->pLines.size(); i++)
	{
		if (pSelectedLine == pLines[i].get())
		{
			auto line = pLines[++i % pLines.size()];
			line->TriggerOnClickedAction();
			line->SetWidgetClicked();
		}
	}
}

void scrHUDDisplay::SelectPreviousItemInList()
{
	if (pLines.size() == 0)
		return;

	for (int i = 0; i < this->pLines.size(); i++)
	{
		if (pSelectedLine == pLines[i].get())
		{
			auto line = pLines[--i % pLines.size()];
			line->TriggerOnClickedAction();
			line->SetWidgetClicked();
		}
	}
}

void scrHUDDisplay::ResetLines()
{
	for (int i = 0; i < this->pLines.size(); i++)
		this->Control()->EraseWidget(this->pLines[i]);
	for (int i = 0; i < this->pLines.size(); i++)
		this->pLines[i].reset();
	this->pLines.clear();
		
}