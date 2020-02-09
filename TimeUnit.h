#pragma once
#include<utility>

class Second
{

public:

	Second() { Value = 0; }
	Second(int value);

	Second& operator-=(const Second& other) { this->Value -= other.Value; return *this; };
	Second& operator+=(const Second& other) { this->Value += other.Value; return *this; };

	bool operator>(const Second& other) { return this->Value > other.Value; }
	bool operator<(const Second& other) { return this->Value < other.Value; }

	bool operator>=(const Second& other) { return this->Value >= other.Value; }
	bool operator<=(const Second& other) { return this->Value <= other.Value; }

	double Value;

private:

};

class Millisecond
{
public:

	Millisecond() { Value = 0; }
	Millisecond(int value);
	Millisecond(const Second& second);

	Millisecond& operator-=(const Millisecond& other) {this->Value -= other.Value; return *this; };
	Millisecond& operator+=(const Millisecond& other) { this->Value += other.Value; return *this; };

	bool operator>(const Millisecond& other) { return this->Value > other.Value; }
	bool operator<(const Millisecond& other) { return this->Value < other.Value; }

	bool operator>=(const Millisecond& other) { return this->Value >= other.Value; }
	bool operator<=(const Millisecond& other) { return this->Value <= other.Value; }

	Second ToSecond() { return Value / 1000; }

	double Value;

private:

};


//
//template<int F> using Millisecond = TimeUnit<1000>;
//template<int F> using Second = TimeUnit<1>;//typedef TimeUnit<1> Second;

