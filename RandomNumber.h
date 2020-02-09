#pragma once

namespace statistics
{

	class RandomNumber	//for normal distribution
	{
	public:

		RandomNumber();
		RandomNumber(double average, double variance);

		RandomNumber operator+(const RandomNumber& b) const;
		RandomNumber& operator+=(const RandomNumber& b);

		RandomNumber operator-(const RandomNumber& b) const;
		RandomNumber& operator-=(const RandomNumber& b);

		double GetRandomValue() const;
		void SetVariance(double var);
		void SetStandardDeviation(double std);

		double Average;

	private:

		double pVariance;
		double pStandardDeviation;

	};

}
