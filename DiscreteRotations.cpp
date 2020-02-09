#include"DiscreteRotations.h"


void IncrementDiscreteRotation(enObjectDiscreteRotation& reference)
{
	switch (reference)
	{
	case(enObjectDiscreteRotation::goNoRotation):
		reference = enObjectDiscreteRotation::go90Rotation;
		break;
	case(enObjectDiscreteRotation::go90Rotation):
		reference = enObjectDiscreteRotation::go180Rotation;
		break;
	case(enObjectDiscreteRotation::go180Rotation):
		reference = enObjectDiscreteRotation::go270Rotation;
		break;
	case(enObjectDiscreteRotation::go270Rotation):
		reference = enObjectDiscreteRotation::goNoRotation;
		break;
	default:
		break;
	}
}
void DecrementDiscreteRotation(enObjectDiscreteRotation& reference)
{
	switch (reference)
	{
	case(enObjectDiscreteRotation::goNoRotation):
		reference = enObjectDiscreteRotation::go270Rotation;
		break;
	case(enObjectDiscreteRotation::go90Rotation):
		reference = enObjectDiscreteRotation::goNoRotation;
		break;
	case(enObjectDiscreteRotation::go180Rotation):
		reference = enObjectDiscreteRotation::go90Rotation;
		break;
	case(enObjectDiscreteRotation::go270Rotation):
		reference = enObjectDiscreteRotation::go180Rotation;
		break;
	default:
		break;
	}
}
void ToggleFlipped(enObjectFlipped& reference)
{
	switch (reference)
	{
	case(enObjectFlipped::goIsFlipped):
		reference = enObjectFlipped::goNotFlipped;
		break;
	case(enObjectFlipped::goNotFlipped):
		reference = enObjectFlipped::goIsFlipped;
		break;
	default:
		break;
	}
}