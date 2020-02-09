#pragma once
#include"GriddedBox.h"


struct EdgeBox
{
	EdgeBox(std::shared_ptr<GriddedBox> box )
	{
		this->gbox = box;
		//this->placement = placement;
	}

	EdgeBox( const EdgeBox& );
	EdgeBox& operator=(const EdgeBox& );

	std::shared_ptr<GriddedBox> gbox;
	//int placement;
};

class EdgeBoxList
{

public:

	EdgeBoxList();
	//EdgeBoxList( int resolution );

	EdgeBoxList( const EdgeBoxList & );

	friend void swap(EdgeBoxList& first, EdgeBoxList& second) //undo
    {
        using std::swap; 

		//swap(first.pResolution, second.pResolution);
		//swap(first.segmentedList, second.segmentedList); 
        swap(first.flatList, second.flatList);
    }

	//~EdgeBoxList();

	EdgeBoxList& operator=(EdgeBoxList other);
	int size() { return flatList.size(); };
	//int angleToSegment( double rad );

	void addBox( std::shared_ptr<GriddedBox> box );
	void mergeBox( const EdgeBoxList &, Coord center  );
	void shiftPosition( Coord ); //shifts position of all boxes in list
	//void refreshList();


	bool hasEdgeAtPosition( const Coord& pos );
	//std::list<std::shared_ptr<EdgeBox>> getSegment( int i ) const { return segmentedList[i]; };

	//int pResolution;
	
	//std::vector<std::list<std::shared_ptr<EdgeBox>>> segmentedList;

	std::map<Coord,std::shared_ptr<EdgeBox>>::const_iterator const begin() const { return flatList.begin(); }
    std::map<Coord,std::shared_ptr<EdgeBox>>::const_iterator const end() const { return flatList.end(); }

	std::map<Coord,std::shared_ptr<EdgeBox>> flatList;

	void ClearList();
	
private:

	void addBox( std::shared_ptr<EdgeBox> box ) { flatList[box->gbox->Position()] = box; };
};
