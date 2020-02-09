#pragma once
#include"ScreenBackground.h"
#include"StyleFactory.h"
#include"ScreenController.h"
typedef std::shared_ptr<Mix_Music>  ScreenMusic;
//int DEFAULT_SCREEN_HEIGHT = 800;
//int DEFAULT_SCREEN_WIDTH  = 600;


enum enCampaignPlanets
{
	Sun = 0,
	Mercury = 1,
	Venus = 2,
	Earth = 3,
	Mars = 4,
	NoPlanet = 100

};



class Widget;
//void CloseTablet(SDL_Event& e, Widget* button);

class Screen
{

public:

	Screen();
	Screen(Coord pos, Coord size, const imagehandling::IMGToSurface*);
	Screen(Coord pos, Coord size);
	Screen(const imagehandling::IMGToSurface*);

	enScreenState GetScreenState() { return this->pController.GetState(); }
	void OpenScreen() { this->pController.SetScreenState(enScreenState::SCREEN_ACTIVE, true); }
	void CloseScreen() { this->pController.SetScreenState(enScreenState::SCREEN_CLOSED,true); }

	void RenderLetter(char letter, Coord position, SDL_Renderer&);
	void RenderLetter(std::string letter, Coord position, SDL_Renderer&);
	void RenderTexture(SDL_Texture*, Coord position, SDL_Renderer&);

	virtual void render(SDL_Renderer&);
	virtual void RenderWidget(std::shared_ptr<Widget> widget, SDL_Renderer&);
	virtual void HandleEvent(SDL_Event& e);
	
	inline ScreenMusic GetMusic() { return _music; }

	virtual void SetMusic(std::string music);
	virtual void ClearMusic();

	virtual void SetBackground(std::string image, EnumColorKey colorKey, double opacity);
	virtual void SetBackground(std::string image, EnumColorKey colorKey);
	virtual void SetBackground(const imagehandling::IMGToSurface* surf, double opacity);

	virtual void AddBackgroundLayer(const imagehandling::IMGToSurface*, double opacity);
	virtual void AddBackgroundLayer(const imagehandling::IMGToSurface*, const Coord& position, double opacity);

	virtual void AddBackgroundLayer(std::string image, EnumColorKey colorKey, double opacity);

	bool isActive() { return this->GetScreenState() == enScreenState::SCREEN_ACTIVE; };
	bool PointWithinScreen(const Coord point);
	bool PointWithinScreen(const Coord point, bool RequireThatScreenIsActive);

	Coord GetPosition() const { return screenPosition; };
	void SetPosition(const Coord& set) { this->screenPosition = set; }

	Coord CenterScreenPositionBasedOnBackgroundImage();
	Coord CenterScreenPositionBasedOnBackgroundImage(const Coord& compareSize);

	int Width() const { return this->pScreenWidth; }
	int Height() const { return this->pScreenHeight; }
	Coord size() const { return Coord(Width(), Height()); };

	//Remove in future, replace by using callbacks in screenController
	virtual void HandleActiveWidget(SDL_Event& e, Widget* widget);
	virtual void HandleWidgetByState(SDL_Event& e);

	virtual ~Screen(void);

protected:
	
	ScreenController* Control() { return &this->pController; }
	ScreenBackground pBackground;
	//std::shared_ptr<SDL_Texture> mTexture; // background texture
	ScreenMusic _music;

	Coord screenPosition;
	//SDL_Texture* Screen::generateTexture(SDL_Renderer & renderer);
	int pScreenHeight;
	int pScreenWidth;
	bool pStretchBackgroundTextures;

	virtual void OnOpenScreen();
	virtual void SelectWidgetByIndex(SDL_Event& e, int index);
	void RenderBackground(SDL_Renderer& renderer);
	void RenderBackground(SDL_Renderer& renderer, const Coord& position);

	int pIndexOfActiveWidget;

private:

	ScreenController pController;

	void Initialize();
	void SetSelectedWidgetIndex(int index);

};

