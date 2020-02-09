#include "Wrapper.h"
#include"Debug.h"
#include <stdexcept>
#include <math.h>

const std::string standardLinkFont = "Roboto-Regular.ttf";

bool CompareColor(const SDL_Color& A, const SDL_Color& B)
{
	return A.a == B.a && A.b == B.b && A.g == B.g && A.r == B.r;
}

Wrapper::Wrapper()
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    }
	
	//Initialize SDL_mixer
   	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
   	{
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );  
	}
    
	if( TTF_Init() == -1 )
	{
		throw "TTF_Init()";
	}
	_Surfaces.reserve(100);
}

Wrapper::~Wrapper()
{
	for (int i = 0; i < _Surfaces.size(); i++)
	{
		SDL_FreeSurface(_Surfaces[i]._surface);
		_Surfaces[i]._surface = NULL;
	}
	if(mFont)
		
    //Quit SDL subsystems
    IMG_Quit();
	SDL_CloseAudio();
	TTF_CloseFont(mFont);
	TTF_Quit();
	SDL_Quit();
}

////////

void Wrapper::LoadFont(std::string fontName, TextSize fontSize)
{
	std::string fontPath = "../Fonts/";
	fontPath += fontName;
	mFontList[fontName][fontSize] = TTF_OpenFont(fontPath.c_str(), fontSize);
}

////////

SDL_Rect Wrapper::CreateRect(int x, int y, int w, int h)
{
	SDL_Rect rect = {x, y, w, h};
	return rect;
}

SDL_Surface* Wrapper::CreateSurface(int width, int height)
{
	return CreateSurface(width, height, C_DEFAULT);
}
SDL_Surface* Wrapper::CreateSurface(int width, int height, COLOUR color)
{
	return CreateSurface(width, height, GetSDLColor(color));
}

SDL_Surface* Wrapper::CreateSurface(int width, int height, SDL_Color color)
{
	SDL_Surface * image;
	unsigned int rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmashttps://www.google.no/preferences?hl=en&prev=https://www.google.no/search?q%3Dkatt%2Bfortoppelse%26ie%3Dutf-8%26oe%3Dutf-8%26client%3Dfirefox-b-ab%26gfe_rd%3Dcr%26ei%3DFSWbWbfkPMyq8wfihLyYBw#languagesk = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	image = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
	Debug::SDLErrorCheck();

	SDL_FillRect(image, NULL, Wrapper::GetColor(image, color));
	return image;
}

SDL_Surface* Wrapper::CreateSurface(int width, int height, int red, int green, int blue, int alpha)
{
	SDL_Surface * image;
	unsigned int rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	image = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
	Debug::SDLErrorCheck();

	SDL_FillRect(image, NULL, SDL_MapRGBA(image->format, red, green, blue, alpha) );
	return image;
}

SDL_Surface* Wrapper::CreateContainedSurface(int width, int height)
{
	return CreateSurface(width, height, GetSDLColor(C_DEFAULT));
}

SDL_Surface* Wrapper::CreateContainedSurface(int width, int height, SDL_Color color)
{
	SDL_Surface* result = CheckExistingSurfaces(width, height, color, ST_DEFAULT);
	if (result != NULL)
		return result;

	SDL_Surface* image = CreateSurface(width, height, color);

	_Surfaces.push_back(Data(image, color, ST_DEFAULT));
	return image;
}

SDL_Rect Wrapper::GetTextSize( TextSize fontSize, std::string fontName, std::string text, int fontStyle)
{
	if(!mFontList[fontName][fontSize])
		LoadFont(fontName, fontSize);
	TTF_SetFontStyle( mFontList[fontName][fontSize], fontStyle );

	int width, height;

	TTF_SizeUTF8( mFontList[fontName][fontSize], text.c_str(), &width, &height );

	SDL_Rect result = {0,0,0,0};
	result.w = fmax(0,width);
	result.h = fmax(0,height);
	return result;
}

SDL_Surface* Wrapper::CreateTextSurface(std::string text, TextSize fontSize, std::string fontName )
{
	SDL_Color txtColor = {0,0,0};
	return Wrapper::CreateTextSurface(text, fontSize,fontName, txtColor, TTF_STYLE_NORMAL );
}

