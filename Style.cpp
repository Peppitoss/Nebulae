#include"Style.h"
#include"IMGToSurface.h"
#include"Wrapper.h"
#include"Debug.h"

const int STANDARD_BUTTON_HEIGHT = 50;


TextStyle::TextStyle()
{
	this->Properties = TextStyleProperties();
	Properties.size = 12;
	Properties.Font = standardLinkFont;
	Properties.pText = Text();
	Properties.pText.SetCurrentValue("button");
	Properties.MaxTextLenght = 1000;
	Properties.NewLineDistance = 4;
	Properties.enPosition = enTextPosition::centerTxt;
	Properties.TextDrawOffset = 0;
	Properties.TextColor = ColorRGB::White();
	Properties.CropText = false;

	Boundary = Coord(0);
	TextPosition = Coord(0);
}

//Coord TextStyle::GetTextDimension()
//{
//	Coord dimension;
//	std::list<std::string> newLines = primitives::SplitNewLine(Properties.pText.GetCurrentValue());
//
//	for (auto it = newLines.begin(); it != newLines.end(); ++it)
//	{
//		Coord linedim = GetTextDimensions(Properties.pText.GetCurrentValue()).xadd(Properties.Margin);
//		dimension.y += linedim.y + Properties.NewLineDistance;
//		dimension.x = std::fmax(dimension.x, linedim.x);
//	}
//	return dimension;
//}

std::vector<std::string> TextStyle::SplitTextToFitBoundary(int boundaryWidth)
{
	std::vector<std::string> result = std::vector<std::string>();
	if (this->Properties.CropText)
		result = primitives::SplitNewLine(Properties.pText.GetCurrentValue(), Properties.size, Properties.Font, boundaryWidth);
	else
		result = primitives::SplitNewLine(Properties.pText.GetCurrentValue());
	return result;
}

std::shared_ptr<SDL_Surface> TextStyle::GetNewSurface(const Coord& boundary)
{
	int width = boundary.x;
	int height = boundary.y;
	

	std::vector<std::string> newLines = SplitTextToFitBoundary(width);

	SDL_Surface * textSurface = sdlWrapper.CreateSurface(width, height, sdlWrapper.GetSDLColor(COLOUR::C_DEFAULT));
	int lineHeight = this->GetTextDimensions(Properties.pText.GetCurrentValue()).y;

	int newLineCounter = 0;
	if (this->Boundary.x != boundary.x || this->Boundary.y != boundary.y)
	{
		this->Boundary = boundary;
		this->TextPosition = GetTextPositionFromEnum(boundary, newLines);
	}

	for (auto it = newLines.begin(); it != newLines.end(); ++it)
	{
		if ((*it) == "")
			continue;

		int drawHeight = this->TextPosition.y + newLineCounter*lineHeight;
		SDL_Surface * textlineSurface = sdlWrapper.CreateTextSurface((*it).c_str(), Properties.size, Properties.Font, this->Properties.TextColor.ToSDL_Color(), TTF_STYLE_BOLD);
		SDL_BlitSurface(textlineSurface, NULL, textSurface, &sdlWrapper.CreateRect(this->TextPosition.x, drawHeight, width, height));
		Debug::SDLErrorCheck();
		newLineCounter++;
		SDL_FreeSurface(textlineSurface);
	}

	this->Surface = std::shared_ptr<SDL_Surface>(textSurface, SDL_FreeSurface);
	return this->Surface;
}

Coord TextStyle::GetTextPositionFromEnum(const Coord& boundary)
{
	std::vector<std::string> newLines = SplitTextToFitBoundary(boundary.x);
	return GetTextPositionFromEnum(boundary, newLines);
}

Coord TextStyle::GetTextPositionFromEnum(const Coord& boundary, std::vector<std::string>& textLines)
{
	Coord result;
	int lineHeight = boundary.y / textLines.size();

	switch (Properties.enPosition)
	{
	case(enTextPosition::centerTxt):
		result.x = boundary.x / 2 - this->GetTextDimensions(GetLongestString(textLines)).x / 2;
		result.y = std::fmax(0, boundary.y / 2 - (this->GetSizeFromText(boundary.x).y) / 2);
		break;
	case(enTextPosition::horizontalCenterTxt):
		result.x = Properties.TextDrawOffset.x - this->GetTextDimensions(GetLongestString(textLines)).x / 2;
		result.y = Properties.TextDrawOffset.y;
		break;
	case(enTextPosition::lowerLeftTxt):
		break;
	case(enTextPosition::lowerRightTxt):
		break;
	case(enTextPosition::noTxtPosition):
		result = TextPosition;
		break;
	case(enTextPosition::upperLeftTxt):
		result.x = Properties.TextDrawOffset.x;
		result.y = Properties.TextDrawOffset.y;
		break;
	case(enTextPosition::upperRightTxt):
		break;
	}
	return result;
}

