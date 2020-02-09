#pragma once
#include<set>
#include<map>
#include<string>
#include"IMGToSurface.h"
#include"Widget.h"
#include"Text.h"

using std::string;
using namespace primitives;

extern const int STANDARD_BUTTON_HEIGHT;
typedef std::map < WIDGET_STATE, WIDGET_STATE> StatePermutation;

enum enTextPosition
{
	noTxtPosition = 0,
	centerTxt = 1,
	horizontalCenterTxt = 2,
	upperLeftTxt = 4,
	upperRightTxt = 5,
	lowerLeftTxt = 6,
	lowerRightTxt = 7
};

struct TextStyleProperties
{
	int size;
	string Font;
	Text pText;
	int MaxTextLenght;
	int NewLineDistance;
	int Margin;
	enTextPosition enPosition;
	bool CropText;
	Coord TextDrawOffset;
	ColorRGB TextColor;
};

struct TextStyle
{
	TextStyle();

	TextStyleProperties Properties;

	//Coord GetTextDimension();
	Coord GetTextPositionFromEnum(const Coord& boundary);
	Coord GetTextPositionFromEnum(const Coord& boundary, std::vector<std::string>& );
	Coord GetSizeFromText(int MaxWidth);
	std::string GetLongestString(const std::vector<std::string>&);
	Coord TextStyle::GetTextDimensions(std::string text);
	std::vector<std::string> SplitTextToFitBoundary(int boundaryWidth);

	std::shared_ptr<SDL_Surface> GetNewSurface(const Coord& boundary);
	std::shared_ptr<SDL_Surface> Surface;

	Coord Boundary;
	Coord TextPosition;
};

struct BorderStyle
{
	BorderStyle();

	ColorRGB BorderColor;
	int BorderThickness;

	imagehandling::IMGToSurface BorderTxt;	//has all edges of the border in a 2x2 format
	imagehandling::IMGToSurface SideTxt;

	std::shared_ptr<SDL_Surface> GetNewSurface(const Coord& boundary);
	std::shared_ptr<SDL_Surface> Surface;

	bool UseTextures;

	int GetSize() const { return this->BorderTxt.getSize()/4; }

};

struct WindowGUI
{
	WindowGUI();
	WindowGUI(Coord dimension);
	WindowGUI(const imagehandling::IMGToSurface* backgroundTexture, const Coord& dimension, const Coord& cropPosition);

	WindowGUI(const WindowGUI&);

	WindowGUI& operator=(const WindowGUI&);

	ColorRGB BackgroundColor;
	BorderStyle Border;
	bool UseBorder;

	std::shared_ptr<SDL_Surface> GetNewSurface( const Coord& boundary );
	std::shared_ptr<SDL_Surface> Surface;

	void ResetValues();
	const imagehandling::IMGToSurface* pPremade;
	Coord Dimension;
	Coord CropPosition;
	SDL_RendererFlip enFlip;

};

class WindowStyle
{
public:
	WindowStyle();
	WindowStyle(WindowGUI);

private:

	WindowGUI pGUI;
};



class ButtonStyle
{

public:

	ButtonStyle();
	ButtonStyle( Coord dimension );

	static ButtonStyle NewStyle(Coord dimension);
	

	ButtonStyle& AddButtonState(WIDGET_STATE);
	ButtonStyle& AddTextDefinition(WIDGET_STATE, TextStyle);
	ButtonStyle& AddTextDefinition(TextStyle);
	ButtonStyle& AddWindowGUI(WIDGET_STATE, WindowGUI);

	//generates new surface for all styles, don't use often!
	SDL_Surface* GetNewSurface(WIDGET_STATE);
	SDL_Surface* GetNewSurface(WIDGET_STATE, bool refreshtxt, bool refreshbtn );

	std::shared_ptr<SDL_Surface> GetText(WIDGET_STATE, bool refresh);
	std::shared_ptr<SDL_Surface> GetButton(WIDGET_STATE, bool refresh);

	bool HasState(WIDGET_STATE state);
	int GetStatesCount() const { return pUsedStates.size(); }
	WIDGET_STATE GetStateFromPermutation(WIDGET_STATE state);
	WIDGET_STATE GetState_Safe(WIDGET_STATE);

	TextStyleProperties GetTextProperties(int index);
	TextStyleProperties GetTextProperties(WIDGET_STATE state, int index);
	bool ChangeTextStyle(Text text);
	bool ChangeTextStyle(ColorRGB color);
	bool ChangeTextStyle(string text);

	//triggers redraw of all surfaces
	void ChangeDimension(const Coord& newDimension);
	Coord const  Dimension() { return Coord(pDimension.w, pDimension.h); }
	
	WIDGET_STATE GetStatePermutation(WIDGET_STATE state) { return this->pStatePermutation[state]; }
	void DefineStatePermutation(std::map < WIDGET_STATE, WIDGET_STATE> set) { this->pStatePermutation = set; }

	bool SetAutoSize(bool value) { this->pAutosize = value; }
	bool Autosize() const { return this->pAutosize; }

	void SetDrawText(bool set) { this->pDrawText = set; }
	bool DrawText() const { return pDrawText; }

	void SetColorKey(ColorRGB set) { this->pColorKey = set; }
	ColorRGB GetColorKey() const { return this->pColorKey; }

	std::set<WIDGET_STATE>::iterator const UsedStatesBegin() { return pUsedStates.begin(); };
	std::set<WIDGET_STATE>::iterator const UsedStatesEnd() { return pUsedStates.end(); };

private:

	SDL_Rect pDimension;
	bool pAutosize;
	bool pDrawText;
	ColorRGB pColorKey;

	std::set<WIDGET_STATE> pUsedStates;
	std::map<WIDGET_STATE, std::vector<TextStyle>> pTextDefinition;
	std::map<WIDGET_STATE, WindowGUI> pGUI;

	WIDGET_STATE textState;	//only one text for entire button style (this is to keep things simple currently) might generalize one text for one surface later, but this will complicate renewal of texts

	std::shared_ptr<SDL_Surface> GetText(WIDGET_STATE, bool refresh, int index);
	StatePermutation pStatePermutation = std::map < WIDGET_STATE, WIDGET_STATE>();

	void Initialize();
	void AutoResize();

};

