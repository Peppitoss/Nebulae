#pragma once

class Angle
{
public:
	Angle();
	Angle(const Angle&);
	Angle(const Angle&&);

	Angle & operator=(const Angle& other);
	Angle & operator=(const Angle&& other);

	Angle(double value, double relativeHeading);
	Angle(double relativeHeading);

	double value;
	double GetRelativeAngle();
	void SetRelativeAngle(double set);

private:
	double pRelativeAngle;	// i.e relativeangle 1.57, means that y is the heading
};