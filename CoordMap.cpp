#include"CoordMap.h"
#include"PixelCollection.h"
#include"PixelFigure.h"
#include"Sector.h"
#include"Grid.h"
#include"RandomWalk.h"
#include <algorithm>

template <typename T> CoordMapItem<T>::CoordMapItem()
{
	Status = enCoordStatus::csEmpty;
	//Item.reset();
}

template <typename T> CoordMapItem<T>::CoordMapItem( const CoordMapItem& other )
{
	if( other.Item )
		this->Item = std::shared_ptr<T>( new T( *other.Item.get() ) );
	this->Status = other.Status;
}
template <typename T> CoordMapItem<T>::CoordMapItem( const CoordMapItem&& other )
{
	this->Item = std::move(other.Item);
	this->Status = std::move(other.Status);
}

template <typename T> CoordMapItem<T> & CoordMapItem<T>::operator=( const CoordMapItem<T>& other )
{
	if( other.Item )
		this->Item = std::shared_ptr<T>( new T(*other.Item.get()) );
	this->Status = other.Status;
	return *this;
}

template <typename T> CoordMapItem<T> & CoordMapItem<T>::operator=( const CoordMapItem<T>&& other )
{
	this->Item = std::move(other.Item);
	this->Status = std::move(other.Status);
	return *this;
}

template <typename T> void CoordMapItem<T>::InsertBox( std::shared_ptr<T> box )
{
	Item = box;
	Status = enCoordStatus::csOccupy;
}




template <class T> CoordMapRow<T>::CoordMapRow() : pSize(0), pColumns(nullptr)
{

}


template <class T> CoordMapRow<T>::CoordMapRow( int size )  : pSize(size), pColumns( new CoordMapItem<T>[pSize] )
{
	//for( int i = 0; i < size; i++ )
	//{
	//	pColumns[i] = CoordMapItem<T>();
	//}
}

template <class T> CoordMapItem<T>& CoordMapRow<T>::GetColumnItem( int column )
{
	return pColumns[column];
}

template <class T> CoordMapRow<T>::CoordMapRow( const CoordMapRow& other ) : pSize(other.pSize) , pColumns( new CoordMapItem<T>[other.pSize] )
{
	std::copy(other.pColumns, other.pColumns + pSize, pColumns);  
}
template <class T> CoordMapRow<T>::CoordMapRow( CoordMapRow&& other ) : pColumns(nullptr), pSize(0)
{
	*this = std::move(other);  
}

template <class T> CoordMapRow<T> & CoordMapRow<T>::operator=( const CoordMapRow<T>& other )
{
	if( this != &other )
	{
		delete[] pColumns;
		this->pSize = other.pSize;
		pColumns = new CoordMapItem<T>[pSize];
		std::copy(other.pColumns, other.pColumns + pSize, pColumns);  
	}
	return *this;
}

template <class T> CoordMapRow<T> & CoordMapRow<T>::operator=( CoordMapRow<T>&& other )
{
	if( this != &other )
	{
		delete[] pColumns;
		this->pColumns = other.pColumns;
		this->pSize = std::move(other.pSize);
		other.pColumns = nullptr;
	}

	return *this;
}

template <class T> std::shared_ptr<T> & CoordMapRow<T>::operator[](int index)
{
	return this->pColumns[index].Item;
}

template <class T> void CoordMapRow<T>::AddItem( std::shared_ptr<T> box, int column )
{
	pColumns[column].Item = box;
	pColumns[column].Status = enCoordStatus::csOccupy;
}

template <class T> void CoordMapRow<T>::EraseAll()
{
	for( int i = 0; i<this->pSize; i++ )
	{
		EraseBox(i);
	}
}

template <class T> void CoordMapRow<T>::EraseBox( int column )
{
	//CoordMapItem<T> item = GetColumnItem(column);
	pColumns[column].Item.reset();
	pColumns[column].Status = enCoordStatus::csEmpty;
}

