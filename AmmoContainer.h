#pragma once
#include"ModuleEntity.h"
#include"ItemAmmo.h"


class AmmoContainer : public ModuleEntity
{

public:

	AmmoContainer();
	AmmoContainer(std::shared_ptr<FiguredGraphics> ge, string name);

	int AmmoAmount(std::string type) const;
	void refreshState() override;

	bool DecrementAmmo(std::string type, int amount);
	bool DecrementAmmo(std::string type);

	void AddAmmo(std::shared_ptr<ItemAmmo> ammo, Coord pos);
	bool RemoveAmmo(std::shared_ptr<ItemAmmo> ammo);
	bool HasAmmo(std::shared_ptr<ItemAmmo> ammo);
	bool HasAmmo(std::shared_ptr<GridObject> ammo);
	//void RemoveAmmo(std::shared_ptr<ItemAmmo> ammo);
	void ClearAmmoList();

	Coord GetContainerSize() const;
	void SetContainerSize(const Coord& set);

	std::vector<std::shared_ptr<ItemAmmo>>::iterator const abegin() { return pAmmo.begin(); };
	std::vector<std::shared_ptr<ItemAmmo>>::iterator const aend() { return pAmmo.end(); };

protected:

	void generateGridObjectDefaultToolTip() override;

private:
	      
	Coord pContainerSize;
	std::vector<std::shared_ptr<ItemAmmo>> pAmmo;
	//std::map<enAmmo, int> pAmmoCounter;
	void initialize();
	std::string GetToolTip();
};