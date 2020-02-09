#include"ModuleObject.h"
#include"CommonReadWriteFunctions.h"


ModuleObject::ModuleObject()
{
	this->name = "NoName";
	this->pos = Coord(0);
	this->HitBoxWidth = 0;
	this->HitBoxHeight = 0;
	dRotation = enObjectDiscreteRotation::goNoRotation;
	dFlipped = enObjectFlipped::goNotFlipped;
	drawGloballyLast = false;
}
ModuleObject::ModuleObject( const ModuleObject& other)
{
	this->name = other.name;
	this->pos = other.pos;
	this->HitBoxWidth = other.HitBoxWidth;
	this->HitBoxHeight = other.HitBoxHeight;
	this->dRotation = other.dRotation;
	this->dFlipped = other.dFlipped;
	this->drawGloballyLast = other.drawGloballyLast;
}

ModuleObject::ModuleObject( ModuleObject&& other)
{
	this->name = std::move(other.name);
	this->pos = std::move(other.pos);
	this->HitBoxWidth = std::move(other.HitBoxWidth);
	this->HitBoxHeight = std::move(other.HitBoxHeight);
	this->dRotation = std::move(other.dRotation);
	this->dFlipped = std::move(other.dFlipped);
	this->drawGloballyLast = std::move(other.drawGloballyLast);
}

ModuleObject & ModuleObject::operator=( const ModuleObject& other)
{
	this->name = other.name;
	this->pos = other.pos;
	this->HitBoxWidth = other.HitBoxWidth;
	this->HitBoxHeight = other.HitBoxHeight;
	this->dRotation = other.dRotation;
	this->dFlipped = other.dFlipped;
	this->drawGloballyLast = other.drawGloballyLast;
	return *this;
}
ModuleObject & ModuleObject::operator=( ModuleObject&& other)
{
	this->name = std::move(other.name);
	this->pos = std::move(other.pos);
	this->HitBoxWidth = std::move(other.HitBoxWidth);
	this->HitBoxHeight = std::move(other.HitBoxHeight);
	this->dRotation = std::move(other.dRotation);
	this->dFlipped = std::move(other.dFlipped);
	this->drawGloballyLast = std::move(other.drawGloballyLast);
	return *this;
}

ModuleObject ModuleObject::ReadHitBox(string content)
{
	string xPos = "";
	string yPos = "";
	string width = "";
	string height = "";
	string name;
	string type = "";
	string globallyLast = "";

	int i = 0;

	type = CommonReadUntilChar(':', content, i);
	type = ""; //note: type not implemented
	xPos = CommonReadUntilChar(',', content, i);
	yPos = CommonReadUntilChar(':', content, i);
	width = CommonReadUntilChar(',', content, i);
	height = CommonReadUntilChar(':', content, i);

	ModuleObject result;

	result.name = "hitbox";
	result.pos = Coord(std::atoi(xPos.c_str()), std::atoi(yPos.c_str()));
	result.HitBoxHeight = std::stoi(height);
	result.HitBoxWidth = std::stoi(width);
	if (type == "")
		result.HitBoxType = 0;
	else
		result.HitBoxType = std::stoi(type);
	return result;

}

ModuleObject ModuleObject::ReadModule(string content)
{
	string xPos = "";
	string yPos = "";
	string rotation = "";
	string flip = "";
	string name;
	string globallyLast = "";

	ModuleObject result;

	int i = 0;

	name = CommonReadUntilChar(':', content, i);
	xPos = CommonReadUntilChar(',', content, i);
	yPos = CommonReadUntilChar(':', content, i);
	rotation = CommonReadUntilChar(',', content, i);
	flip = CommonReadUntilChar(':', content, i);
	globallyLast = CommonReadUntilChar(':', content, i);

	result.name = name;
	result.pos = Coord(std::atoi(xPos.c_str()), std::atoi(yPos.c_str()));
	result.dRotation = enObjectDiscreteRotation(std::atoi(rotation.c_str()));
	result.dFlipped = enObjectFlipped(std::atoi(flip.c_str()));

	if (globallyLast == "1" || globallyLast == "drawlast")
		result.drawGloballyLast = true;
	return result;
}