template <class T> void CoordMapRow<T>::ResetColumns()
{
	if( pColumns != nullptr )
	{
		delete[] pColumns;
		pColumns = nullptr;
	}
}

template <class T> void CoordMapRow<T>::SetBoxFilled( int column ) 
{
	pColumns[column].Status = enCoordStatus::csOccupy;
}

template <class T> bool CoordMapRow<T>::ColumnIsFilled( int column ) const
{
	return pColumns[column].Status == enCoordStatus::csOccupy;
}

template <class T> CoordMapRow<T>::~CoordMapRow()
{
	ResetColumns();
}





template <class T> CoordMap<T>::CoordMap()
{
	pDimension = 0;
	pOptimizedDimension = Coord(0);
	pInsertedBoxes = 0;
	pTopLeftZeroPoint = Coord(0);
	pBottomRightMaxPoint = Coord(0);
	pIsOptimized = false;
	pRows = nullptr;
}

template <class T> CoordMap<T>::CoordMap( int size )
{
	pDimension = size;
	pOptimizedDimension = Coord(size);
	pInsertedBoxes = 0;
	pRows = new CoordMapRow<T>[size];
	//pRows.reserve( size );
	for( int i = 0; i < size; i++ )
		pRows[i] = CoordMapRow<T>(size);
	pTopLeftZeroPoint = Coord(0);
	pBottomRightMaxPoint = Coord(pDimension);
	pIsOptimized = false;
}

template <class T> CoordMap<T>::CoordMap( const CoordMap<T>& other)
{
	this->pDimension = other.pDimension;
	
	pRows = new CoordMapRow<T>[pDimension];
	std::copy(other.pRows, other.pRows + pDimension, pRows);

	//this->pRows = other.pRows;
	this->pInsertedBoxes = other.pInsertedBoxes;
	this->pTopLeftZeroPoint = other.pTopLeftZeroPoint;
	this->pBottomRightMaxPoint = other.pBottomRightMaxPoint;
	this->pIsOptimized = other.pIsOptimized;
	this->pOptimizedDimension = other.pOptimizedDimension;
}
template <class T> CoordMap<T>::CoordMap( CoordMap<T>&& other)
{
	this->pDimension = std::move(other.pDimension);

	this->pRows = other.pRows;
	other.pRows = nullptr;

	//this->pRows = std::move(other.pRows);
	this->pInsertedBoxes = std::move(other.pInsertedBoxes);
	this->pTopLeftZeroPoint = std::move(other.pTopLeftZeroPoint);
	this->pBottomRightMaxPoint = std::move(other.pBottomRightMaxPoint);
	this->pIsOptimized = std::move(other.pIsOptimized);
	this->pOptimizedDimension = std::move(other.pOptimizedDimension);
}


template <class T> const CoordMapItem<T>& CoordMap<T>::GetItem( const Coord& Coord) const
{
	if( this->pBottomRightMaxPoint.y > Coord.y )
		return pRows[Coord.y].GetColumnItem( Coord.x );
	return CoordMapItem<T>();
}

template <class T> const CoordMapItem<T>& CoordMap<T>::GetItem( int index ) const
{
	return GetItem( this->CoordFromIndex(index) );
}

template <class T> std::shared_ptr<T> & CoordMap<T>::operator[](const Coord& Coord)
{
	return this->pRows[Coord.y][Coord.x];
}

template <class T> std::shared_ptr<T> & CoordMap<T>::operator[](int index)
{
	if( index > std::pow(this->Size(),2) )
		return std::shared_ptr<T>();

	return (*this)[CoordFromIndex(index)];
}

template <class T> Coord CoordMap<T>::CoordFromIndex(int i ) const
{
	//int row = i/pDimension;
	//int column = i%pDimension;
	//int row = (this->GetMinCoordValues().y + i)/(this->GetMaxCoordValues().y+1);
	//int column = ( this->GetMinCoordValues().x + i)%(this->GetMaxCoordValues().x+1);
	Coord rectColSize = pOptimizedDimension;
	int row = i/(rectColSize.x) + pTopLeftZeroPoint.y;
	int column = i%(rectColSize.x) + pTopLeftZeroPoint.x;
	return Coord(column,row);
}

