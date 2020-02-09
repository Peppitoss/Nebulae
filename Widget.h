//#pragma once
#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <memory>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Wrapper.h"
#include <string>
#include <utility>
#include "Graphics.h"
#include<functional>
#include"ClickAction.h"


/////////////////////////////////////////////////
//											   //
//			- Courtesy of S.G				   //
//											   //
/////////////////////////////////////////////////

//
//  Widget Definition
// The base class for all widgets.


enum WIDGET_MODE 
{
	WL_BACKGROUND,
	WL_NON_INTERACTIVE,
	WL_INTERACTIVE,
	WL_COUNT
};

enum WIDGET_STATE
{
	WS_OFF,		//mouse not inside
	WS_HOVER,	//hover
	WS_ACTIVE,	//button clicked
	WS_ON,		//clicked and hovering
	WS_NOT_ENABLED,
	WS_HIDDEN,	
	WS_COUNT
};

string StateToString(WIDGET_STATE enstate);

class Widget
{
private:

	double pZoomModifier;
	string pID;
	bool pUseToggleClick;
	bool pWidgetInteriorClicked;
	bool pStateDependOnBinding;

	std::shared_ptr<Widget> pBindingWidget;//shares state with object in list

protected:

	//bool _hovered;

	WIDGET_STATE mState;
	WIDGET_MODE mMode;
	std::shared_ptr<Mix_Chunk> mSoundByte;
	std::shared_ptr<SDL_Texture> mTexture;
	std::shared_ptr<SDL_Surface> mSurface;
	SDL_Rect mClipping;
	SDL_Rect mBoundaryBox;
	//SDL_Color mTextColor;
	//std::string GetFullText() const;
	//std::vector<std::string> mText;
	void SetClipping(const SDL_Rect&);
	void SetBoundaryBox(const SDL_Rect&);

	void UpdateStateInside(SDL_Event & event);
	void UpdateStateOutside(SDL_Event & event);

	virtual void UpdateState(SDL_Event & event);
	virtual bool isInside(int xMouse, int yMouse);
	virtual void SetState(WIDGET_STATE, bool updateToBinding);
	virtual void SetState(WIDGET_STATE);

	void UpdateStateToBindings();

	Coord mOffset;
	Coord mOrigoPosition;
	ClickAction pClickAction;

public:
	Widget();
	Widget(string id, SDL_Surface* surface_arg, SDL_Rect& rect);
	Widget(string id, SDL_Surface* surface_arg, int x, int y, int w, int h);
	Widget(const Widget&);
	Widget(Widget&& rhs);
	virtual Widget& operator=(Widget&& rhs);
	virtual Widget& operator=(const Widget&);
	virtual const std::shared_ptr<SDL_Surface> GetSurface();
	virtual const std::shared_ptr<SDL_Texture> GetTexture();
	virtual const std::shared_ptr<SDL_Texture> Widget::GetTexture(SDL_Renderer& renderer);
	//virtual const Mix_Chunk* GetSound();
	virtual const SDL_Rect* GetClipping();
	virtual const SDL_Rect* GetBoundaryBox();
	Coord GetSize() const { return Coord(mBoundaryBox.w, mBoundaryBox.h); }
	//virtual const TTF_Font* GetFont();

	void SetZoomingModifier(double set);// { this->pZoomModifier = set; }
	double GetZoomModifier() const { return this->pZoomModifier; }
	bool IsDrawingZoomIndependent() const { return this->pZoomModifier != 1; }

	void SetWidgetOff() { this->pStateDependOnBinding = false; SetState(WS_OFF); };
	void SetWidgetActive() { this->pStateDependOnBinding = false; SetState(WS_ACTIVE); };
	void SetWidgetClicked() { this->pStateDependOnBinding = false; SetState(WS_ON); };
	//void SetSelected(WIDGET_STATE selectedstate, WIDGET_STATE unselectedstate, bool value);

	virtual void Activate(SDL_Event& e);
	virtual void Deactivate(SDL_Event& e);
	void SetClickAction(ClickAction action) { this->pClickAction = action; }
	ClickAction GetClickAction() const { return this->pClickAction; }

	void FreeMemory();
	WIDGET_STATE GetState();
	void Render(SDL_Renderer& renderer);
	void Render(SDL_Renderer& renderer, Coord offset);
	void Render(SDL_Texture& windowTexture);
	void Render(SDL_Renderer* renderer, const Coord& pos, const SDL_Rect& Clipping);

	virtual void HandleEvent(SDL_Event&);
	virtual void Resize( Coord size );
	void SetPosition( Coord pos );
	Coord GetPosition() { return Coord( mBoundaryBox.x, mBoundaryBox.y); };
	Coord GetWidgetCenterPosition();
	void SetOrigoPosition( Coord pos ) { mOrigoPosition = pos;};
	void SetWidgetMode( WIDGET_MODE mode ) { this->mMode = mode; };

	virtual void WidgetClicked(SDL_Event & e);
	void SetUseToggleClick( bool set ) { this->pUseToggleClick = set; }
	bool UseToggleClick() { return this->pUseToggleClick;}

	string ID() const { return this->pID; }
	void SetID(string ID) { this->pID = ID; }
	virtual SDL_Texture* generateTexture(SDL_Renderer & renderer);
	void AddLinkToBindingList(std::shared_ptr<Widget>);

	void disable();
	void enable();
	void TriggerOnClickedAction();
	void enableON();
	void hide();
	void sethovered();
	void show();
	void moveTo(int xPos, int yPos)
	{
		mBoundaryBox.x = xPos;
		mBoundaryBox.y = yPos;
	}
	virtual ~Widget(void);
	std::function<void(Widget* w)> OnWidgetClicked;
	std::function<void(Widget* w)> OnWidgetClick;
	std::function<void(Widget* w)> OnWidgetHovered;
	std::function<void(Widget* w)> OnWidgetDisabled;
	std::function<void(Widget* w)> OnWidgetSelected;
	//virtual void SetText(const std::string text);// { mText = text; };

	//void UpdateStateToggle(SDL_Event & e);
};

#endif