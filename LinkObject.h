#pragma once
#include"Graphics.h"
#include"Link.h"
#include<functional>
//#include"Widget.h"
//#include<SDL.h>
#include<string>

using std::string;


class LinkObject
{

public:

	LinkObject();
	
	static LinkObject ReadLink(string content);

	string text;
	Coord size;
	Coord relativepos;
	string type;
	enLinkType GetLinkType();
	static enLinkType GetLinkType( string type );
};
