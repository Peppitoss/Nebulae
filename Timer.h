#pragma once
#include"TimeUnit.h"
#include<SDL.h>

class timer
{

public:

	timer();
	void StartTick();
	void StopTick();

	Millisecond TickRequest();
	Millisecond GetStepTime();

	double  GetAverageTick() const { return this->pCurrentAverageTick; }
	
	bool TickSumEqual(Uint32 sum);
	Uint32 GetTickSum(Uint32 sum, bool resetAtSumValue);

private:

	double pTicksUsedForAverageValue;
	double pAverageTickSum;
	double pCurrentAverageTick;
	int pAverageCount;

	Uint32 pFirstTick;
	Uint32 pSecondTick;

	Uint32 pTickLength;
	Uint32 pTickSum;

};