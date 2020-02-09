#include"ScreenController.h"

ScreenController::ScreenController()
{
	pWidgets = std::vector<std::shared_ptr<Widget>>();

	pWidgets.reserve(10);
	pState = SCREEN_CLOSED;
	
}

bool ScreenController::ValidateWidget(Widget* widget)
{
	if (WidgetIDAlreadyExist(widget->ID()))
		throw std::exception("Error cannot add widget, a widget with same ID has already been added");
	return true;
}

bool ScreenController::AddWidget(std::shared_ptr<Widget> widget)
{
	if (ValidateWidget(widget.get()))
	{
		pWidgets.push_back(widget);
		return true;
	}
	return false;
}

bool ScreenController::AddWidget(std::shared_ptr<Widget> widget, string newWidgetID)
{
	widget->SetID(newWidgetID);
	return AddWidget(widget);
}

bool ScreenController::AddWidget(std::shared_ptr<Widget> widget, std::function<void( Widget*)> callback)
{
	if (AddWidget(widget))
	{
		widget->OnWidgetClicked = callback;
		return true;
	}
	return false;
}

bool ScreenController::AddWidget(std::shared_ptr<Widget> widget, string newWidgetID, std::function<void(Widget*)> callback)
{
	widget->SetID(newWidgetID);
	return AddWidget(widget, callback);
}

bool ScreenController::AddWidget(Widget* widget)
{
	return AddWidget(std::shared_ptr<Widget>(widget));
}

bool ScreenController::AddWidget(Widget* widget, string newWidgetID)
{
	widget->SetID(newWidgetID);
	return AddWidget(widget);
}

bool ScreenController::AddWidget(Widget* widget, std::function<void(Widget*)> callback)
{
	if (AddWidget(widget))
	{
		widget->OnWidgetClicked = callback;
		return true;
	}
	return false;
}

bool ScreenController::AddWidget(Widget* widget, string newWidgetID, std::function<void(Widget*)> callback)
{
	widget->SetID(newWidgetID);
	return AddWidget(widget, callback);

}

std::shared_ptr<Widget> ScreenController::GetWidgetByID(string ID)
{
	for (std::vector<std::shared_ptr<Widget>>::iterator it = pWidgets.begin(); it != pWidgets.end(); ++it)
		if ((*it)->ID() == ID)
			return (*it);
	return nullptr;
}

void ScreenController::SetScreenState(enScreenState set, bool triggercallback)
{
	this->pState = set;

	if (triggercallback == false)
		return;

		switch (set)
	{
	case SCREEN_ACTIVE:
		if(OnOpenScreen)
			OnOpenScreen();
		break;
	case SCREEN_CLOSED:
		if(OnCloseScreen)
			OnCloseScreen();
		ControllerCloseScreen();
		break;
	default:
		break;
	}
}

void ScreenController::SetScreenState(enScreenState set)
{
	this->SetScreenState(set, true);
}

void ScreenController::ControllerCloseScreen()
{
	for (std::vector<std::shared_ptr<Widget>>::iterator it = pWidgets.begin(); it != pWidgets.end(); ++it)
		(*it)->SetWidgetOff();
}

void ScreenController::HandleEvent(SDL_Event& e)
{
	for (std::vector<std::shared_ptr<Widget>>::iterator it = pWidgets.begin(); it != pWidgets.end(); ++it)
	{
		(*it)->HandleEvent(e);
	}

	//HandleWidgetByState(e);
}

//void ScreenController::HandleWidgetByState(SDL_Event& e)
//{
//	for (int i = 0; i < pWidgets.size(); i++)
//	{
//		if (pWidgets[i]->GetState() == WIDGET_STATE::WS_ACTIVE)
//		{
//			HandleActiveWidget(e, pWidgets[i].get());
//			break;
//		}
//	}
//}
//
//void ScreenController::HandleActiveWidget(SDL_Event& e, Widget* widget)
//{
//	if (widget->OnWidgetClicked)
//		widget->OnWidgetClicked(widget);
//}

bool ScreenController::WidgetIDAlreadyExist(string ID)
{
	return GetWidgetByID(ID) != nullptr;
}

void ScreenController::ResetData()
{
	ClearWidgets();
}

bool ScreenController::EraseWidget(std::shared_ptr<Widget> toRemove)
{
	for (auto it = this->pWidgets.begin(); it != this->pWidgets.end(); ++it)
	{
		if ((*it) == toRemove)
		{
			pWidgets.erase(it);
			return true;
		}
	}
	return false;
}

void ScreenController::ClearWidgets()
{
	for (std::vector<std::shared_ptr<Widget>>::iterator it = pWidgets.begin(); it != pWidgets.end(); ++it)
	{
		it->reset();
	}
	pWidgets.clear();
}