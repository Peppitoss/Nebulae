#include"GridTransferObject.h"


GridTransferSet::GridTransferSet()
{
	std::vector<GridTransferObject> Set = std::vector<GridTransferObject>();
	std::shared_ptr<ITransferObject> pObject = nullptr;
}

GridTransferSet::GridTransferSet(GridTransferObject transferData)
{
	pSet.push_back(transferData);
	pTransferObject = transferData.Object;
}

std::shared_ptr<ITransferObject> GridTransferSet::GetTransferObject()
{
	if (this->pTransferObject == nullptr)
		return nullptr;
	else
		return this->pTransferObject;
}

Screen* GridTransferSet::GetTransferSource()
{
	if (this->pSet.size() == 0)
		return nullptr;
	else
		return this->GetFirstTransfer().SourceScreen;
}

//GridTransferObject GridTransferSet::GetTransferLineageWithSource(Screen* source)
//{
//	std::vector<GridTransferObject> result = std::vector<GridTransferObject>();
//	if (this->GetFirstTransfer().SourceScreen == source)
//		result.push_back(this->GetEntireTransferLineage());
//	return result;
//}
GridTransferObject GridTransferSet::GetEntireTransferLineage()
{
	auto first = GetFirstTransfer();
	auto last = GetLastTransfer();

	GridTransferObject result = GridTransferObject(first);
	result.DestinationScreen = last.DestinationScreen;
	return result;
}
GridTransferObject GridTransferSet::GetLastTransfer()
{
	return pSet[pSet.size() - 1];
}

GridTransferObject GridTransferSet::GetFirstTransfer()
{
	return pSet[0];
}

void GridTransferSet::AddTransfer(GridTransferObject transfer, bool eraseCyclicTransfers)
{
	auto transferObject = this->GetTransferObject();

	if (transferObject == nullptr)
		return;

	if (transferObject == transfer.Object)
	{
		pSet.push_back(transfer);
		if (eraseCyclicTransfers)
			EraseCyclicTransfer(transfer);
	}
}

void GridTransferSet::EraseCyclicTransfer(GridTransferObject transfer)
{
	std::vector<GridTransferObject> newSet = std::vector<GridTransferObject>();

	for (int i = 0; i < this->pSet.size(); i++)
	{
		if (pSet[i].SourceScreen != transfer.DestinationScreen)
			newSet.push_back(pSet[i]);
		else
			break;
	}
	this->pSet = newSet;
}

void GridTransferSet::RemoveLastTransfer()
{
	pSet.pop_back();
}


GridTransferObject::GridTransferObject()
{
	Object = nullptr;
	SourceScreen = nullptr;
	DestinationScreen = nullptr;
	SourcePosition = Coord(0);
	DestinationPosition = Coord(0);
}

GridTransferObject::GridTransferObject(std::shared_ptr<ITransferObject> object, GridObjectFrame* source, GridObjectFrame* destination, Coord destinationpos)
{
	Object = object;
	SourceScreen = source;
	DestinationScreen = destination;
	DestinationPosition = destinationpos;
	SourcePosition = object->getGridObject()->getLastPlacedPosition();
}

bool GridTransferObject::operator==(const GridTransferObject& other) const
{
	return this->SourceScreen == other.SourceScreen &&
		this->DestinationScreen == other.DestinationScreen &&
		this->SourcePosition == other.SourcePosition &&
		this->DestinationPosition == other.DestinationPosition;
}

bool GridTransferObject::TransferIsReversion(const GridTransferObject& other) const
{
	return this->SourceScreen == other.DestinationScreen &&
		this->DestinationScreen == other.SourceScreen;
}

GridTransferObject GridTransferObject::GetReverseVariant() const
{
	GridTransferObject result = GridTransferObject();
	result.Object = this->Object;
	result.SourceScreen = this->DestinationScreen;
	result.DestinationScreen = this->SourceScreen;
	result.SourcePosition = this->DestinationPosition;
	result.DestinationPosition = this->SourcePosition;
	return result;
}

GridTransferObject& GridTransferObject::SetSourcePosition(const Coord newPosition)
{
	this->SourcePosition = newPosition;
	return *this;
}

GridTransferObject& GridTransferObject::SetDestinationPosition(const Coord newPosition)
{
	this->DestinationPosition = newPosition;
	return *this;
}