template <class T> void CoordMap<T>::OverrideBox( std::shared_ptr<T> box, const Coord& Coord )
{
	if( !this->CoordIsFilled( Coord ) )
		pInsertedBoxes++;

	pRows[Coord.y].AddItem( box, Coord.x );
	
}

template <class T> bool CoordMap<T>::AllBoxesFilled()
{
	return this->pInsertedBoxes == this->IndexSize();
}
template <class T> double CoordMap<T>::FillPercent()
{
	return (double)this->pInsertedBoxes / this->IndexSize();
}



template <class T> void CoordMap<T>::Clear()
{
	if(IsOptimized())
		return;

	for( int i = 0; i < this->pDimension; i++ )
	{
		pRows[i].EraseAll();
	}
	pInsertedBoxes = 0;
}

template <class T> void CoordMap<T>::ResetSize()
{
	if(IsOptimized())
		return;

	this->Clear();
	for( int i = 0; i < this->pDimension; i++ )
	{
		pRows[i].ResetColumns();
	}
	delete[] pRows;
	pRows = nullptr;
	//pRows.clear();
}

template <class T> void CoordMap<T>::Erase( const Coord& Coord )
{
	if(IsOptimized())
		return;

	pRows[Coord.y].EraseBox(Coord.x);
	pInsertedBoxes--;
}
template <class T> int CoordMap<T>::IndexSize() const 
{
	return this->pOptimizedDimension.MultiplyXY();
}

template <class T> int CoordMap<T>::GetInsertedBoxes() const
{ 
	return this->pInsertedBoxes; 
};



template <class T> void CoordMap<T>::OptimizeIteratorConstraints()
{
	if( this->IsOptimized() )
		return;

	this->pIsOptimized = true;

	int highestX = 0;
	int highestY = 0;
	int lowestX = 99999;
	int lowestY = 99999;

	CoordMapIterator<T> EndIterator = this->End();

	for( CoordMapIterator<T> it = this->Begin(); it != EndIterator; ++it )
	{
		Coord current = it.Coord();
		if( highestX < current.x )
			highestX = current.x;
		if( highestY < current.y )
			highestY = current.y;

		if(lowestX > current.x )
			lowestX = current.x;
		if( lowestY > current.y )
			lowestY = current.y;
	}
	this->pTopLeftZeroPoint = Coord(lowestX,lowestY);
	this->pBottomRightMaxPoint = Coord(highestX,highestY+1);
	this->pOptimizedDimension = (this->GetMaxCoordValues() - this->GetMinCoordValues()).xadd(1);
}

template <class T> void CoordMap<T>::SetBoxFilled( const Coord& Coord )
{
	if( IsOptimized() || this->CoordIsFilled( Coord ) )
		return;

	pInsertedBoxes++;
	pRows[Coord.y].SetBoxFilled( Coord.x );
}

template <class T> void CoordMap<T>::InsertBox( std::shared_ptr<T> item, const Coord& Coord )
{
	if( IsOptimized() || this->CoordIsFilled( Coord ) )
		return;

	if(Coord.IsPositive() == false )
		throw 17;

	if(Coord.x >= this->GetMatrixDimension().x || Coord.y >= this->GetMatrixDimension().y )
		throw 18;

	pInsertedBoxes++;
	pRows[Coord.y].AddItem( item, Coord.x );
};

template <class T> bool CoordMap<T>::CoordIsFilled( const Coord& Coord) const
{
	if( geometry::pointWithinRectangle_inclusive( Coord, this->pTopLeftZeroPoint, this->pBottomRightMaxPoint) )
		return pRows[Coord.y].ColumnIsFilled(Coord.x);
	return false;
}

template <class T> bool CoordMap<T>::IndexIsFilled( int i ) const
{
	return CoordIsFilled(this->CoordFromIndex(i));
}


