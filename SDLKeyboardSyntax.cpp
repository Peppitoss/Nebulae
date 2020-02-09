#include"SDLKeyboardSyntax.h"
#include"StringBuilder.h"

namespace SDLKeyBible
{

	const string sLeftMouseButton = "LMB";
	const string sMiddleMouseButton = "MMB";
	const string sRightMouseButton = "RMB";
	const string sX1MouseButton = "X1";
	const string sX2MouseButton = "X2";

	bool IsNumber(SDL_Keycode& key)
	{
		return (key >= 48 && key <= 57);
	}

	string GetKey(SDL_Event& e)
	{
		SDL_KeyboardEvent* key = &e.key;
		return SDL_GetKeyName(key->keysym.sym);
	}

	string GetKeyFromScancode(SDL_Scancode& c)
	{
		return SDL_GetScancodeName(c);
	}

	bool IsArrowKey(SDL_Scancode& c)
	{
		return c == SDL_SCANCODE_RIGHT || c == SDL_SCANCODE_LEFT || c == SDL_SCANCODE_DOWN || c == SDL_SCANCODE_UP;
	}

	bool IsArrowKey(string value)
	{
		return value == "Up" || value == "Down" || value == "Right" || value == "Left";
	}

	SDL_Scancode GetNumberAsScancode(int number)
	{
		return SDL_GetScancodeFromName(&IntToString(number)[0]);
	}

	SDL_Scancode GetLetterAsScancode(const char* letter)
	{
		return SDL_GetScancodeFromName(letter);
	}

	bool IsEmptyString(SDL_Scancode& c)
	{
		return c == SDL_SCANCODE_UNKNOWN;
	}

	bool IsNumber(SDL_Scancode& c)
	{
		return ((c >= SDL_SCANCODE_1) && (c <= SDL_SCANCODE_0));
	}

	bool IsEscape(SDL_Scancode& c)
	{
		return c == SDL_SCANCODE_ESCAPE;
	}

	bool IsLetter(SDL_Scancode& c)
	{
		return ((c >= SDL_SCANCODE_A) && (c <= SDL_SCANCODE_Z));
	}
	bool IsBackspace(SDL_Scancode& c)
	{
		return c == SDL_SCANCODE_BACKSPACE;
	}

	bool IsDelete(SDL_Scancode& c)
	{
		return c == SDL_SCANCODE_DELETE;
	}

	bool IsKeyMouseButton(string key)
	{
		if (key == sLeftMouseButton)
			return true;
		if (key == sMiddleMouseButton);
			return true;
		if (key == sRightMouseButton)
			return true;
		if (key == sX1MouseButton)
			return true;
		if (key == sX2MouseButton)
			return true;
		return false;
	}

	string GetMouseButtonKey(Uint8 button)
	{
		switch (button)
		{
		case(SDL_BUTTON_LEFT):
			return sLeftMouseButton;
			break;
		case(SDL_BUTTON_MIDDLE):
			return sMiddleMouseButton;
			break;
		case(SDL_BUTTON_RIGHT):
			return sRightMouseButton;
			break;
		case(SDL_BUTTON_X1):
			return sX1MouseButton;
			break;
		case(SDL_BUTTON_X2):
			return sX2MouseButton;
			break;
		}
	}
	Uint8 GetMouseButtonFromKey(string key)
	{
		if (key == sLeftMouseButton)
			return SDL_BUTTON_LEFT;
		if (key == sMiddleMouseButton)
			return SDL_BUTTON_MIDDLE;
		if (key == sRightMouseButton)
			return SDL_BUTTON_RIGHT;
		if (key == sX1MouseButton)
			return SDL_BUTTON_X1;
		if (key == sX2MouseButton)
			return SDL_BUTTON_X2;


	}

}