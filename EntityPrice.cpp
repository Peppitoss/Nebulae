#include"EntityPrice.h"


EntityPrice::EntityPrice()
{
	pPriceValue = statistics::RandomNumber(0, 0);
}

EntityPrice::EntityPrice(const statistics::RandomNumber& value)
{
	pPriceValue = value;
}

void EntityPrice::IncreaseSellValue(const statistics::RandomNumber& amount)
{
	pPriceValue += amount;
}
void EntityPrice::DecreaseellValue(const statistics::RandomNumber& amount)
{
	pPriceValue -= amount;
}