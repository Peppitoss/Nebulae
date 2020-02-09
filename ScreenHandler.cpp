#include"ScreenHandler.h"
#include"StringBuilder.h"

ScreenHandler::ScreenHandler()
{
	Init();
}

void ScreenHandler::AddScreen( std::shared_ptr<Screen> scr, string name)
{
	pScreenCollection[name] = scr;
}

void ScreenHandler::AddScreen(std::shared_ptr<Screen> scr, string name, bool active)
{
	AddScreen(scr, name);
	pScreenCollection[name]->OpenScreen();
}

void ScreenHandler::AddScreen(std::shared_ptr<Screen> scr, string name, bool active, bool AllowDuplicateNames)
{
	if (this->GetScreenByName(name) == nullptr)
		AddScreen(scr, name, active);
	else
	{
		AddScreen(scr, name + "_" + IntToString(pScreenCollection.size()), active);
	}
}

void ScreenHandler::Init()
{
	pCurrentSelectedScreen = nullptr;
	this->pPreviousSelectedScreen = nullptr;
	pScreenCollection = std::map<string, std::shared_ptr<Screen>>();
}

void ScreenHandler::Render( SDL_Renderer& renderer)
{
	for( auto it = pScreenCollection.rbegin(); it != pScreenCollection.rend(); ++it )
	{
		if( (*it).second->isActive() )
			RenderActiveScreen( (*it).second, renderer );
	}

	//if(this->pCurrentSelectedScreen != nullptr)
	//	RenderActiveScreen(this->pCurrentSelectedScreen, renderer);	//double rendering
}
void ScreenHandler::RenderActiveScreen( std::shared_ptr<Screen> scr, SDL_Renderer& renderer)
{
	scr->render(renderer);
}

void ScreenHandler::HandleEvent( SDL_Event& e )
{
	Coord mousePos;
	SDL_GetMouseState( &mousePos.x, &mousePos.y );
	SwitchScreens( mousePos );
	if (pCurrentSelectedScreen.get())
		pCurrentSelectedScreen->HandleEvent(e);
}

void ScreenHandler::SwitchScreens( const Coord& mousePos )
{

	if( GetSelectedScreen() != nullptr && GetSelectedScreen()->isActive() && GetSelectedScreen()->PointWithinScreen(mousePos) ) //mouse still within screen
		return;

	//pCurrentSelectedScreen = nullptr;
	this->SelectNewScreen(SelectNewScreen(mousePos));
	
}
std::shared_ptr<Screen> ScreenHandler::SelectNewScreen(const Coord& mousePos)
{
	//std::shared_ptr<Screen> result = nullptr;
	for( auto it = pScreenCollection.begin(); it != pScreenCollection.end(); ++it )
	{
		if( (*it).second->isActive() )
		{
			if( (*it).second->PointWithinScreen(mousePos) )
			{
				return (*it).second;
			}
		}
	}
	return nullptr;
}

std::shared_ptr<Screen> ScreenHandler::SelectNewScreen(string name)
{
	auto screen = pScreenCollection[name];
	if(screen->isActive())
		this->SelectNewScreen(screen);
	return this->pCurrentSelectedScreen;
}

void ScreenHandler::SelectNewScreen(std::shared_ptr<Screen> screen)
{
	this->pPreviousSelectedScreen = this->pCurrentSelectedScreen;
	this->pCurrentSelectedScreen = screen;
}

void ScreenHandler::DeselectCurrentScreen()
{
	this->pPreviousSelectedScreen = this->pCurrentSelectedScreen;
	this->pCurrentSelectedScreen = nullptr;
}


//void ScreenHandler::HandleActiveScreen( std::shared_ptr<Screen> scr, const Coord& mousepos)
//{
//	if( scr->PointWithinScreen(mousepos) )
//	{
//		pCurrentSelectedScreen = scr;
//	}
//}

void ScreenHandler::CloseAllScreens()
{
	for (auto it = pScreenCollection.begin(); it != pScreenCollection.end(); ++it)
	{
		if ((*it).second->isActive())
		{
			(*it).second->CloseScreen();
		}
	}
	pCurrentSelectedScreen = nullptr;
	pPreviousSelectedScreen = nullptr;
}

void ScreenHandler::OpenAllScreens()
{
	for (auto it = pScreenCollection.begin(); it != pScreenCollection.end(); ++it)
		if ((*it).second->isActive())
			(*it).second->OpenScreen();
}

void ScreenHandler::DeactivateScreen(string name)
{
	std::shared_ptr<Screen> scr = GetScreenByName(name);
	if (scr.get())
	{
		scr->CloseScreen();
		if (pCurrentSelectedScreen == scr)
			DeselectCurrentScreen();
	}
}

void ScreenHandler::ResetAllScreens()
{
	for (auto it = pScreenCollection.begin(); it != pScreenCollection.end(); ++it)
		if ((*it).second->isActive())
			(*it).second.reset();
	pScreenCollection.clear();
}

void ScreenHandler::ResetAllScreensExceptScreen(string name)
{
	for (auto it = pScreenCollection.begin(); it != pScreenCollection.end(); ++it)
		if ((*it).first != name && (*it).second->isActive())
			(*it).second.reset();
	pScreenCollection.clear();
}

void ScreenHandler::ResetCurrentSelectedScreen()
{
	if (pScreenCollection.size() <= 1)
		this->ResetAllScreens();
	else
	{
		for (auto it = pScreenCollection.begin(); it != pScreenCollection.end(); ++it)
			if (pCurrentSelectedScreen == (*it).second)
			{
				(*it).second.reset();
				pScreenCollection.erase(it);
				SelectNewScreen((*pScreenCollection.begin()).second);
				return;
			}
	}

}

std::shared_ptr<Screen> ScreenHandler::GetScreenByName( string name )
{
	auto it = pScreenCollection.find(name);
	if (it != pScreenCollection.end())
		return this->pScreenCollection[name];
	return nullptr;
}

std::shared_ptr<Screen> ScreenHandler::GetScreenByName(string name, bool searchPrefixes)
{
	if (searchPrefixes == false)
		return GetScreenByName(name);
	else
	{
		for (auto it = pScreenCollection.begin(); it != pScreenCollection.end(); ++it)
			if (it->first.find(name) != string::npos)
				it->second;
	}
	return nullptr;
}

Screen* ScreenHandler::GetSelectedScreen()
{
	return pCurrentSelectedScreen.get();
}

bool ScreenHandler::HasScreenSelection()
{
	return pCurrentSelectedScreen.get() && pCurrentSelectedScreen->isActive();
}

