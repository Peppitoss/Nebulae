#pragma once
#include"Link.h"

class TransparentButton : public Link
{

public:

	TransparentButton();
	TransparentButton( const char* text, Coord position, int h, int w, std::string callbackname);

	//enLinkType GetType() override;
	bool isInside(int, int)override;

	void UseZoomModifier(bool value);

private:

	bool pUseZoomModifiers;
	void Initialize();

};