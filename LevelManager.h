#pragma once
#include<string>
#include"ProgressData.h"

using std::string;


class LevelManager
{
public:

	LevelManager();

	std::vector<string> GetLevelsFromProgress(const ProgressData& data);


private:


};