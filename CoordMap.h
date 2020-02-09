#pragma once
#include"Coord.h"

enum enCoordStatus
{
	csEmpty = 1,
	csOccupy = 2
};

template <typename T>
struct CoordMapItem
{

	CoordMapItem();
	CoordMapItem( const CoordMapItem<T>& );
	CoordMapItem( const CoordMapItem<T>&& );

	CoordMapItem<T> & operator=( const CoordMapItem<T>& other );
	CoordMapItem<T> & operator=( const CoordMapItem<T>&& other );

	void InsertBox( std::shared_ptr<T> );

	enCoordStatus Status;
	std::shared_ptr<T> Item;
};

template <class T> class CoordMapRow
{

public:

	CoordMapRow();
	CoordMapRow( int size );
	CoordMapRow( const CoordMapRow<T>& );
	CoordMapRow( CoordMapRow<T>&& );

	CoordMapRow<T> & operator=( const CoordMapRow<T>& other );
	CoordMapRow<T> & operator=( CoordMapRow<T>&& other );

	void AddItem( std::shared_ptr<T>, int column );
	void SetBoxFilled( int column ); 
	void EraseBox( int index );
	void EraseAll();
	void ResetColumns();

	int Size() { return pSize; }

	std::shared_ptr<T> & operator[]( int index); 
	bool ColumnIsFilled( int column ) const;
	CoordMapItem<T>& GetColumnItem( int column );

	~CoordMapRow();

private:

	int pSize;
	CoordMapItem<T>* pColumns;
};

template <class T> class CoordMapIterator;

template <class T> class CoordMap 
{

public:

	CoordMap();
	CoordMap( int size );
	CoordMap( const CoordMap<T>& );
	CoordMap( CoordMap<T>&& );

	CoordMap<T> & operator=( const CoordMap<T>& other );
	CoordMap<T> & operator=( CoordMap<T>&& other );

	void CopyParameters( CoordMap<bool>& copyTo) const;	//mmust be a more general way.. but I don' know which
	
	void SetBoxFilled( const Coord&  );
	void InsertBox( std::shared_ptr<T> , const Coord& );
	void OverrideBox( std::shared_ptr<T> , const Coord& );

	bool AllBoxesFilled();
	double FillPercent();

	std::shared_ptr<T> & operator[](const Coord& );
	std::shared_ptr<T> & operator[](int );
	
	Coord CoordFromIndex( int i ) const;

	int Size() const { return pInsertedBoxes; };
	int IndexSize() const;
	int GetInsertedBoxes() const;// { return this->pInsertedBoxes; };
	void Erase( const Coord& );
	void Clear();
	void ResetSize();
	bool CoordIsFilled( const Coord& ) const;
	bool IndexIsFilled( int ) const;
	CoordMapIterator<T> End() const;
	CoordMapIterator<T> Begin() const;
	
	const CoordMapItem<T>& GetItem( const Coord& ) const;
	const CoordMapItem<T>& GetItem( int ) const;

	Coord GetMinCoordValues() const;
	Coord GetMaxCoordValues() const;

	void OptimizeIteratorConstraints();
	bool IsOptimized() const;
	Coord GetOptimiziedDimension() const;
	Coord GetMatrixDimension() const;
	Coord GetTopLeftZeroPoint() const;

	void SetIsOptimized(bool set) { this->pIsOptimized = set; };
	void SetBottomRightMaxPoint(const Coord&);
	void SetTopLeftZeroPoint(const Coord&);
	void SetOptimizedDimension(const Coord&);

	EnumAdjacency Neighbourhood(const Coord& pos);
	std::vector<Coord> FilledCoordinateList();

	CoordMap<bool> ToBinaryMap() const;
	std::vector<Coord> GetRandomFilledCoords(int amount);

private:

	int pInsertedBoxes;
	int pDimension;
	Coord pOptimizedDimension;

	Coord pTopLeftZeroPoint;
	Coord pBottomRightMaxPoint;
	
	CoordMapRow<T>* pRows; // x-axis
	bool pIsOptimized;

};

template <class T> class CoordMapIterator
{
public:
	CoordMapIterator( const CoordMap<T>*, int startIterator);
	
	CoordMapIterator<T>& operator++();
	
	CoordMapIterator<T>& incrementIterator( int value );
    bool operator!=(const CoordMapIterator<T>& rhs) const { return (this->currentIterator != rhs.currentIterator); };

	Coord Coord() const;
	std::shared_ptr<T>& item() { return currentItem; };

	int currentIterator; // should be private

private:

	
	//Coord pLastValidCoord;

	//Coord CoordFromIndex() const;
	const CoordMap<T>* MapReference;
	std::shared_ptr<T> currentItem;
};
