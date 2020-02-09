#pragma once


class BoundedDouble
{
public:
	BoundedDouble();
	BoundedDouble(double value);
	BoundedDouble( double value, double limit);

	double value() const { return pValue; }
	void setvalue(double set);
	void increaseValue(double set);
	void ChangeValueByPercentage(double set);
	void setLimit(double set);

	BoundedDouble& operator=(const BoundedDouble&);
	BoundedDouble& operator=(BoundedDouble&&);

	BoundedDouble& operator=(const double&);
	BoundedDouble& operator+=(const double&);
	double operator+(const double&);
	double operator*(const double&);
	double operator/(const double&);
	double operator-(const double&);

private:

	//BoundedDouble& operator=(const BoundedDouble&) = delete;
	//BoundedDouble& operator=(BoundedDouble&&) = delete;

	void LimitValue( double& ref);

	double pValue;
	double avLimit;
};