Coord TextStyle::GetSizeFromText(int MaxWidth)
{
	Coord result = Coord(0);
	std::vector<std::string> newLines = SplitTextToFitBoundary(MaxWidth);
	int newLineCounter = 0;

	for (auto it = newLines.begin(); it != newLines.end(); ++it)
	{
		Coord dimension = GetTextDimensions(*it).xadd(Properties.Margin);
		result.y += dimension.y;
		if (newLineCounter++ > 0)
			result += Properties.NewLineDistance;

		result.x = std::fmax(result.x, dimension.x);
	}
	return result;
}

//Coord TextStyle::GetTextDimension()
//{
//	Coord dimension;
//	std::list<std::string> newLines = primitives::SplitNewLine(Properties.pText.GetCurrentValue());
//
//	for (auto it = newLines.begin(); it != newLines.end(); ++it)
//	{
//		Coord linedim = GetTextDimensions(Properties.pText.GetCurrentValue()).xadd(Properties.Margin);
//		dimension.y += linedim.y + Properties.NewLineDistance;
//		dimension.x = std::fmax(dimension.x, linedim.x);
//	}
//	return dimension;
//}

std::string TextStyle::GetLongestString(const std::vector<std::string>& strList)
{
	int longest = 0;
	std::string result = "";
	for (auto it = strList.begin(); it != strList.end(); ++it)
	{
		int length = this->GetTextDimensions(*it).x;
		if (length > longest)
		{
			longest = length;
			result = *it;
		}
	}
	return result;
}

Coord TextStyle::GetTextDimensions(std::string text)
{
	SDL_Rect dimension = sdlWrapper.GetTextSize(this->Properties.size, this->Properties.Font, text, TTF_STYLE_BOLD);
	return Coord(dimension.w, dimension.h);
}

BorderStyle::BorderStyle()
{
	BorderColor = ColorRGB(0, 0, 0);
	BorderThickness = 1;
	BorderTxt;
	SideTxt;
	UseTextures = false;
}

std::shared_ptr<SDL_Surface> BorderStyle::GetNewSurface(const Coord& boundary)
{
	if (UseTextures)
	{
		SDL_Rect topleftcorner = { 0, 0, this->GetSize(), this->GetSize() };
		SDL_Rect toprightcorner = { boundary.x - this->GetSize(), 0, this->GetSize(), this->GetSize() };
		SDL_Rect bottomleftcorner = { 0, boundary.y - this->GetSize(), this->GetSize(), this->GetSize() };
		SDL_Rect bottomrightcorner = { boundary.x - this->GetSize(), boundary.y - this->GetSize(), this->GetSize(), this->GetSize() };

		SDL_Surface * textSurface = sdlWrapper.CreateSurface(boundary.x, boundary.y, sdlWrapper.GetSDLColor(COLOUR::C_DEFAULT));

		auto bordersurf = BorderTxt.GetSurface();
		auto sidesurf = SideTxt.GetSurface();
		if (bordersurf.get() == false)
			throw new std::exception("missing border surface to generate border style surface");
		if (sidesurf.get() == false)
			throw new std::exception("missing side surface to generate border style surface");

		SDL_BlitSurface(bordersurf.get(), &topleftcorner, textSurface, &topleftcorner);
		Debug::SDLErrorCheck();
		SDL_BlitSurface(bordersurf.get(), &SDLRect(0, this->GetSize(), this->GetSize(), this->GetSize()), textSurface, &toprightcorner);
		Debug::SDLErrorCheck();
		SDL_BlitSurface(bordersurf.get(), &SDLRect(this->GetSize(), 0, this->GetSize(), this->GetSize()), textSurface, &bottomleftcorner);
		Debug::SDLErrorCheck();
		SDL_BlitSurface(bordersurf.get(), &SDLRect(this->GetSize(), this->GetSize(), this->GetSize(), this->GetSize()), textSurface, &bottomrightcorner);
		Debug::SDLErrorCheck();

		SDL_BlitScaled(sidesurf.get(), &topleftcorner, textSurface,		&SDLRect(this->GetSize(), 0, boundary.x - this->GetSize() * 2, this->GetSize()));
		Debug::SDLErrorCheck();
		SDL_BlitScaled(sidesurf.get(), &bottomleftcorner, textSurface,	&SDLRect(this->GetSize(), boundary.y - this->GetSize(), boundary.x - this->GetSize() * 2, this->GetSize()));
		Debug::SDLErrorCheck();
		SDL_BlitScaled(sidesurf.get(), &toprightcorner, textSurface, &SDLRect(this->GetSize(), boundary.x - this->GetSize(), boundary.x - this->GetSize() * 2, this->GetSize()));
		Debug::SDLErrorCheck();

		this->Surface = std::shared_ptr<SDL_Surface>(textSurface, SDL_FreeSurface);
		return Surface;
	}
	else
	{
		SDL_Surface * textSurface = sdlWrapper.CreateSquareBorder( this->BorderColor.ToSDL_Color(), primitives::SDLRect(0,0,boundary.x, boundary.y), BorderThickness);
		this->Surface = std::shared_ptr<SDL_Surface>(textSurface, SDL_FreeSurface);
		return Surface;
	}

}

