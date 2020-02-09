#include"scrAmmo.h"
#include"GameManager.h""

scrAmmo::scrAmmo() : ItemFrame()
{
	Initialize();
}
scrAmmo::scrAmmo(spriteSize size, Coord pos) : ItemFrame(size, pos)
{
	Initialize();
}

void scrAmmo::SetupAmmoContainer(std::shared_ptr<AmmoContainer> container)
{
	//this->GetAmmoContainer()->r
	this->ResizeGrid(container->GetContainerSize());
	for (auto it = container->abegin(); it != container->aend(); ++it)
	{
		this->GetGrid()->AddNewItem((*it));//  ((*it)->getGridObject());
	}

}

void scrAmmo::Initialize()
{
	this->GetGrid()->SetAllowedItemType(enItemType::ProjectileItem);
}

void scrAmmo::btnClose()
{
	GameManager::Screens.closeAmmoScreen();
}