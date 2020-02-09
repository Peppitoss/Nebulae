#include"Second.h"





Second::Second() : TimeUnit()
{

}

Second::Second(double seconds) : TimeUnit(seconds)
{

}
//
//Second& Second::operator=(const Millisecond& other)
//{
//	this->pValue = other.ToSecond().GetValue();
//	return *this;
//}
//
//bool Second::operator>(const Millisecond& other)
//{
//	return this->ToMillisecond().GetValue() > other.GetValue();
//}
//bool Second::operator<(const Millisecond& other)
//{
//	return this->ToMillisecond().GetValue() < other.GetValue();
//}
//
//
