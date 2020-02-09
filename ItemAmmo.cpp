#include"ItemAmmo.h"
#include"GridObjectItem.h"
#include"ProjectileGlobalList.h"
#include"SurfaceGlobalList.h"
#include"StringBuilder.h"

//ItemAmmo::ItemAmmo(imagehandling::IMGToSurface* surface, string name)
//{
//	this->pType = name;
//	initialize(new GridObjectItem(surface), name);
//}

ItemAmmo::ItemAmmo(string ammo) : InventoryItem( enItemType::ProjectileItem)
{
	if (ProjectileGlobalList::HasInterface(ammo))
	{
		auto projectileInterface = ProjectileGlobalList::GetInterface(ammo);
		this->pType = ammo;
		this->SetPrice(projectileInterface.GetPrice());

		auto surface = SurfaceGlobalList::GetSurface(projectileInterface.GetSurfaceName() + "_");
		initialize(new GridObject(surface.get()));
	}
	else
		this->pType = InvalidText();
	this->pAmmoAmount = 1;
}

void ItemAmmo::DecrementAmmo(int amount)
{
	this->pAmmoAmount -= amount;
	if (this->pAmmoAmount < 0)
		this->pAmmoAmount = 0;
	
}
void ItemAmmo::DecrementAmmo()
{
	this->pAmmoAmount--;
	if (this->pAmmoAmount < 0)
		this->pAmmoAmount = 0;
}

void ItemAmmo::SetAmmoAmount(int amount)
{
	this->pAmmoAmount = amount;
}
int ItemAmmo::GetAmmoAmount() const
{
	return this->pAmmoAmount;
}

void ItemAmmo::generateGridObjectDefaultToolTip()
{
	string tooltip = this->pType + " x" + IntToString(this->GetAmmoAmount());
	generateGridObjectToolTip(tooltip);
}

string ItemAmmo::GetAmmoType() const
{
	return this->pType;
}