#include"GridTransfer.h"


GridTransfer::GridTransfer()
{
	pEmptySet = GridTransferObject();
}

void GridTransfer::ResetTransferData()
{
	this->pTransferData.clear();
}

//Screen* GridTransfer::GetPreviousScreenFromObject(std::shared_ptr<ITransferObject> obj)
//{
//	int index = this->GetTransferSetIndex(obj);
//	if (index < 0)
//		return nullptr;
//
//	if (this->pTransferData[index].IsEmpty())
//		return nullptr;
//
//	return this->pTransferData[index].GetLastTransfer().SourceScreen;
//}

GridTransferObject GridTransfer::GenerateTransfer(std::shared_ptr<ITransferObject> gridObject, GridObjectFrame* source, GridObjectFrame* destination, const Coord& position)
{
	GridTransferObject result = GridTransferObject(gridObject, source, destination, position);
	int index = this->GetTransferSetIndex(gridObject);
	
	if (index >= 0)
	{
		GridTransferSet& currentSet = this->pTransferData[index];
		if (!currentSet.IsEmpty() && currentSet.GetLastTransfer().SourceScreen == source)
		{
			if (currentSet.GetLastTransfer().DestinationScreen != destination)
				result.SourceScreen = currentSet.GetLastTransfer().DestinationScreen;
		}
	}
	return result;
}

//GridTransferObject result = GridTransferObject(gridObject, source, destination, position);
//int index = this->GetTransferSetIndex(gridObject);
//
//if (index >= 0)
//{
//	GridTransferSet& currentSet = this->pTransferData[index];
//	if (currentSet.GetLastTransfer().SourceScreen == source)
//	{
//		if (currentSet.GetLastTransfer().DestinationScreen != destination)
//			result.SourceScreen = currentSet.GetLastTransfer().DestinationScreen;
//	}
//}
//return result;

//void GridTransfer::SetTransferObjectOrigo(std::shared_ptr<ITransferObject>& object)
//{
//	if (object->getGridObject()->isPlacementOrigoDependent())
//		object->getGridObject()->SetNewGridOrigo(transfer.SourceScreen->GetGrid()->Origo());
//}

//void GridTransfer::Transfer(GridTransferObject& transfer, bool isTemporary)
//{
//	//if (transfer.Object->getGridObject()->isPlacementOrigoDependent())
//	//{
//	//	this->ExecuteTransfer(transfer.SetDestinationPosition(transfer.DestinationPosition - transfer.Object->getGridObject()->GridOrigo *0), isTemporary);
//	//	//if( this->ExecuteTransfer(transfer.SetDestinationPosition(transfer.DestinationPosition - transfer.Object->getGridObject()->GridOrigo*0), isTemporary) )
//	//		/*transfer.Object->getGridObject()->SetNewGridOrigo(transfer.DestinationScreen->GetGrid()->Origo());*/
//	//}
//	//else
//		this->ExecuteTransfer(transfer, isTemporary);
//}
//
//void GridTransfer::RevertTransfer(const GridTransferObject& transfer, bool IsTemporary)
//{
//	if (transfer.Object == nullptr)
//		return;
//
//	Transfer(transfer.GetReverseVariant(), IsTemporary);
//	//if (transfer.Object->isPlacementOrigoDependent())
//	//	transfer.Object->SetNewGridOrigo(transfer.SourceScreen->GetGrid()->Origo());
//	//this->ExecuteTransfer(transfer.GetReverseVariant(), IsTemporary);
//	RemoveTransfer(transfer);
//}

void GridTransfer::RevertTransfer(std::shared_ptr<ITransferObject> obj, bool IsTemporary)
{
	int index = GetTransferSetIndex(obj);
	if (index < 0)
		return;

	this->ExecuteTransfer(this->pTransferData[index].GetEntireTransferLineage().GetReverseVariant(), IsTemporary);
	//se
	//RevertTransfer(this->GetTransfer(obj), IsTemporary);
}