SDL_Surface* Wrapper::CreateTextSurface(std::string text, TextSize fontSize, std::string fontName, SDL_Color color, int fontStyle )
{
	SDL_Surface* surface = 0;

	SDL_Color textColor = {color.r,color.g,color.b,color.a};
	if(!mFontList[fontName][fontSize])
		LoadFont(fontName, fontSize);

	if(!mFontList[fontName][fontSize])
	{
		throw std::runtime_error("No font!");
	}

	TTF_SetFontStyle( mFontList[fontName][fontSize], fontStyle );

	surface = TTF_RenderText_Blended(mFontList[fontName][fontSize], text.c_str(), textColor);
	if(surface == nullptr)
		throw std::runtime_error("Text surface failed to generate!");
	return surface;
}

SDL_Surface* Wrapper::CreateTextSurface(std::string text)
{
	return CreateTextSurface(text, 16,"Xcelsion.ttf");
}

SDL_Texture* Wrapper::GetLetterTexture(char letter, TextSize fontSize, std::string fontName, SDL_Renderer& renderer)
{

	//slow, because it is used in renderings alot, there should be really fast access for this
	SDL_Texture* texture = 0;
	texture = mFontSingleLetterSurfaces[letter][fontName][fontSize];
	if (texture)
		return mFontSingleLetterSurfaces[letter][fontName][fontSize];

	std::string strLetter = "";
	strLetter.push_back(letter);
	auto surface = CreateTextSurface(strLetter, fontSize, fontName, GetSDLColor(COLOUR::C_WHITE), TTF_STYLE_NORMAL);
	
	char interpolation = *SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	texture = SDL_CreateTextureFromSurface(&renderer, surface);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, &interpolation);

	Debug::SDLErrorCheck();

	mFontSingleLetterSurfaces[letter][fontName][fontSize] = texture;
	return texture;
}

Uint32 Wrapper::GetColor(SDL_Surface* image, COLOUR color)
{
	return GetColor(image, GetSDLColor(color));
}

Uint32 Wrapper::GetColor(SDL_Surface* image, SDL_Color color)
{
	return SDL_MapRGBA(image->format, color.r, color.g, color.b, color.a);
}

SDL_Color Wrapper::GetSDLColor(COLOUR color)
{
	SDL_Color result = { 0,0,0,0 };	//C_DEFAULT
	switch (color)
	{
	case C_WHITE:		result.r = 255; result.g = 255; result.b = 255; result.a = 255;	break;
	case C_GRAY:		result.r = 127; result.g = 127; result.b = 127; result.a = 255;	break;
	case C_BLACK:		result.r = 0; result.g = 0; result.b = 0; result.a = 255;	break;
	case C_BLACK_2:		result.r = 1; result.g = 2; result.b = 3; result.a = 255;	break;
	case C_RED:			result.r = 255; result.g = 31; result.b = 31; result.a = 255;	break;
	case C_GREEN:		result.r = 31; result.g = 255; result.b = 31; result.a = 255;	break;
	case C_BLUE:		result.r = 31; result.g = 31; result.b = 255; result.a = 255;	break;
	case C_CYAN:		result.r = 0; result.g = 255; result.b = 255; result.a = 255;	break;
	case C_MAGENTA:		result.r = 255; result.g = 0; result.b = 255; result.a = 255;	break;
	case C_YELLOW:		result.r = 255; result.g = 255; result.b = 0; result.a = 255;	break;
	case C_BEIGE:		result.r = 255; result.g = 255; result.b = 191; result.a = 255;	break;
	}
	return result;
}

void Wrapper::Set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
    *(Uint32 *)target_pixel = pixel;
}

