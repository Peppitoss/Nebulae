#pragma once
#include<SDL.h>
#include<string>

namespace SDLKeyBible
{
	using std::string;

	bool IsNumber(SDL_Keycode& key);
	string GetKey(SDL_Event& e);
	string GetKeyFromScancode(SDL_Scancode&);

	bool IsKeyMouseButton(string key);

	string GetMouseButtonKey(Uint8);
	Uint8 GetMouseButtonFromKey(string key);

	//bool IsArrowKey(SDL_Event& e);
	bool IsArrowKey(SDL_Scancode&);
	bool IsArrowKey(string);

	bool IsEmptyString(SDL_Scancode& c);

	bool IsNumber(SDL_Scancode& c);
	bool IsEscape(SDL_Scancode& c);

	bool IsLetter(SDL_Scancode& c);
	//bool IsLetter(SDL_KeyboardEvent* c);

	bool IsBackspace(SDL_Scancode& c);
	//bool IsBackspace(SDL_KeyboardEvent* c);

	bool IsDelete(SDL_Scancode& c);
	//bool IsDelete(SDL_KeyboardEvent* c);

	SDL_Scancode GetNumberAsScancode(int number);
	SDL_Scancode GetLetterAsScancode(const char* letter);

	extern const string sLeftMouseButton;
	extern const string sMiddleMouseButton;
	extern const string sRightMouseButton;
	extern const string sX1MouseButton;
	extern const string sX2MouseButton;

}