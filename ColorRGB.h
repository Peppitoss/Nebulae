#pragma once
#include<SDL.h>
#include<string>
#include"BoundedDouble.h"
//#include"TimeVariabel.h"

enum EnumColor
{
	RED = 0,
	GREEN,
	BLUE,
	YELLOW,
	MAGENTA,
	CYAN,
	GREY,
	WHITE,
	BLACK,
	INVISIBLE = 999
};

enum EnumColorKey
{
	kGREEN = 0,
	kMAGENTA = 1,
	kCYAN = 2,
	kNoAlpha = 3,
	kUNDEFINED,
};

class ColorRGB
{

public:
	ColorRGB();

	//ColorRGB(ColorRGB &);
	ColorRGB(SDL_Color color) { newColor(color.r, color.g, color.b, color.a); }
	ColorRGB(double red, double green, double blue) { newColor(red, green, blue, 255); };
	ColorRGB(double red, double green, double blue, double alpha) { newColor(red, green, blue, alpha); };
	ColorRGB(const ColorRGB & other);
	ColorRGB(const ColorRGB && other);

	double red() const { return pRed.value(); };
	double green() const { return pGreen.value(); };
	double blue() const { return pBlue.value(); };
	double alpha() const { return pAlpha.value(); };

	void newColor(double red, double green, double blue, double alpha);
	void newColor(ColorRGB color) { newColor(color.red(), color.green(), color.blue(), color.alpha()); };
	void newColor(EnumColor enumColor);

	void ReduceColorsByAmount(double amount) 
	{ 
		this->pBlue.increaseValue( -amount);
		this->pRed.increaseValue(-amount);
		this->pGreen.increaseValue(-amount);
		if (this->pBlue.value() < 0)
			this->pBlue.setvalue(0);
		if (this->pRed.value() < 0)
			this->pRed.setvalue(0);
		if (this->pGreen.value() < 0)
			this->pGreen.setvalue(0);
	}

	ColorRGB ChangeColorsByPercentAmount(double amount)
	{
		this->pBlue.ChangeValueByPercentage(amount);
		this->pRed.ChangeValueByPercentage(amount);
		this->pGreen.ChangeValueByPercentage(amount);
		if (this->pBlue.value() < 0)
			this->pBlue.setvalue(0);
		if (this->pRed.value() < 0)
			this->pRed.setvalue(0);
		if (this->pGreen.value() < 0)
			this->pGreen.setvalue(0);
		return *this;
	}

	ColorRGB& operator=(const ColorRGB&);
	ColorRGB operator+(const ColorRGB& b) const;
	ColorRGB & ColorRGB::operator+=(const ColorRGB &rhs);

	ColorRGB operator-(const ColorRGB& b) const;
	ColorRGB & ColorRGB::operator-=(const ColorRGB &rhs);

	static ColorRGB White() { return ColorRGB(255, 255, 255, 255); }
	static ColorRGB Black() { return ColorRGB(0, 0, 0, 255); }
	static ColorRGB Red() { return ColorRGB(255, 0, 0, 255); }
	static ColorRGB Green() { return ColorRGB(0, 255, 0, 255); }
	static ColorRGB Grey(int amount) { return ColorRGB(amount, amount, amount, 255); }
	static ColorRGB Grey(int amount, int alpha) { return ColorRGB(amount, amount, amount, alpha); }
	static ColorRGB Blue() { return ColorRGB(0, 0, 255, 255); }
	static ColorRGB Cyan() { return ColorRGB(0, 255, 255, 255); }
	static ColorRGB Magenta() { return ColorRGB(255, 0, 255, 255); }
	static ColorRGB Yellow() { return ColorRGB(255, 255, 0, 255); }
	static ColorRGB Invisible() { return ColorRGB(0, 0, 0, 0); }

	Uint8 limitColor(const Uint8 col) const;
	SDL_Color ToSDL_Color() const { return SDL_Color{ (Uint8)pRed.value(), (Uint8)pGreen.value(), (Uint8)pBlue.value(), (Uint8)pAlpha.value() }; }
	Uint32 ToSDL_Map(const SDL_PixelFormat&) const;

	std::string ToString();

protected:

	void ChangeVariabelToValue(ColorRGB value) { this->newColor(value); }
	ColorRGB GetVariabel() { return *this; }

private:
	BoundedDouble pBlue;
	BoundedDouble pRed;
	BoundedDouble pGreen;
	BoundedDouble pAlpha;
};

bool operator==(const ColorRGB& lhs, const ColorRGB& rhs);
bool operator==(const ColorRGB& lhs, const SDL_Color& rhs);

bool operator!=(const ColorRGB& lhs, const ColorRGB& rhs);
bool operator!=(const ColorRGB& lhs, const SDL_Color& rhs);


ColorRGB colorFromEnum(EnumColor enumColor);
ColorRGB colorKeyFromEnum(EnumColorKey enumColor);