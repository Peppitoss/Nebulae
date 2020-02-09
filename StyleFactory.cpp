#include"StyleFactory.h"
#include"ButtonSurfaceCollection.h"


StyleFactory::StyleFactory()
{

}

StatePermutation StyleFactory::InitializePermutation(WIDGET_STATE state)
{
	StatePermutation result = StatePermutation();
	result[WS_OFF] = state;
	result[WS_HOVER] = state;
	result[WS_ACTIVE] = state;
	result[WS_HIDDEN] = state;
	result[WS_NOT_ENABLED] = state;
	result[WS_ON] = state;
	return result;
}

ButtonStyle StyleFactory::SelectTargetStyle(int width, int height)
{
	Coord dimension = Coord(width, height);
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);
	result.AddButtonState(WS_HOVER);
	result.AddButtonState(WS_ON);
	result.AddButtonState(WS_ACTIVE);
	result.SetDrawText(false);
	result.SetColorKey(colorKeyFromEnum(EnumColorKey::kCYAN));

	WindowGUI wsOff = WindowGUI();
	wsOff.BackgroundColor = ColorRGB(0, 255, 255, 255);	//transparent
	wsOff.Dimension = dimension;
	wsOff.pPremade = false;
	wsOff.UseBorder = false;

	WindowGUI wsHover = WindowGUI();
	wsHover.BackgroundColor = ColorRGB(0, 255, 255, 255);	//transparent
	wsHover.Dimension = dimension;
	wsHover.pPremade = false;
	wsHover.UseBorder = true;
	wsHover.Border.BorderThickness = 1;
	wsHover.Border.BorderColor = ColorRGB(255, 255, 255, 255);

	WindowGUI wsOn = WindowGUI();
	wsOn.BackgroundColor = ColorRGB(0, 255, 255, 255);	//transparent
	wsOn.Dimension = dimension;
	wsOn.pPremade = false;
	wsOn.UseBorder = true;
	wsOn.Border.BorderThickness = 1;
	wsOn.Border.BorderColor = ColorRGB(100, 255, 100, 255);

	WindowGUI wsActive = WindowGUI();
	wsActive.BackgroundColor = ColorRGB(0, 100, 0, 100);	//transparent
	wsActive.Dimension = dimension;
	wsActive.pPremade = false;
	wsActive.UseBorder = true;
	wsActive.Border.BorderThickness = 1;
	wsActive.Border.BorderColor = ColorRGB(255, 255, 255, 255);

	result.AddWindowGUI(WS_OFF, wsOff).AddWindowGUI(WS_HOVER, wsHover).AddWindowGUI(WS_ON, wsOn).AddWindowGUI(WS_ACTIVE, wsActive);
	return result;
}

ButtonStyle StyleFactory::TextBoxStyle(int width, int height, string fontname, int fontsize, Text text, const  ColorRGB& textColor, enTextPosition textPosition, bool OnlyOneState)
{
	if (!OnlyOneState)
		return LinkStyle(width, height, fontname, fontsize, text, textColor);

	Coord dimension = Coord(width, height);
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);

	TextStyle txtStyle = TextStyle();
	txtStyle.Properties.pText = Text();
	txtStyle.Properties.pText.SetCurrentValue(text.GetCurrentValue());
	txtStyle.Properties.Font = fontname;
	txtStyle.Properties.size = fontsize;
	txtStyle.Properties.TextColor = textColor;
	txtStyle.Properties.enPosition = textPosition;
	txtStyle.Properties.CropText = true;

	WindowGUI wsStandard = WindowGUI();
	wsStandard.BackgroundColor = ColorRGB(150, 13, 0, 220);	//transparent
	wsStandard.Dimension = dimension;
	wsStandard.pPremade = false;
	wsStandard.UseBorder = true;
	wsStandard.Border.BorderThickness = 1;
	wsStandard.Border.BorderColor = ColorRGB(255, 255, 255, 220);	

	result.AddWindowGUI(WS_OFF, wsStandard);
	result.AddTextDefinition(txtStyle);

	return result;
}

ButtonStyle StyleFactory::LinkStyle(int width, int height, string fontname, int fontsize, Text text)
{
	return StyleFactory::LinkStyle(width, height, fontname, fontsize, text, ColorRGB::White());
}

