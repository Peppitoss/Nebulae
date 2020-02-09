#pragma once
#include"RandomNumber.h"

class EntityPrice
{
public:

	EntityPrice();
	EntityPrice(const statistics::RandomNumber& value);

	void SetNewPrice(statistics::RandomNumber newValue) { this->pPriceValue = newValue; }
	statistics::RandomNumber PriceValue() { return this->pPriceValue; }
	statistics::RandomNumber SellValue() const { return statistics::RandomNumber(this->pPriceValue.Average*0.25, 0); }
	double Value() const { return this->pPriceValue.Average; }

	void IncreaseSellValue(const statistics::RandomNumber& amount);
	void DecreaseellValue(const statistics::RandomNumber& amount);

private:

	statistics::RandomNumber pPriceValue;

		
};