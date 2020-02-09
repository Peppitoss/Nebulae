#include"RandomNumberGenerator.h"


std::default_random_engine & RANDGENERATOR()
{
	static std::default_random_engine e(60920115);
	return e;
}