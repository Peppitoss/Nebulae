#include"BoundedDouble.h"
#include<memory>

const double standardLimit = 5;

BoundedDouble::BoundedDouble()
{
	pValue = 0;
	avLimit = standardLimit;

}

BoundedDouble::BoundedDouble(double value)
{
	pValue = value;
	avLimit = standardLimit;
}

BoundedDouble::BoundedDouble(double value, double limit)
{
	pValue = value;
	avLimit = limit;

}

BoundedDouble& BoundedDouble::operator=(const BoundedDouble& other)
{
	this->pValue = other.pValue;
	this->avLimit = other.avLimit;
	return *this;
}
BoundedDouble& BoundedDouble::operator=(BoundedDouble&& other)
{
	this->pValue = std::move(other.pValue);
	this->avLimit = std::move(other.avLimit);
	return *this;
}

BoundedDouble& BoundedDouble::operator=(const double& val)
{
	this->setvalue(val);
	return *this;
}

BoundedDouble& BoundedDouble::operator+=(const double& val)
{
	this->setvalue( this->value() + val);
	return *this;
}

double BoundedDouble::operator+(const double& val)
{
	return this->value() + val;
}
double BoundedDouble::operator*(const double& val)
{
	return this->value() * val;
}

double BoundedDouble::operator/(const double& val)
{
	return this->value() / val;
}
double BoundedDouble::operator-(const double& val)
{
	return this->value() - val;
}

void BoundedDouble::LimitValue(double& ref)
{
	if ((ref < avLimit*-1 )|| (ref > avLimit))
		ref = avLimit;
}

void BoundedDouble::setLimit(double set)
{
	avLimit = set;
	LimitValue(pValue);
}

void BoundedDouble::setvalue(double set)
{
	LimitValue(set);
	pValue = set;
}

void BoundedDouble::increaseValue(double set)
{
	pValue += set;
	LimitValue(pValue);
}

void BoundedDouble::ChangeValueByPercentage(double set)
{
	pValue *= set;
	LimitValue(pValue);
}