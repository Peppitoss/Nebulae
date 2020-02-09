#include"AmmoContainer.h"
#include"StringBuilder.h"

AmmoContainer::AmmoContainer() : ModuleEntity()
{
	initialize();
}

AmmoContainer::AmmoContainer(std::shared_ptr<FiguredGraphics> ge, string name) : ModuleEntity(ge, name)
{
	initialize();
}

void AmmoContainer::generateGridObjectDefaultToolTip()
{
	this->generateGridObjectToolTip(GetToolTip());
}

int AmmoContainer::AmmoAmount(std::string type) const
{
	int result = 0;
	for (int i = 0; i < pAmmo.size(); i++)
	{
		if (pAmmo[i]->GetAmmoType() == type)
			result += pAmmo[i]->GetAmmoAmount();
	}
	return result;
}

void AmmoContainer::refreshState()
{
	ModuleEntity::refreshState();
	generateGridObjectDefaultToolTip();
	for (int i = 0; i < pAmmo.size(); i++)
	{
		pAmmo[i]->generateGridObjectDefaultToolTip();
	}
	//for (int i = 0; i < pAmmo.size(); i++)
	//{
	//	pAmmoCounter[pAmmo[i]->GetAmmoType()] += pAmmo[i]->GetAmmoAmount();
	//}
	

}

bool AmmoContainer::DecrementAmmo(std::string type, int amount)
{
	for (int i = 0; i < pAmmo.size(); i++)
	{
		if (pAmmo[i]->GetAmmoType() == type && pAmmo[i]->GetAmmoAmount() >= amount)
		{
			pAmmo[i]->DecrementAmmo(amount);
			return true;
		}
	}
	return false;
}
bool AmmoContainer::DecrementAmmo(std::string type)
{
	for (int i = 0; i < pAmmo.size(); i++)
	{
		if (pAmmo[i]->GetAmmoType() == type && pAmmo[i]->HasAmmo() )
		{
			pAmmo[i]->DecrementAmmo();
			return true;
		}
	}
	return false;
}

void AmmoContainer::AddAmmo(std::shared_ptr<ItemAmmo> ammo, Coord pos)
{
	if (HasAmmo(ammo))
		return;
	ammo->getGridObject()->setProperPosition(pos);
	pAmmo.push_back(ammo);

}

bool AmmoContainer::RemoveAmmo(std::shared_ptr<ItemAmmo> ammo)
{
	for (auto it = pAmmo.begin(); it != this->pAmmo.end(); ++it)
	{
		if ((*it) == ammo)
		{
			pAmmo.erase(it);
			return true;
		}
	}
	return false;
}

bool AmmoContainer::HasAmmo(std::shared_ptr<ItemAmmo> ammo)
{
	//Unsure if implemented correct, comparing memory is good enough??
	for (int i = 0; i < pAmmo.size(); i++)
	{
		if (pAmmo[i] == ammo)
			return true;	
	}
	return false;
}

bool AmmoContainer::HasAmmo(std::shared_ptr<GridObject> ammo)
{
	for (int i = 0; i < pAmmo.size(); i++)
	{
		if (pAmmo[i]->getGridObject() == ammo)
			return true;
	}
	return false;
}

void AmmoContainer::ClearAmmoList()
{
	for (int i = 0; i < pAmmo.size(); i++)
		pAmmo[i].reset();
	pAmmo.clear();
}

std::string AmmoContainer::GetToolTip()
{
	std::string ID = GetIDName();
	std::string strAmmo = "";
	for (int i = 0; i < pAmmo.size(); i++)
	{
		strAmmo += pAmmo[i]->GetAmmoType() + ": " + IntToString(pAmmo[i]->GetAmmoAmount()) + "\n";
	}
	return ID + "\n" + strAmmo;
}

void AmmoContainer::initialize()
{
	this->SetModuleType(enModuleType::mAmmoContainer);
	//pAmmoCounter = std::map<enAmmo, int>();

}

Coord AmmoContainer::GetContainerSize() const
{
	return this->pContainerSize;
}
void AmmoContainer::SetContainerSize(const Coord& set)
{
	this->pContainerSize = set;
}