ButtonStyle StyleFactory::LinkStyle(int width, int height, string fontname, int fontsize, Text text, const ColorRGB& textColor, const ColorRGB& backColor)
{
	Coord dimension = Coord(width, height);
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);
	result.AddButtonState(WS_HOVER);
	result.AddButtonState(WS_ACTIVE);

	TextStyle txtStyle = TextStyle();
	txtStyle.Properties.pText = Text();
	txtStyle.Properties.pText.SetCurrentValue(text.GetCurrentValue());
	txtStyle.Properties.Font = fontname;
	txtStyle.Properties.size = fontsize;
	txtStyle.Properties.TextColor = textColor;

	WindowGUI wsStandard = WindowGUI();
	wsStandard.BackgroundColor = ColorRGB(backColor.red(), backColor.green(), backColor.blue(), 100);	//transparent
	wsStandard.Dimension = dimension;
	wsStandard.pPremade = false;
	wsStandard.UseBorder = false;

	WindowGUI wsHover = WindowGUI();
	wsHover.BackgroundColor = ColorRGB(backColor.red(), backColor.green(), backColor.blue(), 200);	//transparent
	wsHover.Dimension = dimension;
	wsHover.pPremade = false;
	wsHover.UseBorder = false;

	WindowGUI wsSelect = WindowGUI();
	wsSelect.BackgroundColor = ColorRGB(backColor.red(), backColor.green(), backColor.blue(), 150);	//transparent
	wsSelect.Dimension = dimension;
	wsSelect.pPremade = false;
	wsSelect.UseBorder = false;

	result.AddWindowGUI(WS_OFF, wsStandard).AddWindowGUI(WS_HOVER, wsHover).AddWindowGUI(WS_ACTIVE, wsSelect);
	result.AddTextDefinition(txtStyle);

	return result;
}

ButtonStyle StyleFactory::LinkStyle(int width, int height, string fontname, int fontsize, Text text, ColorRGB Color)
{
	return StyleFactory::LinkStyle(width, height, fontname, fontsize, text, ColorRGB::White(), ColorRGB::White());
}

ButtonStyle StyleFactory::SingleStateIconButtonStyle(const imagehandling::IMGToSurface* surface, SDL_RendererFlip  flip)
{
	Coord dimension = Coord(surface->GetSpriteSize().width(), surface->GetSpriteSize().height());
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);
	result.SetDrawText(false);
	result.SetColorKey(colorKeyFromEnum(surface->get_colorKey()));

	WindowGUI wsOff = WindowGUI(surface, dimension, Coord(0));
	wsOff.Dimension = dimension;
	wsOff.CropPosition = Coord(0);
	wsOff.UseBorder = false;
	wsOff.enFlip = flip;

	result.AddWindowGUI(WS_OFF, wsOff);
	return result;
}

ButtonStyle StyleFactory::IconButtonStyle(const imagehandling::IMGToSurface* surface)
{
	Coord dimension = Coord(surface->GetSpriteSize().width(), surface->GetSpriteSize().height()/3);
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);
	result.AddButtonState(WS_HOVER);
	result.AddButtonState(WS_ACTIVE);
	result.SetDrawText(false);
	result.SetColorKey(colorKeyFromEnum(surface->get_colorKey()));

	WindowGUI wsOff = WindowGUI(surface, dimension, Coord(0));
	wsOff.Dimension = dimension;
	wsOff.CropPosition = Coord(0).yadd(dimension.y * 2);
	wsOff.UseBorder = false;

	WindowGUI wsHover = WindowGUI(surface, dimension, Coord(0));
	wsHover.Dimension = dimension;
	wsHover.CropPosition = Coord(0).yadd(dimension.y);
	wsHover.UseBorder = false;

	WindowGUI wsActive = WindowGUI(surface, dimension, Coord(0));
	wsActive.Dimension = dimension;
	wsActive.CropPosition = Coord(0);
	wsActive.UseBorder = false;

	result.AddWindowGUI(WS_OFF, wsOff).AddWindowGUI(WS_HOVER, wsHover).AddWindowGUI(WS_ACTIVE, wsActive);
	return result;

}

