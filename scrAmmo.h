#pragma once
#include"AmmoContainer.h"
#include"scrItem.h"

class scrAmmo : public ItemFrame
{
public:

	scrAmmo();
	scrAmmo(spriteSize, Coord pos);

	void SetupAmmoContainer(std::shared_ptr<AmmoContainer> container);
	void btnClose();

protected:


private:

	void Initialize();
};
