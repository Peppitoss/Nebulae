#include"Angle.h"
#include<memory>

Angle::Angle()
{
	this->value = 0;
	this->pRelativeAngle = 0;
}

Angle::Angle(double value, double relativeHeading)
{
	this->value = value;
	this->pRelativeAngle = relativeHeading;
}

Angle::Angle(double relativeHeading)
{
	this->value = 0;
	this->pRelativeAngle = relativeHeading;
}

Angle::Angle(const Angle& other)
{
	this->pRelativeAngle = other.pRelativeAngle;
	this->value = other.value;
}
Angle::Angle(const Angle&& other)
{
	this->pRelativeAngle = std::move(other.pRelativeAngle);
	this->value = std::move(other.value);
}

Angle & Angle::operator=(const Angle& other)
{
	this->pRelativeAngle = other.pRelativeAngle;
	this->value = other.value;
	return *this;
}
Angle & Angle::operator=(const Angle&& other)
{
	this->pRelativeAngle = std::move(other.pRelativeAngle);
	this->value = std::move(other.value);
	return *this;
}

double Angle::GetRelativeAngle()
{
	return this->pRelativeAngle;
}
void Angle::SetRelativeAngle(double set)
{
	this->value = set;
}