ButtonStyle StyleFactory::StandardButtonStyle(int width, int height, Text text)
{
	Coord dimension = Coord(width, height);
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);
	result.AddButtonState(WS_HOVER);
	result.AddButtonState(WS_ACTIVE);
	result.AddButtonState(WS_ON);
	result.AddButtonState(WS_NOT_ENABLED);
	
	TextStyle txtStyle = TextStyle();
	txtStyle.Properties.pText = text;

	BorderStyle borderStyle = BorderStyle();
	borderStyle.BorderColor = ColorRGB(100, 100, 100);
	borderStyle.BorderThickness = 2;
	borderStyle.UseTextures = false;

	WindowGUI wsStandard = WindowGUI();
	wsStandard.BackgroundColor = ColorRGB(68, 53, 19, 200);	//transparent
	wsStandard.Border = borderStyle;
	wsStandard.Dimension = dimension;
	wsStandard.pPremade = false;
	wsStandard.UseBorder = true;

	WindowGUI wsHover = WindowGUI();
	wsHover.BackgroundColor = ColorRGB(88, 73, 39, 255);	//transparent
	wsHover.Border = borderStyle;
	wsHover.Dimension = dimension;
	wsHover.pPremade = false;
	wsHover.UseBorder = true;

	WindowGUI wsActive = WindowGUI();
	wsActive.BackgroundColor = ColorRGB(108, 93, 59, 255);	//transparent
	wsActive.Border = borderStyle;
	wsActive.Dimension = dimension;
	wsActive.pPremade = false;
	wsActive.UseBorder = true;

	WindowGUI wsDisabled = WindowGUI();
	wsDisabled.BackgroundColor = ColorRGB(38, 23, 0, 100);	//transparent
	wsDisabled.Border = borderStyle;
	wsDisabled.Dimension = dimension;
	wsDisabled.pPremade = false;
	wsDisabled.UseBorder = true;

	result.AddWindowGUI(WS_OFF, wsStandard).AddWindowGUI(WS_HOVER, wsHover).AddWindowGUI(WS_ACTIVE, wsActive).AddWindowGUI(WS_ON, wsStandard).AddWindowGUI(WS_NOT_ENABLED, wsDisabled);
	result.AddTextDefinition(txtStyle);

	return result;
}

ButtonStyle StyleFactory::StandardButtonStyle(const imagehandling::IMGToSurface* surface, int textSize, Text text)
{
	Coord dimension = Coord(surface->GetSpriteSize().width(), surface->GetSpriteSize().height()/3);
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);
	result.AddButtonState(WS_HOVER);
	result.AddButtonState(WS_ON);
	result.AddButtonState(WS_ACTIVE);
	result.SetDrawText(true);
	result.SetColorKey(colorKeyFromEnum(surface->get_colorKey()));

	TextStyle txtStyle = TextStyle();
	txtStyle.Properties.pText = text;
	txtStyle.Properties.size = textSize;

	WindowGUI wsOff = WindowGUI(surface, dimension, Coord(0));
	wsOff.Dimension = dimension;
	wsOff.CropPosition = Coord(0).yadd(dimension.y * 2);
	wsOff.UseBorder = false;

	WindowGUI wsHover = WindowGUI(surface, dimension, Coord(0));
	wsHover.Dimension = dimension;
	wsHover.CropPosition = Coord(0).yadd(dimension.y);
	wsHover.UseBorder = false;

	WindowGUI wsActive = WindowGUI(surface, dimension, Coord(0));
	wsActive.Dimension = dimension;
	wsActive.CropPosition = Coord(0);
	wsActive.UseBorder = false;

	result.AddWindowGUI(WS_OFF, wsOff).AddWindowGUI(WS_HOVER, wsHover).AddWindowGUI(WS_ACTIVE, wsActive).AddWindowGUI(WS_ON, wsActive);
	result.AddTextDefinition(txtStyle);

	return result;
}