template <class T> CoordMap<T> & CoordMap<T>::operator=( const CoordMap<T>& other )
{
	this->pDimension = other.pDimension;

	delete[]pRows;
	pRows = new CoordMapRow<T>[pDimension];
	std::copy(other.pRows, other.pRows + pDimension, pRows);

	//this->pRows = other.pRows;
	this->pInsertedBoxes = other.pInsertedBoxes;
	this->pTopLeftZeroPoint = other.pTopLeftZeroPoint;
	this->pBottomRightMaxPoint = other.pBottomRightMaxPoint;
	this->pIsOptimized = other.pIsOptimized;
	this->pOptimizedDimension = other.pOptimizedDimension;
	return *this;
}

template <class T> CoordMap<T> & CoordMap<T>::operator=( CoordMap<T>&& other )
{
	this->pDimension = std::move(other.pDimension);

	delete[] pRows;
	this->pRows = other.pRows;
	other.pRows = nullptr;

	//this->pRows = std::move(other.pRows);
	this->pInsertedBoxes = std::move(other.pInsertedBoxes);
	this->pTopLeftZeroPoint = std::move(other.pTopLeftZeroPoint);
	this->pBottomRightMaxPoint = std::move(other.pBottomRightMaxPoint);
	this->pIsOptimized = std::move(other.pIsOptimized);
	this->pOptimizedDimension = std::move(other.pOptimizedDimension);
	return *this;
}

template <class T> Coord CoordMap<T>::GetMinCoordValues() const
{
	return this->pTopLeftZeroPoint;
}
template <class T> Coord CoordMap<T>::GetMaxCoordValues() const
{
	return this->pBottomRightMaxPoint;
}

template <class T> bool CoordMap<T>::IsOptimized() const
{
	return this->pIsOptimized;
}

template <class T> Coord CoordMap<T>::GetOptimiziedDimension() const
{
	return this->pOptimizedDimension; //
}

template <class T> Coord CoordMap<T>::GetMatrixDimension() const
{
	return Coord(this->pDimension);
}
template <class T> Coord CoordMap<T>::GetTopLeftZeroPoint() const
{
	return this->pTopLeftZeroPoint;
}

template <class T> CoordMap<bool> CoordMap<T>::ToBinaryMap() const
{
	CoordMap<bool> result = CoordMap<bool>(this->pDimension); 
	for (CoordMapIterator<T> it = this->Begin(); it != this->End(); ++it)
	{
		result.InsertBox(std::shared_ptr<bool>( new bool(true)), it.Coord());
	}
	this->CopyParameters(result);
	return result;
}

template <class T> void CoordMap<T>::CopyParameters(CoordMap<bool>& copyTo) const
{
	copyTo.SetTopLeftZeroPoint(this->pTopLeftZeroPoint);
	copyTo.SetBottomRightMaxPoint(this->pBottomRightMaxPoint);
	copyTo.SetIsOptimized(this->pIsOptimized);
	copyTo.SetOptimizedDimension(this->pOptimizedDimension);
}

template <class T> void CoordMap<T>::SetBottomRightMaxPoint(const Coord& pos)
{
	this->pBottomRightMaxPoint = pos;
}
template <class T> void CoordMap<T>::SetTopLeftZeroPoint(const Coord& pos)
{
	this->pTopLeftZeroPoint = pos;
}
template <class T> void CoordMap<T>::SetOptimizedDimension(const Coord& pos)
{
	this->pOptimizedDimension = pos;
}

template <class T> std::vector<Coord> CoordMap<T>::GetRandomFilledCoords(int amount)
{
	auto randomizedList = this->FilledCoordinateList();
	std::random_shuffle(randomizedList.begin(), randomizedList.end());
	auto result = std::vector<Coord>();
	for (int i = 0; i < randomizedList.size() && i < amount; i++)
	{
		result.push_back(randomizedList[i]);
	}
	return result;
}

