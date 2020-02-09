#pragma once
#include"scrGridObject.h"
#include"GridObject.h"

//enum enTransferValidity
//{
//	trNormal = 1,
//	trDuplicate = 2,
//	trReversion
//
//};

struct GridTransferObject;

class GridTransferSet
{

public:

	GridTransferSet();
	GridTransferSet(GridTransferObject data);

	GridTransferObject GetEntireTransferLineage();

	GridTransferObject GetLastTransfer();
	GridTransferObject GetFirstTransfer();

	Screen* GetTransferSource();
	std::shared_ptr<ITransferObject> GetTransferObject();

	void AddTransfer(GridTransferObject transfer, bool eraseCyclicTransfers);
	void RemoveLastTransfer();

	bool IsEmpty() { return this->pSet.size() == 0; }

private:

	void EraseCyclicTransfer(GridTransferObject transfer);
	std::vector<GridTransferObject> pSet;
	std::shared_ptr<ITransferObject> pTransferObject;

};

struct GridTransferObject
{

public:

	GridTransferObject();
	GridTransferObject(std::shared_ptr<ITransferObject> object, GridObjectFrame* source, GridObjectFrame* destination, Coord destinationpos);

	bool operator==(const GridTransferObject&) const;
	bool TransferIsReversion(const GridTransferObject&) const;

	GridTransferObject& SetSourcePosition(const Coord newPosition);
	GridTransferObject& SetDestinationPosition(const Coord newPosition);

	GridTransferObject GetReverseVariant() const;

	std::shared_ptr<ITransferObject> Object;
	GridObjectFrame* SourceScreen;	//should be weak ptr
	GridObjectFrame* DestinationScreen; //should be weak ptr
	Coord SourcePosition;
	Coord DestinationPosition;

};

//template class GridTransferObject<GridObject>;
//template class GridTransferObject<InventoryItem>;