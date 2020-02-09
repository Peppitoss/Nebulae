#pragma once


enum enObjectDiscreteRotation
{
	goNoRotation = 0,
	go90Rotation = 1,
	go180Rotation = 2,
	go270Rotation = 3
};

enum enObjectFlipped
{
	goNotFlipped = 0,
	goIsFlipped = 1
};

void IncrementDiscreteRotation(enObjectDiscreteRotation&);
void DecrementDiscreteRotation(enObjectDiscreteRotation&);
void ToggleFlipped(enObjectFlipped&);