SDL_Surface* Wrapper::CreateCircle(SDL_Color color, int size)
{
	SDL_Surface* result = CheckExistingSurfaces(size, size, color, ST_CIRCLE);
	if (result != NULL)
		return result;

	SDL_Surface* image = CreateContainedSurface(size, size);

	double cx = size / 2;
	double cy = size / 2;
	double radius = cx;
	Uint32 pixel = GetColor(image, color);

	//  From:
	// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles

	static const int BPP = 4;

	double r = (double)radius - 0.5;

	for (double dy = 1; dy <= r; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
		int x = cx - dx;

		Uint8 *target_pixel_a = (Uint8 *)image->pixels + ((int)(cy + r - dy)) * image->pitch + x * BPP;
		Uint8 *target_pixel_b = (Uint8 *)image->pixels + ((int)(cy - r + dy)) * image->pitch + x * BPP;

		for (; x <= cx + dx; x++)
		{
			*(Uint32 *)target_pixel_a = pixel;
			*(Uint32 *)target_pixel_b = pixel;
			target_pixel_a += BPP;
			target_pixel_b += BPP;
		}
	}

	pixel = GetColor(image, C_BLACK);
	double error = (double)-radius;
	double x = (double)radius -0.5;
	double y = (double)0.5;
	cx -= 0.5;
	cy -= 0.5;

	while (x >= y)
	{
		Set_pixel(image, (int)(cx + x), (int)(cy + y), pixel);
		Set_pixel(image, (int)(cx + y), (int)(cy + x), pixel);

		if (x != 0)
		{
			Set_pixel(image, (int)(cx - x), (int)(cy + y), pixel);
			Set_pixel(image, (int)(cx + y), (int)(cy - x), pixel);
		}

		if (y != 0)
		{
			Set_pixel(image, (int)(cx + x), (int)(cy - y), pixel);
			Set_pixel(image, (int)(cx - y), (int)(cy + x), pixel);
		}

		if (x != 0 && y != 0)
		{
			Set_pixel(image, (int)(cx - x), (int)(cy - y), pixel);
			Set_pixel(image, (int)(cx - y), (int)(cy - x), pixel);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}

	_Surfaces.push_back(Data(image, color, ST_CIRCLE));

	return image;
}

SDL_Surface* Wrapper::CreateSquare(SDL_Color color, SDL_Rect size)
{
	SDL_Surface* image = CreateSurface(size.w, size.h, GetSDLColor(C_DEFAULT));
	SDL_FillRect(image, NULL, GetColor(image, color));

	//SDL_Surface* interior = CreateContainedSurface(size.w - 3, size.h - 3);
	//SDL_FillRect(interior, NULL, GetColor(image, color));

	//SDL_Rect clip = CreateRect(2, 2, size.w-3, size.h-3);

	//SDL_BlitSurface(interior, NULL, image, &clip);
	//SDL_FreeSurface(interior);
	return image;
}

SDL_Surface* Wrapper::CreateSquareBorder(SDL_Color color, SDL_Rect size, int thickness)
{
	if (thickness <= 0)
		thickness = 1;

	SDL_Surface* image = CreateSurface(size.w, size.h, color);
	//SDL_FillRect(image, NULL, GetColor(image, color));

	int t = thickness*2;

	SDL_Surface* interior = CreateContainedSurface(size.w - t, size.h - t);
	SDL_FillRect(interior, NULL, GetColor(image, C_CYAN));

	SDL_Rect clip = CreateRect(thickness, thickness, 0, 0);

	SDL_BlitSurface(interior, NULL, image, &clip);
	Debug::SDLErrorCheck();

	SDL_SetColorKey(image, 1, GetColor(image, C_CYAN));
	Debug::SDLErrorCheck();
	SDL_FreeSurface(interior);
	return image;
}

SDL_Surface* Wrapper::CreateTriangle(SDL_Color color, int size)
{
	return CreateTriangle(color, size, size);
}

SDL_Surface* Wrapper::CreateTriangle(SDL_Color color, int width, int height)
{
	SDL_Surface* result = CheckExistingSurfaces(width, height, color, ST_TRIANGLE);
	if (result != NULL)
		return result;

	SDL_Surface* image = CreateSurface(width, height);;

	Uint32 inner = GetColor(image, color);
	Uint32 outer = GetColor(image, C_BLACK);

	double m = 2.0 * height / width;
	double mid = (width - 1) / 2.0;

	int y,x;

	for(y = 0; y < height-1; y++)
	{
		// Paint the interior.
		for(x = mid - y / m; x < mid; x ++)
		{
			Set_pixel(image, x, y, inner);
			Set_pixel(image, width - x - 1.5, y, inner);
		}

		// Draw the left and right edges.
		Set_pixel(image, mid - y / m, y, outer);
		Set_pixel(image, width - mid + y / m - 0.5, y, outer);
	}

	// Draw the bottom edge.
	for(x = 0; x < width; x ++)
		Set_pixel(image, x, height - 1, outer);

	_Surfaces.push_back(Data(image, color, ST_TRIANGLE));
	return image;
}

SDL_Surface* Wrapper::CheckExistingSurfaces(int width, int height, SDL_Color colour, SURFACE_TYPE type)
{
	for (int i = 0; i < _Surfaces.size(); i++)
		if (_Surfaces[i]._surface->w == width && 
			_Surfaces[i]._surface->h == height && 
			CompareColor(_Surfaces[i]._colour, colour) &&
			_Surfaces[i]._type == type)
			return _Surfaces[i]._surface;
	return NULL;
}
