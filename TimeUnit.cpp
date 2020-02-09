#include"TimeUnit.h"

Millisecond::Millisecond(int value)
{
	Value = value;
}

Millisecond::Millisecond(const Second& second)
{
	Value = second.Value * 1000;
}


Second::Second(int value)
{
	Value = value;
}
