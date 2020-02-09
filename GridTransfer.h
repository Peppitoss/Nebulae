#pragma once
#include"GridTransferObject.h"

//enum enTransferValidity
//{
//	trNormal = 1,
//	trDuplicate = 2,
//	trReversion
//
//};

class GridTransfer
{

public:
	GridTransfer();



	std::vector<GridTransferObject> GetTransferWithSource(Screen* source);
	//GridTransferObject GetTransfer(std::shared_ptr<ITransferObject> gridObject);

	GridTransferObject GenerateTransfer(std::shared_ptr<ITransferObject> gridObject, GridObjectFrame* source, GridObjectFrame* destination, const Coord& position);
	void ResetTransferData();

	//Screen* GetPreviousScreenFromObject(std::shared_ptr<ITransferObject> obj);

	//void RevertTransfer(const GridTransferObject&, bool IsTemporary);
	bool ExecuteTransfer(const GridTransferObject&, bool TransferIsTemporary);
	void RevertTransfer(std::shared_ptr<ITransferObject> obj, bool IsTemporary);
	//void Transfer(GridTransferObject&, bool isTemporary);

	

private:

	std::vector<GridTransferSet> pTransferData;
	//bool HasTransfer(const GridTransferObject&);

	
	//void SetTransferObjectOrigo(std::shared_ptr<ITransferObject>&);
	

	bool AddTransferData(const GridTransferObject& transfer);
	GridTransferObject AddTransferData(std::shared_ptr<ITransferObject>, GridObjectFrame* source, GridObjectFrame* destination, const Coord& position);

	int GetTransferSetIndex(std::shared_ptr<ITransferObject> object);

	GridTransferSet pEmptySet;
	//void RemoveTransfer(const GridTransferObject&);


};
