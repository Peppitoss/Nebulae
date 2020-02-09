#pragma once
#include"InventoryItem.h"

//enum enAmmo
//{
//	Argus = 1, //cannon
//	Khalif = 2, //player torpedo
//	Messiah = 3, //large torpedo
//	Nablus = 4, //Torpedo
//	Jaffar = 5, // long range pierce 
//	Bragd = 6
//};

class ItemAmmo : public InventoryItem
{

public:

	//ItemAmmo(imagehandling::IMGToSurface* surface, string name);
	ItemAmmo(string ammo);
	//ItemAmmo(ItemAmmo&);
	//ItemAmmo(ItemAmmo&&);

	//ItemAmmo& operator=(const ItemAmmo& other);
	//ItemAmmo& operator=(const ItemAmmo&& other);

	void generateGridObjectDefaultToolTip() override;
	bool isValid() { return this->pType != InvalidText(); }

	string GetAmmoType() const;

	bool HasAmmo() const { return this->pAmmoAmount > 0; };
	void DecrementAmmo(int amount);
	void DecrementAmmo();
	void SetAmmoAmount(int amount);
	int GetAmmoAmount() const;


protected:

	//void initialize(GridObjectItem* graphicObject) override;

private:

	string InvalidText() { return "invalid"; }

	int pAmmoAmount;
	std::string pType;

};