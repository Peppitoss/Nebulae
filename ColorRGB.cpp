#include"ColorRGB.h"
#include"StringBuilder.h"
#include<string>

ColorRGB::ColorRGB() 
{ 
	pRed = BoundedDouble(0,255); 
	pGreen = BoundedDouble(0, 255);
	pBlue = BoundedDouble(0, 255);
	pAlpha = BoundedDouble(255, 255);
};

ColorRGB::ColorRGB(const ColorRGB & other)
{
	newColor(other.red(), other.green(), other.blue(), other.alpha());
}

ColorRGB::ColorRGB(const ColorRGB && other)
{
	newColor(other.red(), other.green(), other.blue(), other.alpha());
}

void ColorRGB::newColor(EnumColor enumColor)
{
	newColor(colorFromEnum(enumColor));
};

Uint8 ColorRGB::limitColor(const Uint8 color) const
{
	if (color < 0)
		return 0;
	if (color > 255)
		return 255;
	return color;
}

Uint32 ColorRGB::ToSDL_Map(const SDL_PixelFormat& format) const
{
	return SDL_MapRGBA(&format, red(), green(), blue(), alpha());
}

ColorRGB ColorRGB::operator+(const ColorRGB& b) const
{
	return ColorRGB(limitColor(this->red() + b.red()), limitColor(this->green() + b.green()), limitColor(this->blue() + b.blue()));
}
ColorRGB & ColorRGB::operator+=(const ColorRGB &b)
{
	*this = ColorRGB(limitColor(this->red() + b.red()), limitColor(this->green() + b.green()), limitColor(this->blue() + b.blue()));
	return *this;
}

ColorRGB ColorRGB::operator-(const ColorRGB& b) const
{
	return ColorRGB(limitColor(this->red() - b.red()), limitColor(this->green() - b.green()), limitColor(this->blue() - b.blue()));
}
ColorRGB & ColorRGB::operator-=(const ColorRGB &b)
{
	*this = ColorRGB(limitColor(this->red() - b.red()), limitColor(this->green() - b.green()), limitColor(this->blue() - b.blue()));
	return *this;
}

void ColorRGB::newColor(double red, double green, double blue, double alpha)
{
	pRed = BoundedDouble(red,255);
	pGreen = BoundedDouble(green, 255);
	pBlue = BoundedDouble(blue, 255);
	pAlpha = BoundedDouble(alpha, 255);
};

ColorRGB& ColorRGB::operator=(const ColorRGB& other)
{
	pRed = other.pRed;
	pGreen = other.pGreen;
	pBlue = other.pBlue;
	pAlpha = other.pAlpha;
	return *this;
}

ColorRGB colorFromEnum(EnumColor enumColor)
{
	ColorRGB result;

	switch (enumColor) {

	case RED:
		result.newColor(255, 0, 0, 255);
		break;
	case GREEN:
		result.newColor(0, 255, 0, 255);
		break;
	case YELLOW:
		result.newColor(255, 255, 0, 255);
		break;
	case MAGENTA:
		result.newColor(255, 0, 255, 255);
		break;
	case CYAN:
		result.newColor(0, 255, 255, 255);
		break;
	case BLUE:
		result.newColor(0, 0, 255, 255);
		break;
	case GREY:
		result.newColor(192, 192, 192, 255);
		break;
	case WHITE:
		result.newColor(255, 255, 255, 255);
		break;
	case BLACK:
		result.newColor(0, 0, 0, 255);
		break;
	case INVISIBLE:
		result.newColor(0, 0, 0, 0);
		break;
	default:
		result.newColor(0, 0, 0, 255);
	}
	return result;
}

ColorRGB colorKeyFromEnum(EnumColorKey enumColor)
{
	ColorRGB result;

	switch (enumColor) {

	case kGREEN:
		result.newColor(0, 255, 0, 255);
		break;
	case kMAGENTA:
		result.newColor(255, 0, 255, 255);
		break;
	case kCYAN:
		result.newColor(0, 255, 255, 255);
		break;
	case kNoAlpha:
		result.newColor(0, 0, 0, 0);
	default:
		result.newColor(0, 0, 0, 0);
	}
	return result;
}

std::string ColorRGB::ToString()
{
	return IntToString(this->pRed.value()) + "," + IntToString(this->pGreen.value()) + "," + IntToString(this->pBlue.value()) + "," + IntToString(this->pAlpha.value());
}

bool operator==(const ColorRGB& lhs, const ColorRGB& rhs) { return lhs.red() == rhs.red() && lhs.green() == rhs.green() && lhs.blue() == rhs.blue() && lhs.alpha() == rhs.alpha(); };
bool operator==(const ColorRGB& lhs, const SDL_Color& rhs) { return lhs.red() == rhs.r && lhs.green() == rhs.g && lhs.blue() == rhs.b && lhs.alpha() == rhs.a; };

bool operator!=(const ColorRGB& lhs, const ColorRGB& rhs) { return !(lhs == rhs); };
bool operator!=(const ColorRGB& lhs, const SDL_Color& rhs) { return !(lhs == rhs); };