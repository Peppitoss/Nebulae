#pragma once
#include"Screen.h"

//used to handle a collection of screens such that they can communicate with one another

using std::string;

class ScreenHandler
{

public:

	ScreenHandler();

	void AddScreen( std::shared_ptr<Screen> , string name);
	void AddScreen(std::shared_ptr<Screen>, string name, bool active);
	void AddScreen(std::shared_ptr<Screen>, string name, bool active, bool AllowDuplicateNames);
	virtual void Render( SDL_Renderer&);
	virtual void HandleEvent( SDL_Event& e );

	Screen* GetSelectedScreen();
	std::shared_ptr<Screen> GetScreenByName( string name );
	std::shared_ptr<Screen> GetScreenByName(string name, bool searchPrefixes );

	virtual void DeactivateScreen(string name);
	void CloseAllScreens();
	virtual void OpenAllScreens();
	void ResetCurrentSelectedScreen();
	void ResetAllScreens();
	void ResetAllScreensExceptScreen(string name);

	bool HasScreenSelection();
	void SetScreenState( std::shared_ptr<Screen>, enScreenState ); //make this

	std::map<string,std::shared_ptr<Screen>>::const_iterator const begin() const  { return pScreenCollection.begin(); };
	std::map<string,std::shared_ptr<Screen>>::const_iterator const end() const  { return pScreenCollection.end(); };

protected:

	void Init();
	std::shared_ptr<Screen> pCurrentSelectedScreen;
	std::shared_ptr<Screen> pPreviousSelectedScreen;

	virtual void SwitchScreens(const Coord& mousePos);
	std::shared_ptr<Screen> SelectNewScreen(const Coord& mousePos);
	std::shared_ptr<Screen> SelectNewScreen(string name);

	void SelectNewScreen(std::shared_ptr<Screen> screen);
	void DeselectCurrentScreen();

private:

	//void HandleActiveScreen( std::shared_ptr<Screen>, const Coord& mousepos );
	void RenderActiveScreen( std::shared_ptr<Screen>, SDL_Renderer& renderer );

	std::map<string,std::shared_ptr<Screen>> pScreenCollection;

};