WindowGUI::WindowGUI()
{
	ResetValues();
}

WindowGUI::WindowGUI(Coord dimension)
{
	ResetValues();
	Dimension = dimension;
	pPremade = nullptr;
}

WindowGUI::WindowGUI(const imagehandling::IMGToSurface* backgroundTexture, const Coord& dimension, const Coord& CropPosition)
{
	ResetValues();
	Dimension = dimension;
	pPremade = backgroundTexture;
	this->CropPosition = CropPosition;
}

WindowGUI::WindowGUI(const WindowGUI& other)
{
	BackgroundColor = other.BackgroundColor;
	Border = other.Border;
	UseBorder = other.UseBorder;
	pPremade = other.pPremade;
	Dimension = other.Dimension;
	CropPosition = other.CropPosition;
	enFlip = other.enFlip;
}

WindowGUI& WindowGUI::operator=(const WindowGUI& other)
{
	BackgroundColor = other.BackgroundColor;
	Border = other.Border;
	UseBorder = other.UseBorder;
	pPremade = other.pPremade;
	Dimension = other.Dimension;
	CropPosition = other.CropPosition;
	enFlip = other.enFlip;
	return *this;
}

void WindowGUI::ResetValues()
{
	BackgroundColor = ColorRGB(100, 100, 100);
	Border = BorderStyle();
	UseBorder = true;
	Dimension = Coord(0);
	CropPosition = Coord(0);
	enFlip = SDL_FLIP_NONE;
}

std::shared_ptr<SDL_Surface> WindowGUI::GetNewSurface(const Coord& boundary)
{
	Dimension = boundary;
	SDL_Surface * textSurface;
	if (pPremade == nullptr)
	{
		SDL_Rect rDimension = primitives::SDLRect(0, 0, Dimension.x, Dimension.y);
		textSurface = sdlWrapper.CreateSquare(this->BackgroundColor.ToSDL_Color(), rDimension);
		if (this->UseBorder)
		{
			std::shared_ptr<SDL_Surface> border = Border.GetNewSurface(Dimension);
			SDL_BlitSurface(border.get(), &rDimension, textSurface, &rDimension);
			Debug::SDLErrorCheck();
		}
	}
	else
	{
		SDL_Rect rDimension = primitives::SDLRect(0, 0, Dimension.x, Dimension.y);
		textSurface = sdlWrapper.CreateSquare(this->BackgroundColor.ToSDL_Color(), rDimension);
		SDL_Rect crop = { this->CropPosition.x, this->CropPosition.y, this->Dimension.x, this->Dimension.y };
		SDL_BlitSurface(pPremade->GenerateSurfaceFromSurface(crop), &rDimension, textSurface, &rDimension);
		Debug::SDLErrorCheck();
		
		if (this->UseBorder)
		{
			std::shared_ptr<SDL_Surface> border = Border.GetNewSurface(Dimension);
			SDL_BlitSurface(border.get(), &rDimension, textSurface, &rDimension);
			Debug::SDLErrorCheck();
		}

		this->Surface = std::shared_ptr<SDL_Surface>(textSurface, SDL_FreeSurface);
		//SDL_SetSurfaceAlphaMod(this->Surface.get(), this->BackgroundColor.alpha());
	}

	if (this->enFlip == SDL_FLIP_HORIZONTAL || this->enFlip == SDL_FLIP_VERTICAL)
	{
		SDL_Surface * flipped = imagehandling::IMGToSurface::flip_surface(textSurface, this->enFlip);
		SDL_FreeSurface(textSurface);
		this->Surface = std::shared_ptr<SDL_Surface>(flipped, SDL_FreeSurface);
	}
	else
		this->Surface = std::shared_ptr<SDL_Surface>(textSurface, SDL_FreeSurface);
	return this->Surface;

}

