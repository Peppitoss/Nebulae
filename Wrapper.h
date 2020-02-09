#pragma once
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <map>
#include <vector>
#include <string>

/////////////////////////////////////////////////
//											   //
//			- Courtesy of S.G				   //
//											   //
/////////////////////////////////////////////////

typedef int TextSize;
enum COLOUR 
{
	C_WHITE,
	C_GRAY,
	C_BLACK,
	C_RED,
	C_GREEN,
	C_TRANSPARENT,
	C_BLUE,
	C_CYAN,
	C_MAGENTA,
	C_YELLOW,
	C_BLACK_2,

	C_BEIGE,
	C_DEFAULT
};

enum SURFACE_TYPE
{
	ST_DEFAULT,
	ST_TEXT,
	ST_CIRCLE,
	ST_SQUARE,
	ST_TRIANGLE,
	ST_MODIFIABLE
};
bool CompareColor(const SDL_Color& A, const SDL_Color& B);

struct Data
{
	SURFACE_TYPE _type;
	SDL_Color _colour;
	SDL_Surface* _surface;

	Data(SDL_Surface* s, SDL_Color c, SURFACE_TYPE t) : _colour(c),_type(t),_surface(s)
	{ 

	}

	bool const operator==(const Data& sd) const
	{
		bool status = false;
		if (sd._surface->w == this->_surface->w && sd._surface->h == this->_surface->h && CompareColor(sd._colour,this->_colour) && sd._type == this->_type)
		{
			status = true;
		}
		return status;
	}

	virtual ~Data(void)
	{
		SDL_FreeSurface(_surface);
	}
};

struct SDLWindowDeleter{
    void operator()(SDL_Window* window) {
        SDL_DestroyWindow(window);
    }
};
struct SDLRendererDeleter{
	void operator()(SDL_Renderer* renderer) {
		SDL_DestroyRenderer(renderer);
    }
};
struct SDLMixChunkDeleter{
	void operator()(Mix_Chunk* renderer) {
		Mix_FreeChunk(renderer);
    }
};
struct SDLMusicDeleter{
	void operator()(Mix_Music* renderer) {
		Mix_FreeMusic(renderer);
    }
};
struct SDLSurfaceDeleter{
	void operator()(SDL_Surface* surf) {
		SDL_FreeSurface(surf);
    }
};
//struct SDLTextureDeleter{
//    void operator()(SDL_Texture* texture) {
//		SDL_DestroyTexture(texture);
//    }
//};
class Wrapper
{
	TTF_Font* mFont;
	std::map<std::string,std::map<int, TTF_Font*>> mFontList;
	std::map<char, std::map<std::string, std::map<int, SDL_Texture*>>> mFontSingleLetterSurfaces;

	std::vector<Data> _Surfaces;
	SDL_Surface* CheckExistingSurfaces(int w, int h, SDL_Color c, SURFACE_TYPE t);
	void Set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
	void LoadFont(std::string fontName, TextSize fontSize);
public:
	Wrapper(void);
	~Wrapper(void);
	static SDL_Rect CreateRect(int x, int y, int w, int h);
	SDL_Rect GetTextSize( TextSize fontSize, std::string fontName, std::string text, int fontStyle);
	static Uint32 GetColor(SDL_Surface* image, SDL_Color color);
	static Uint32 GetColor(SDL_Surface* image, COLOUR color);
	static SDL_Color GetSDLColor(COLOUR color);
	static SDL_Surface* CreateSurface(int width, int height);
	static SDL_Surface* CreateSurface(int width, int height, COLOUR color);
	static SDL_Surface* CreateSurface(int width, int height, SDL_Color color);
	static SDL_Surface* CreateSurface(int width, int height, int red, int green, int blue, int alpha);
	SDL_Surface* CreateTextSurface(std::string text, TextSize fontSize, std::string fontName, SDL_Color color, int fontStyle );
	SDL_Surface* CreateTextSurface(std::string text, TextSize fontSize, std::string fontName);
	SDL_Surface* CreateTextSurface(std::string text);
	SDL_Texture* GetLetterTexture(char letter, TextSize fontSize, std::string fontName, SDL_Renderer& );
	static SDL_Surface* CreateContainedSurface(int width, int height);
	SDL_Surface* CreateContainedSurface(int width, int height, SDL_Color color);
	SDL_Surface* CreateCircle(SDL_Color, int size);
	static SDL_Surface* CreateSquare(SDL_Color, SDL_Rect size);
	static SDL_Surface* Wrapper::CreateSquareBorder(SDL_Color color, SDL_Rect size, int thickness);
	SDL_Surface* CreateTriangle(SDL_Color, int size);
	SDL_Surface* CreateTriangle(SDL_Color, int width, int height);
};

extern Wrapper sdlWrapper;
extern const std::string standardLinkFont;