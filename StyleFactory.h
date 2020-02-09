#pragma once
#include"Style.h"

class StyleFactory
{
public:	

		StyleFactory();

		static ButtonStyle StandardButtonStyle(int width, int height, Text text);
		static ButtonStyle StandardButtonStyle(const imagehandling::IMGToSurface* surface, int textSize, Text text);
		static ButtonStyle StandardButtonStyleWithDisabledState(const imagehandling::IMGToSurface* surface, int textSize, Text text);
		
		static ButtonStyle IconButtonStyle(const imagehandling::IMGToSurface* surface );
		static ButtonStyle SingleStateIconButtonStyle(const imagehandling::IMGToSurface* surface, SDL_RendererFlip = SDL_FLIP_NONE);

		static ButtonStyle DestinationButtonStyle(const imagehandling::IMGToSurface* surface, Text header, Text text);

		static ButtonStyle TextBoxStyle(int width, int height, string fontname, int fontsize, Text text, const ColorRGB& textColor, enTextPosition position, bool OnlyOneState);
		static ButtonStyle LinkStyle(int width, int height, string fontname, int fontsize, Text text);
		static ButtonStyle LinkStyle(int width, int height, string fontname, int fontsize, Text text, ColorRGB );
		static ButtonStyle LinkStyle(int width, int height, string fontname, int fontsize, Text text, const ColorRGB& txtColor, const ColorRGB& backColor );

		static ButtonStyle SelectTargetStyle(int width, int height);

		static StatePermutation InitializePermutation(WIDGET_STATE);

private:


};
