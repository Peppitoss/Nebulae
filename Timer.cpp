#include"Timer.h"

timer::timer()
{
	this->pFirstTick = SDL_GetTicks();
	this->pSecondTick = SDL_GetTicks();
	this->pTickSum = 0;
	this->pCurrentAverageTick = 0;
	this->pAverageCount = 0;
	this->pAverageTickSum = 0;
	this->pTicksUsedForAverageValue = 100;
}

Millisecond timer::GetStepTime()
{
	return Millisecond(this->pTickLength);
}

void timer::StartTick()
{
	this->pFirstTick = SDL_GetTicks();
	this->pSecondTick = SDL_GetTicks();
}

void timer::StopTick()
{
	this->pSecondTick = SDL_GetTicks();
	this->pTickLength = this->pSecondTick - this->pFirstTick;

	if (this->pAverageCount < this->pTicksUsedForAverageValue)
	{
		this->pAverageTickSum += this->pTickLength;// / this->pTicksUsedForAverageValue;
		this->pAverageCount++;
	}
	else
	{
		this->pCurrentAverageTick = this->pAverageTickSum / this->pTicksUsedForAverageValue;
		this->pAverageTickSum = 0;
		this->pAverageCount = 0;

	}
}

bool timer::TickSumEqual(Uint32 sum)
{
	return GetTickSum(sum, true) == 0;
}

Uint32 timer::GetTickSum(Uint32 sum, bool resetAtSumValue)
{
	if (this->pTickSum < sum)
	{
		TickRequest();
		this->pTickSum += this->pTickLength;
	}
	else
	{
		this->pFirstTick = SDL_GetTicks();
		if (resetAtSumValue)
			this->pTickSum = 0;
	}

	return pTickSum;
}

Millisecond timer::TickRequest()
{
	this->StopTick();
	this->StartTick();
	return Millisecond(this->pTickLength);
}


