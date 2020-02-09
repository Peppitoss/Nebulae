#pragma once
#include"LauncherEntity.h"

class LauncherProvider
{
public:

	LauncherProvider();

	virtual std::vector<std::shared_ptr<LauncherEntity>>::const_iterator const LE_begin() const = 0;
	virtual std::vector<std::shared_ptr<LauncherEntity>>::const_iterator const LE_end() const = 0;

};