#include"RandomNumber.h"
#include"RandomNumberGenerator.h"

namespace statistics
{
	RandomNumber::RandomNumber()
	{
		this->Average = 0;
		this->SetVariance(0);
	}

	RandomNumber::RandomNumber(double average, double variance)
	{
		this->Average = average;
		this->SetVariance(variance);
	}

	double RandomNumber::GetRandomValue() const
	{
		std::normal_distribution<double> nDistribution(Average, this->pStandardDeviation);
		return nDistribution(RANDGENERATOR());
	}

	void RandomNumber::SetVariance(double var)
	{
		if (var == 0)
			var = 0.00001;

		if (this->pVariance != var)
		{
			this->pVariance = var;
			this->pStandardDeviation = std::sqrt(this->pVariance);
		}
	}
	void RandomNumber::SetStandardDeviation(double std)
	{
		if (this->pStandardDeviation != std)
		{
			this->pVariance = this->pStandardDeviation*this->pStandardDeviation;
			this->pStandardDeviation = std;
		}
	}

	RandomNumber RandomNumber::operator+(const RandomNumber& other) const
	{
		return RandomNumber(this->Average + other.Average, this->pVariance + other.pVariance);
	}
	RandomNumber& RandomNumber::operator+=(const RandomNumber &other)
	{
		*this = RandomNumber(this->Average + other.Average, this->pVariance + other.pVariance);
		return *this;
	}

	RandomNumber RandomNumber::operator-(const RandomNumber& other) const
	{
		return RandomNumber(this->Average - other.Average, this->pVariance - other.pVariance);
	}
	RandomNumber& RandomNumber::operator-=(const RandomNumber &other)
	{
		*this = RandomNumber(this->Average - other.Average, this->pVariance - other.pVariance);
		return *this;
	}

}