bool GridTransfer::ExecuteTransfer(const GridTransferObject& transfer, bool IsTemporary)
{
	bool AddResult = false;

	if (IsTemporary)
		AddResult = transfer.DestinationScreen->AddGridObject(transfer.Object, transfer.DestinationPosition); //continue here
	else
		AddResult = transfer.DestinationScreen->AddNewGridObject(transfer.Object, transfer.DestinationPosition); //was previously addnew grid object

	if (AddResult)
	{
		if( transfer.Object->getGridObject()->isPlacementOrigoDependent() )
			transfer.Object->getGridObject()->SetNewGridOrigo(transfer.DestinationScreen->GetGrid()->Origo());

		if (IsTemporary)
		{
			transfer.DestinationScreen->setSelectedObject(transfer.Object->getGridObject());
			transfer.DestinationScreen->SetFlagGridObject_SeperateGrid(1);
		}
		transfer.SourceScreen->RemoveGridObject(transfer.Object);
		AddTransferData(transfer);
		return true;
	}
	return false;

}

GridTransferObject GridTransfer::AddTransferData(std::shared_ptr<ITransferObject> gridObject, GridObjectFrame* source, GridObjectFrame* destination, const Coord& position)
{
	GridTransferObject newTransfer = this->GenerateTransfer(gridObject, source, destination, position);
	if (AddTransferData(newTransfer))
		return newTransfer;
	return GridTransferObject();
	
}

int GridTransfer::GetTransferSetIndex(std::shared_ptr<ITransferObject> object)
{
	for (int i = 0; i < this->pTransferData.size(); i++)
	{
		if (this->pTransferData[i].GetTransferObject() == object)
			return i;
	}
	return -1;
}

bool GridTransfer::AddTransferData(const GridTransferObject& transfer)
{
	int index = GetTransferSetIndex(transfer.Object);

	if (index < 0)
		this->pTransferData.push_back(GridTransferSet(transfer));	//add completely new transfer
	else
		this->pTransferData[index].AddTransfer(transfer,true);

	//for (int i = 0; i < this->pTransferData.size(); i++)
	//{
	//	if (transfer.TransferIsReversion(this->pTransferData[i]))
	//	{
	//		RemoveTransfer(this->pTransferData[i]);
	//		return true;
	//	}

	//	if (transfer.Object == this->pTransferData[i].Object)
	//	{
	//		if (transfer.SourceScreen == this->pTransferData[i].SourceScreen)
	//		{
	//			GridTransferObject pseudoTransfer = GridTransferObject(transfer);
	//			pseudoTransfer.SourceScreen = this->pTransferData[i].DestinationScreen;
	//			this->ExecuteTransfer(pseudoTransfer, true);

	//			this->pTransferData[i].DestinationScreen = transfer.DestinationScreen;
	//			return false;
	//		}
	//		else if (transfer.SourceScreen == this->pTransferData[i].DestinationScreen)
	//		{
	//			GridTransferObject pseudoTransfer = GridTransferObject(transfer);
	//			this->ExecuteTransfer(pseudoTransfer, true);
	//			this->pTransferData[i].DestinationScreen = transfer.SourceScreen;
	//			return false;
	//		}
	//		else
	//			return false;
	//	}

	//	//if (transfer.SourceScreen == this->pTransferData[i].SourceScreen && transfer.DestinationScreen == this->pTransferData[i].DestinationScreen)
	//	//	return false;




	//}
	//this->pTransferData.push_back(transfer);
	return true;
}

std::vector<GridTransferObject> GridTransfer::GetTransferWithSource(Screen* source)
{
	std::vector<GridTransferObject> result = std::vector<GridTransferObject>();
	for (int i = 0; i < this->pTransferData.size(); i++)
		if (this->pTransferData[i].GetTransferSource() == source)
			result.push_back(this->pTransferData[i].GetEntireTransferLineage());
	return result;
}

//GridTransferObject GridTransfer::GetTransfer(std::shared_ptr<ITransferObject> gridObject)
//{
//	GridTransferObject result = GridTransferObject();
//	for (int i = 0; i < this->pTransferData.size(); i++)
//		if (this->pTransferData[i].Object == gridObject)
//			result = this->pTransferData[i];
//	return result;
//}

//void GridTransfer::RemoveTransfer(const GridTransferObject& transfer)
//{
//	for (int i = 0; i < this->pTransferData.size(); i++)
//	{
//		if (transfer == this->pTransferData[i])
//		{
//			this->pTransferData.erase(this->pTransferData.begin() + i);
//			return;
//		}
//	}
//}

//bool GridTransfer::HasTransfer(const GridTransferObject& transfer)
//{
//	for (int i = 0; i < this->pTransferData.size(); i++)
//	{
//		if (transfer == this->pTransferData[i])
//			return true;
//	}
//	return false;
//}