#pragma once
#include"graphics.h"
#include<string>

using std::string;
//using enObjectDiscreteRotation;
//using enObjectFlipped;

struct ModuleObject
{
public:

	ModuleObject();
	ModuleObject( const ModuleObject& );
	ModuleObject( ModuleObject&& );

	ModuleObject & operator=( const ModuleObject& );
	ModuleObject & operator=( ModuleObject&& );

	static ModuleObject ReadModule(string content);
	static ModuleObject ReadHitBox(string content);

	Coord pos;
	string name;
	int HitBoxWidth;
	int HitBoxHeight;
	int HitBoxType;
	enObjectDiscreteRotation dRotation;
	enObjectFlipped dFlipped;
	bool drawGloballyLast;
};