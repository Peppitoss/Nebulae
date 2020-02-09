#pragma once
#include"Graphics.h"

struct Kinematics
{
	Kinematics();

	primitives::vector CurrentVelocity;
	double CurrentAngularSpeed;
	double CurrentHeading;
};