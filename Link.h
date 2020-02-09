#pragma once
#include"SurfaceButton.h"
#include"Graphics.h"
#include"Wrapper.h"
#include "Globals.h"

using std::string;

enum enLinkBorderStyle
{
	NoBorder = 0,
	WholeBorder = 1,
	EdgeBorders = 2
};

enum enBorderFlags
{
	BorderFlagOff = 0,
	BorderOnStandard = 1,
	BorderOnHover = 2,
	BorderOnSelect = 4
};

inline enBorderFlags operator|(enBorderFlags a, enBorderFlags b)
{
	return static_cast<enBorderFlags>(static_cast<int>(a) | static_cast<int>(b));
}
//
//enum enumLinkText
//{
//	CLICKME = 0,
//	DOCKINGINITIATED,
//	DOCKEDDONE,
//
//};

enum enLinkType
{
	LINKSTANDARD = 0,
	LINKHUDBUTTON = 1,
	LINKTARGETSELECTED = 2,
	LINKHOTKEYSELECT = 3,
	LINKDIALOG = 4,
	LINKDOCKBUTTON = 5
};

//const extern string standardLinkFont;

class Link : public SurfaceButton
{
public:

	Link();
	Link(Text text, TextSize, Coord position, int h, int w );
	Link(Text text, TextSize, Coord position, int h, int w, ColorRGB& textColor, ColorRGB& bColor);
	Link(Text text, const char* fontName, TextSize, Coord position, int h, int w );
	Link(Text text, const char* fontName, TextSize, Coord position, int h, int w, const ColorRGB& textColor, enTextPosition enPos, bool OnlyOneState);

	Link( Link& );
	Link( Link&& );

	Link& operator=(Link&& src);
	Link& operator=(Link& src);

	void UpdateState(SDL_Event & event) override;
	void virtual HandleLinkIsSelected(SDL_Event & event);
	virtual void UpdateStateAddition(SDL_Event & event);

	void SetOrigoPosition( const Coord& newOrigo ) { this->mOrigoPosition = newOrigo; };
	void RenewText( std::string text);
	void RenewText(std::string text, SDL_Color newColor);
	void RenewText(SDL_Color newColor);

	void SetState(WIDGET_STATE) override;
	void SetSelected( bool value );
	bool CheckIfLinkIsSelected();

	void Deactivate(SDL_Event& e) override;
	void Activate(SDL_Event& e) override;

	int GetIntState() const { return this->pLinkNumericState; };
	void SetIntState(int set) { this->pLinkNumericState = set; };

	//virtual enLinkType GetType();
	std::string GetFunctionName() const { return this->pCallbackFunctionName; }

	bool IsInside(int mouseX, int mouseY, bool includeZoomModifier, bool IncludeOrigoPosition);
	void FadeText(double Opacity);
	SDL_Texture* generateTexture(SDL_Renderer & renderer, double alpha);


	~Link();

protected:


	Coord virtual TextDrawPosition();
	void SetCallbackFunctionName(std::string set) { this->pCallbackFunctionName = set; };

	//virtual SDL_Surface* GetStandardSurface(SDL_Rect rect);
	//virtual SDL_Surface* GetSelectionSurface(SDL_Rect rect);
	//virtual SDL_Surface* GetHoverSurface(SDL_Rect rect);

	//SDL_Surface* Get_WS_OFF_Surface(SDL_Rect rect) override;
	//SDL_Surface* Get_WS_HOVER_Surface(SDL_Rect rect) override;
	//SDL_Surface* Get_WS_ACTIVE_Surface(SDL_Rect rect) override;
	//SDL_Surface* Get_WS_ON_Surface(SDL_Rect rect) override;
	//SDL_Surface* Get_WS_NOT_ENABLED_Surface(SDL_Rect rect) override;
	//SDL_Surface* Get_WS_HIDDEN_Surface(SDL_Rect rect) override;

private:

	WIDGET_STATE ActiveState() { return WIDGET_STATE::WS_ACTIVE; };
	WIDGET_STATE DeactiveState() { return WIDGET_STATE::WS_OFF; };

	int pLinkNumericState;	//used as sa numeric code for the display text, simple way to enable text comparison without using string comparison codes

	
	//Coord GetTextDimensions( std::string );
	void Initialize();
	
	//std::string GetLongestString( const std::list<std::string>& );
	SDL_Surface* AddBorder(SDL_Surface* surface, enBorderFlags, SDL_Rect& rect);

	//ColorRGB pSelectionColor;
	//std::map<enBorderFlags,ColorRGB> pBorderColor;
	//ColorRGB GetColorFromBorder(enBorderFlags);

	bool pSelected;
	enLinkBorderStyle pBorderStyle;
	enBorderFlags pBorderFlag;

	//int pNewLineDistance;
	//int pTextMargin;
	std::string pCallbackFunctionName;
	
};

class LinkAttachment
{
public:
	LinkAttachment()
	{
		link = nullptr;
		RelativePosition = Coord(0,0);
	}

	LinkAttachment(const LinkAttachment& other);
	LinkAttachment(LinkAttachment&& other);

	LinkAttachment& LinkAttachment::operator=(const LinkAttachment& other);
	LinkAttachment& LinkAttachment::operator=(const LinkAttachment&& other);

	std::shared_ptr<SurfaceButton> link;
	Coord RelativePosition;

	std::shared_ptr<SurfaceButton> generateLink( Coord relativePosition, Coord globalPosition, std::string text, enLinkType type );
	std::shared_ptr<SurfaceButton> generateLink( Coord relativePosition, Coord globalPosition, std::string text, enLinkType type, string callbackname );

	void ChangeText( std::string text );
	std::string GetTextID() const { link->ID(); };
	
	void setRelativePosition(const Coord& set); //OPS doesn't set a new position for link, in that case use setAttachmentposition :)
	void setAttachmentposition(const Coord& set);

	void Resize( Coord size );

	void clearMemory();
	~LinkAttachment() {  clearMemory();  };

};