#pragma once
#include "Widget.h"
#include"Graphics.h"
#include"Style.h"

class SurfaceButton : public Widget
{

public:

	SurfaceButton();
	SurfaceButton(string id, int x, int y, ButtonStyle style);
	SurfaceButton(string id, Coord pos, ButtonStyle style);

	void Refresh();
	void RefreshText();
	void RefreshText(const string& newText);
	string GetText();
	ColorRGB GetTextColor();
	bool ChangeText(const Text& newText);
	bool ChangeText(const string& newText);
	bool ChangeText(const ColorRGB& color);

	void Resize(Coord size) override;

	std::shared_ptr<Mix_Chunk> GetHoverEffect();
	std::shared_ptr<Mix_Chunk> GetClickEffect();

	~SurfaceButton();
	virtual void FreeMemory();
	const SDL_Rect* GetClipping();
	void HandleEvent(SDL_Event & event);

protected:
	
	virtual SDL_Surface* Get_WS_OFF_Surface(SDL_Rect rect);
	virtual SDL_Surface* Get_WS_HOVER_Surface(SDL_Rect rect);
	virtual SDL_Surface* Get_WS_ACTIVE_Surface(SDL_Rect rect);
	virtual SDL_Surface* Get_WS_ON_Surface(SDL_Rect rect);
	virtual SDL_Surface* Get_WS_NOT_ENABLED_Surface(SDL_Rect rect);
	virtual SDL_Surface* Get_WS_HIDDEN_Surface(SDL_Rect rect);

	void SetState(WIDGET_STATE);
	virtual void InitializeSoundEffects();
	
private:
	
	bool pCurrentlyRefreshing;
	bool pRefreshOnlyText;

	ButtonStyle pButtonStyle;
	std::shared_ptr<Mix_Chunk> _buttonHover;
	std::shared_ptr<Mix_Chunk> _buttonClick;

	SDL_Surface* GetSurfaceFromState( int i, SDL_Rect rect );
	SDL_Surface* GetSurfaceFromState(WIDGET_STATE);
	SDL_Surface* GetSurfaceFromState(WIDGET_STATE, bool refreshTextOnly);
	
	void GenerateSurface(bool refreshTextOnly );
	void Initialize();



};

//
//class BoxedButton : public SurfaceButton
//{
//
//public:
//	BoxedButton(const char* text, TextSize, Coord pos, int w, int h, void (*)(SDL_Event&, Widget*));
//	BoxedButton( const char* text, const imagehandling::IMGToSurface* image, Coord pos, void (*)(SDL_Event&, Widget*));
//	//void DrawText();
//
//protected:
//
//	SDL_Surface* Get_WS_OFF_Surface(SDL_Rect rect) override;
//	SDL_Surface* Get_WS_HOVER_Surface(SDL_Rect rect) override;
//	SDL_Surface* Get_WS_ACTIVE_Surface(SDL_Rect rect) override;
//	SDL_Surface* Get_WS_ON_Surface(SDL_Rect rect) override;
//	SDL_Surface* Get_WS_NOT_ENABLED_Surface(SDL_Rect rect) override;
//	SDL_Surface* Get_WS_HIDDEN_Surface(SDL_Rect rect) override;
//
//private:
//	//std::shared_ptr<SDL_Surface> CreateButtonBackground(SDL_Rect rect);
//	void initialize(SDL_Rect &, const char*, SDL_Color textcolor);
//	void initialize( const imagehandling::IMGToSurface* image );
//};