WindowStyle::WindowStyle()
{
	this->pGUI = WindowGUI();

}

WindowStyle::WindowStyle(WindowGUI gui)
{
	this->pGUI = gui;
}


ButtonStyle::ButtonStyle()
{
	this->pDimension = primitives::SDLRect(0, 0, STANDARD_BUTTON_HEIGHT, STANDARD_BUTTON_HEIGHT);
	Initialize();
}

ButtonStyle::ButtonStyle( Coord dimension )
{
	this->pDimension = primitives::SDLRect(0, 0, dimension.x, dimension.y);
	Initialize();
}

TextStyleProperties ButtonStyle::GetTextProperties( int index)
{
	//WIDGET_STATE safeState = this->GetState_Safe(state);
	return this->pTextDefinition[textState][index].Properties;
}

TextStyleProperties ButtonStyle::GetTextProperties(WIDGET_STATE state, int index)
{
	return GetTextProperties(index);
}

bool ButtonStyle::ChangeTextStyle(Text text)
{
	if (pTextDefinition.size() != 1)
		throw std::exception("Can't change text style if there are multiple texts");	//change later


	if (pTextDefinition.size() == 1)
	{
		TextStyle& style = (*pTextDefinition.begin()).second[0];
		style.Properties.pText = text;
	}
	return true;

}
bool ButtonStyle::ChangeTextStyle(ColorRGB color)
{
	if (pTextDefinition.size() != 1)
		throw std::exception("Can't change text style if there are multiple texts");	//change later

	if (pTextDefinition.size() == 1)
	{
		TextStyle& style = (*pTextDefinition.begin()).second[0];
		style.Properties.TextColor = color;
	}
	return true;
}
bool ButtonStyle::ChangeTextStyle(string text)
{
	if (pTextDefinition.size() != 1)
		throw std::exception("Can't change text style if there are multiple texts");	//change later

	if (pTextDefinition.size() == 1)
	{
		TextStyle& style = (*pTextDefinition.begin()).second[0];
		string currentText = style.Properties.pText.GetCurrentValue();
		if (text != currentText)
		{
			style.Properties.pText.SetCurrentValue(text);
			style.TextPosition = style.GetTextPositionFromEnum(style.Boundary);
			return true;
		}
	}
	return false;
}


ButtonStyle ButtonStyle::NewStyle(Coord dimension)
{
	return ButtonStyle(dimension);
}

SDL_Surface* ButtonStyle::GetNewSurface(WIDGET_STATE state)
{
	return GetNewSurface(state, true, true);
}

SDL_Surface* ButtonStyle::GetNewSurface(WIDGET_STATE state, bool refreshtxt, bool refreshbtn)
{
	if (refreshbtn && Autosize())
		this->AutoResize();

	auto it = pUsedStates.find(state);
	if (it == pUsedStates.end())
		return nullptr;

	auto btn = GetButton(state, refreshbtn);
	auto txt = GetText(state, refreshtxt);

	SDL_Surface * textSurface = sdlWrapper.CreateSquare(sdlWrapper.GetSDLColor(COLOUR::C_DEFAULT), pDimension);

	if (btn.get())
	{
		SDL_BlitSurface(btn.get(), &pDimension, textSurface, &pDimension);
		Debug::SDLErrorCheck();
		if (DrawText())
		{
			SDL_BlitSurface(txt.get(), &pDimension, textSurface, &pDimension);
			Debug::SDLErrorCheck();
		}
		return textSurface;
	}
	return nullptr;
}

void ButtonStyle::AutoResize()
{
	if (this->pTextDefinition[textState].size() == 1)
	{
		auto currentdimension = this->pTextDefinition[textState][0].GetSizeFromText(this->pDimension.w);
		auto textposition = this->pTextDefinition[textState][0].GetTextPositionFromEnum(currentdimension);
		if (this->pDimension.w < currentdimension.x)
			this->pDimension.w = currentdimension.x;
		if (this->pDimension.h < currentdimension.y + std::abs(textposition.y * 2))
			this->pDimension.h = currentdimension.y + std::abs(textposition.y * 2);
		this->pDimension.x = 0;
		this->pDimension.y = 0;
	}
}