template <class T> EnumAdjacency CoordMap<T>::Neighbourhood(const Coord& pos)
{
	EnumAdjacency result = EnumAdjacency::ZERO;
	if (CoordIsFilled(pos.xadd(1)))
		result = result | EnumAdjacency::RIGHT;
	if (CoordIsFilled(pos.xadd(1).yadd(1)))
		result = result | EnumAdjacency::DOWNRIGHT;
	if (CoordIsFilled(pos.xadd(1).yadd(-1)))
		result = result | EnumAdjacency::UPRIGHT;
	if (CoordIsFilled(pos.xadd(-1)))
		result = result | EnumAdjacency::LEFT;
	if (CoordIsFilled(pos.xadd(-1).yadd(1)))
		result = result | EnumAdjacency::DOWNLEFT;
	if (CoordIsFilled(pos.xadd(-1).yadd(-1)))
		result = result | EnumAdjacency::UPLEFT;
	if (CoordIsFilled(pos.yadd(1)))
		result = result | EnumAdjacency::DOWN;
	if (CoordIsFilled(pos.yadd(-1)))
		result = result | EnumAdjacency::UP;
	return result;
}


template <class T> std::vector<Coord> CoordMap<T>::FilledCoordinateList()
{
	std::vector<Coord> result = std::vector<Coord>();
	for (CoordMapIterator<T> it = this->Begin(); it != this->End(); ++it)
		result.push_back(it.Coord());
	return result;
}




template <class T> CoordMapIterator<T> CoordMap<T>::End() const
{
	return CoordMapIterator<T>(this,this->IndexSize());
}
template <class T> CoordMapIterator<T> CoordMap<T>::Begin() const
{
	if( this->Size() > 0 )
	{
			CoordMapIterator<T> result = CoordMapIterator<T>(this,0);
			if( result.item() == nullptr )
				++result;
		return result;
	}
	return CoordMap::End();

}

template <class T> CoordMapIterator<T>::CoordMapIterator( const CoordMap<T>* map, int startIterator)
{
	MapReference = map;
	currentIterator = startIterator;
	if(map->Size()>0 && startIterator < map->IndexSize() )
		currentItem = map->GetItem(startIterator).Item;
}

template <class T> CoordMapIterator<T>& CoordMapIterator<T>::incrementIterator( int value ) // void incrementIterator( int value );
{
	this->currentIterator += value;
	return *this;
}

template <class T> CoordMapIterator<T> & CoordMapIterator<T>::operator++()
{
	currentIterator++;
	int indexSize = this->MapReference->IndexSize();
	auto coord = this->MapReference->CoordFromIndex(currentIterator);
	if (currentIterator == indexSize)
	{
		this->currentItem == nullptr;
		return *this;
	}


	auto filled = (*MapReference).CoordIsFilled(coord);
	if (filled == true)
		this->currentItem = (*MapReference).GetItem(coord).Item;
	else
		this->currentItem = nullptr;

	//if(iteratedItem.Status == enCoordStatus::csEmpty )
	//	this->currentItem = nullptr;
	//else
	//	this->currentItem = iteratedItem.Item;

	while( this->currentItem == nullptr && ++currentIterator < indexSize)
	{
		coord = this->MapReference->CoordFromIndex(currentIterator);
		filled = (*MapReference).CoordIsFilled(coord);
		if (filled)
			this->currentItem = (*MapReference).GetItem(coord).Item;
		//std::shared_ptr<T> ptr = (*MapReference).GetItem(currentIterator).Item;
		//if (ptr.get())
		//	this->currentItem = ptr;
	}

	return *this;
}

template <class T> Coord CoordMapIterator<T>::Coord() const
{
	return MapReference->CoordFromIndex(currentIterator);
}

//template <class T> Coord CoordMapIterator<T>::Coord() const
//{
//	return this->pLastValidCoord;
//}

//CoordMapIterator CoordMapIterator::operator++(int)
//{
//	X tmp(*this); // copy
//	operator++(); // pre-increment
//	return tmp;   // return old value
//}