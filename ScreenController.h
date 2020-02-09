#pragma once
#include "Widget.h"
#include<functional>

class Screen;

enum enScreenState
{
	SCREEN_ACTIVE,
	SCREEN_CLOSED,
};

class ScreenController
{

public:
	
	ScreenController();

	void HandleEvent(SDL_Event& e);

	void SetScreenState(enScreenState set, bool Triggercallback);
	void SetScreenState(enScreenState set);
	inline enScreenState GetState() const { return this->pState; }

	bool AddWidget(std::shared_ptr<Widget> widget);
	bool AddWidget(std::shared_ptr<Widget> widget, string newWidgetID );
	bool AddWidget(std::shared_ptr<Widget> widget, std::function<void( Widget*)> callback);
	bool AddWidget(std::shared_ptr<Widget> widget, string newWidgetID, std::function<void( Widget*)> callback);
	bool AddWidget(Widget* widget);
	bool AddWidget(Widget* widget, string newWidgetID);
	bool AddWidget(Widget* widget, std::function<void( Widget*)> callback);
	bool AddWidget(Widget* widget, string newWidgetID, std::function<void(Widget*)> callback);

	std::shared_ptr<Widget> GetWidgetByID(string ID);
	std::shared_ptr<Widget> GetWidgetByIndex(int index) { return this->pWidgets[index]; }


	std::vector<std::shared_ptr<Widget>>::const_iterator Widget_begin() { return this->pWidgets.begin(); }
	std::vector<std::shared_ptr<Widget>>::const_iterator Widget_end() { return this->pWidgets.end(); }
	int WidgetsCount() { return this->pWidgets.size(); }

	std::vector<std::shared_ptr<Widget>>::const_iterator EraseWidget(std::vector<std::shared_ptr<Widget>>::const_iterator it) { return this->pWidgets.erase(it); }
	bool EraseWidget(std::shared_ptr<Widget> widget);

	void ResetData();
	void ClearWidgets();

	std::function<void()> OnOpenScreen;
	std::function<void()> OnCloseScreen;

private:

	enScreenState pState;
	std::vector<std::shared_ptr<Widget>> pWidgets;

	void ControllerCloseScreen();

	bool WidgetIDAlreadyExist(string ID);
	bool ValidateWidget(Widget*);
	
};