void ButtonStyle::ChangeDimension( const Coord& dimension )
{
	if (this->pDimension.w != dimension.x && this->pDimension.h != this->pDimension.y)
	{
		this->pDimension = primitives::SDLRect(0, 0, dimension.x, dimension.y);
		for( auto it = this->pUsedStates.begin(); it != pUsedStates.end(); ++it )
			this->GetNewSurface(*it);
	}
}


void ButtonStyle::Initialize()
{
	pTextDefinition = std::map<WIDGET_STATE, std::vector<TextStyle>>();
	pGUI = std::map<WIDGET_STATE, WindowGUI>();
	pUsedStates = std::set<WIDGET_STATE>();
	pStatePermutation = std::map<WIDGET_STATE, WIDGET_STATE>();

	pStatePermutation[WS_OFF] = WS_ON;
	pStatePermutation[WS_ON] = WS_NOT_ENABLED;
	pStatePermutation[WS_ACTIVE] = WS_HIDDEN;
	pStatePermutation[WS_HIDDEN] = WS_HOVER;
	pStatePermutation[WS_HOVER] = WS_HOVER;	//don't change this one, it is for exception, and avoid infinite loop in GetState_Safe(...)

	textState = WS_OFF;
	pTextDefinition[textState] = std::vector<TextStyle>();

	pColorKey = ColorRGB(0, 255, 255);

	pAutosize = true;	//initially true for test
}

ButtonStyle& ButtonStyle::AddWindowGUI(WIDGET_STATE state, WindowGUI gui)
{
	pGUI[state] = gui;
	return *this;
}

ButtonStyle& ButtonStyle::AddButtonState(WIDGET_STATE state)
{
	pUsedStates.insert(state);
	pStatePermutation[state] = state;
	return *this;
}

ButtonStyle& ButtonStyle::AddTextDefinition(WIDGET_STATE state, TextStyle style)
{
	pTextDefinition[textState].push_back(style);
	return *this;
}

ButtonStyle& ButtonStyle::AddTextDefinition(TextStyle style)
{
	AddTextDefinition(textState, style);
	return *this;
}

std::shared_ptr<SDL_Surface> ButtonStyle::GetText(WIDGET_STATE state, bool refresh)
{
	if (pTextDefinition[textState].size() == 1)
	{
		if (refresh)
			return pTextDefinition[textState][0].GetNewSurface(Coord(pDimension.w, pDimension.h));
		return pTextDefinition[textState][0].Surface;
	}
	else if (pTextDefinition[textState].size() > 1)
	{
		SDL_Surface * textResult = sdlWrapper.CreateSurface(pDimension.w, pDimension.h);
		SDL_Rect rect = sdlWrapper.CreateRect(0,0, pDimension.w, pDimension.h);

		for (int i = 0; i < pTextDefinition[textState].size(); i++)
		{
			auto textSurface = GetText(state, refresh, i);
			SDL_BlitSurface(textSurface.get(), NULL, textResult, NULL);
			Debug::SDLErrorCheck();
		}
		return std::shared_ptr<SDL_Surface>(textResult, SDL_FreeSurface);
	}
	return nullptr;
}

std::shared_ptr<SDL_Surface> ButtonStyle::GetText(WIDGET_STATE state, bool refresh, int index)
{
	if (pTextDefinition[textState].size() > index)
	{
		auto text = pTextDefinition[textState][index];
		if (refresh)
			return text.GetNewSurface(Coord(pDimension.w, pDimension.h));
		return text.Surface;
	}
	return nullptr;
}

std::shared_ptr<SDL_Surface> ButtonStyle::GetButton(WIDGET_STATE state,bool refresh)
{
	//auto it = pGUI.find(state);
	//if (it != pGUI.end())
	//{
		if(refresh)
			return pGUI[state].GetNewSurface(Coord(pDimension.w, pDimension.h));
		return pGUI[state].Surface;
	//}
	//return nullptr;
}

bool ButtonStyle::HasState(WIDGET_STATE state)
{
	auto it = pUsedStates.find(state);
	if (it == pUsedStates.end())
		return false;
	return true;
}

WIDGET_STATE ButtonStyle::GetStateFromPermutation(WIDGET_STATE state)
{
	if (HasState(state))
		return state;
	else
		return GetStateFromPermutation(this->pStatePermutation[state]);
}

WIDGET_STATE ButtonStyle::GetState_Safe(WIDGET_STATE state)
{
	if (HasState(state))
		return state;
	WIDGET_STATE result = GetStateFromPermutation(state);
	if (result == state)
		throw std::exception("error in finding state from button style");
}