ButtonStyle StyleFactory::StandardButtonStyleWithDisabledState(const imagehandling::IMGToSurface* surface, int textSize, Text text)
{
	Coord dimension = Coord(surface->GetSpriteSize().width(), surface->GetSpriteSize().height() / 4);
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);
	result.AddButtonState(WS_HOVER);
	result.AddButtonState(WS_ON);
	result.AddButtonState(WS_ACTIVE);
	result.AddButtonState(WS_NOT_ENABLED);
	result.SetDrawText(true);
	result.SetColorKey(colorKeyFromEnum(surface->get_colorKey()));

	TextStyle txtStyle = TextStyle();
	txtStyle.Properties.pText = text;
	txtStyle.Properties.size = textSize;

	WindowGUI wsDisabled = WindowGUI(surface, dimension, Coord(0));
	wsDisabled.Dimension = dimension;
	wsDisabled.CropPosition = Coord(0).yadd(dimension.y * 3);
	wsDisabled.UseBorder = false;

	WindowGUI wsOff = WindowGUI(surface, dimension, Coord(0));
	wsOff.Dimension = dimension;
	wsOff.CropPosition = Coord(0).yadd(dimension.y * 2);
	wsOff.UseBorder = false;

	WindowGUI wsHover = WindowGUI(surface, dimension, Coord(0));
	wsHover.Dimension = dimension;
	wsHover.CropPosition = Coord(0).yadd(dimension.y);
	wsHover.UseBorder = false;

	WindowGUI wsActive = WindowGUI(surface, dimension, Coord(0));
	wsActive.Dimension = dimension;
	wsActive.CropPosition = Coord(0);
	wsActive.UseBorder = false;

	result.AddWindowGUI(WS_OFF, wsOff).AddWindowGUI(WS_HOVER, wsHover).AddWindowGUI(WS_ACTIVE, wsActive).AddWindowGUI(WS_ON, wsActive).AddWindowGUI(WS_NOT_ENABLED, wsDisabled);
	result.AddTextDefinition(txtStyle);

	return result;
}


ButtonStyle StyleFactory::DestinationButtonStyle(const imagehandling::IMGToSurface* surface, Text header, Text text)
{
	auto usedSurface = ImportImage_DestinationBtn;

	Coord dimension = Coord(usedSurface->GetSpriteSize().width(), usedSurface->GetSpriteSize().height() / 3);
	ButtonStyle result = ButtonStyle(dimension);
	result.DefineStatePermutation(InitializePermutation(WS_OFF));
	result.AddButtonState(WS_OFF);
	result.AddButtonState(WS_HOVER);
	result.AddButtonState(WS_ACTIVE);
	result.SetDrawText(true);
	result.SetColorKey(colorKeyFromEnum(usedSurface->get_colorKey()));

	TextStyle txtStyleHeader = TextStyle();
	txtStyleHeader.Properties.pText = header;
	txtStyleHeader.TextPosition = Coord(8, 4);
	txtStyleHeader.Properties.enPosition = enTextPosition::noTxtPosition;

	TextStyle txtStyleValue = TextStyle();
	txtStyleValue.Properties.pText = text;
	txtStyleValue.TextPosition = Coord(8, 24);
	txtStyleValue.Properties.enPosition = enTextPosition::noTxtPosition;

	WindowGUI wsOff = WindowGUI(surface, dimension, Coord(0));
	wsOff.Dimension = dimension;
	//wsOff.pPremade = surface;
	wsOff.CropPosition = Coord(0).yadd(dimension.y * 2);
	wsOff.UseBorder = false;

	WindowGUI wsHover = WindowGUI(surface, dimension, Coord(0));
	wsHover.Dimension = dimension;
	//wsHover.pPremade = surface;
	wsHover.CropPosition = Coord(0).yadd(dimension.y);
	wsHover.UseBorder = false;

	WindowGUI wsActive = WindowGUI(surface, dimension, Coord(0));
	wsActive.Dimension = dimension;
	//wsActive.pPremade = surface;
	wsActive.CropPosition = Coord(0);
	wsActive.UseBorder = false;

	result.AddWindowGUI(WS_OFF, wsOff).AddWindowGUI(WS_HOVER, wsHover).AddWindowGUI(WS_ACTIVE, wsActive);
	result.AddTextDefinition(txtStyleHeader);
	result.AddTextDefinition(txtStyleValue);
	return result;
}