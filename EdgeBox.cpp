#include"EdgeBox.h"


EdgeBox::EdgeBox( const EdgeBox& other)
{
	this->gbox = std::shared_ptr<GriddedBox>( new GriddedBox(*other.gbox) );
	//this->placement = other.placement;
}

EdgeBox& EdgeBox::operator=(const EdgeBox& other)
{
	this->gbox = std::shared_ptr<GriddedBox>( new GriddedBox(*other.gbox) );
	//this->placement = other.placement;
	return *this;
}

EdgeBoxList::EdgeBoxList()
{
	//pResolution = 0;

	//this->segmentedList = std::vector<std::list<std::shared_ptr<EdgeBox>>>();
	flatList = std::map<Coord,std::shared_ptr<EdgeBox>>();
}

//EdgeBoxList::EdgeBoxList( int resolution )
//{
//	pResolution = resolution;
//
//	//for( int i = 0; i<this->pResolution; i++ )
//	//	this->segmentedList.push_back( std::list<std::shared_ptr<EdgeBox>>() );
//
//	flatList = std::map<Coord,std::shared_ptr<EdgeBox>>();
//}

EdgeBoxList::EdgeBoxList( const EdgeBoxList & other )
{
	//pResolution = other.pResolution;
	//segmentedList.clear();
	flatList.clear();

	//for( int i = 0; i<this->pResolution; i++ )
	//	this->segmentedList.push_back( other.getSegment(i) );

	flatList = other.flatList;
}

//EdgeBoxList::~EdgeBoxList()
//{
//	for( int i = 0; i<segmentedList.size(); i++ )
//	{
//		segmentedList[i].clear();
//	}
//
//	segmentedList.clear();
//	flatList.clear();
//
//}

EdgeBoxList& EdgeBoxList::operator=(EdgeBoxList other)
{   
	swap(*this, other);
	return *this;
}

void EdgeBoxList::addBox( std::shared_ptr<GriddedBox> box)
{
	//double rad = geometry::angleBetweenTwoPoints( center, box->Coord );
	//int segment = angleToSegment(rad);

	std::shared_ptr<EdgeBox> addition = std::shared_ptr<EdgeBox>(new EdgeBox( box ));

	//segmentedList[segment].push_back( addition ); 
	addBox( addition );
}

bool EdgeBoxList::hasEdgeAtPosition( const Coord& pos ) //slow! bottleneck here!
{
	auto it = this->flatList.find(pos);
	if(it != this->flatList.end())
	{
		//element found
		return true; 
	}
	return false;
}

//int EdgeBoxList::angleToSegment( double rad )
//{
//	rad = rad*180/std::_Pi;
//	int step = 360/this->pResolution;
//
//	double result = ( rad /= step );
//	return (int)result;
//}

void EdgeBoxList::shiftPosition( Coord shift ) //not working flatlist doesn't change in the end ,, test this
{
	if( shift == Coord(0,0) )
		return;

	std::map<Coord,std::shared_ptr<EdgeBox>> newMap;

	for( auto it = this->flatList.begin(); it != this->flatList.end(); it++ )
	{
		newMap[ (*it).second->gbox->Position() + shift ] = this->flatList[(*it).second->gbox->Position()];
	}
	this->flatList.clear();
	this->flatList.insert(newMap.begin(), newMap.end());
}

void EdgeBoxList::mergeBox( const EdgeBoxList & newbox, Coord center )
{

	for( auto it = newbox.flatList.begin(); it != newbox.flatList.end(); it++ )
	{
		for( auto itT = this->flatList.begin(); itT != this->flatList.end(); itT++ )
		{
			Coord distance = (*itT).second->gbox->Position() - (*it).second->gbox->Position();

			if( distance.y == 1 && distance.x == 0 )
			{
				(*itT).second->gbox->box.shareNeighbourhood( (*it).second->gbox->box, EnumAdjacency::UP, true );
				(*it).second->gbox->box.shareNeighbourhood( (*itT).second->gbox->box, EnumAdjacency::DOWN, true );
			}
			else if( distance.y == -1 && distance.x == 0 )
			{
				(*itT).second->gbox->box.shareNeighbourhood( (*it).second->gbox->box, EnumAdjacency::DOWN, true );
				(*it).second->gbox->box.shareNeighbourhood( (*itT).second->gbox->box, EnumAdjacency::UP, true );
			}
			else if( distance.y == 0 && distance.x == 1 )
			{
				(*itT).second->gbox->box.shareNeighbourhood( (*it).second->gbox->box, EnumAdjacency::LEFT, true );
				(*it).second->gbox->box.shareNeighbourhood( (*itT).second->gbox->box, EnumAdjacency::RIGHT, true );
			}
			else if( distance.y == 0 && distance.x == -1 )
			{
				(*itT).second->gbox->box.shareNeighbourhood( (*it).second->gbox->box, EnumAdjacency::RIGHT, true );
				(*it).second->gbox->box.shareNeighbourhood( (*itT).second->gbox->box, EnumAdjacency::LEFT, true );
			}
			else if( distance.y == 1 && distance.x == 1 )
			{
				(*itT).second->gbox->box.shareNeighbourhood( (*it).second->gbox->box, EnumAdjacency::UPLEFT, true );
				(*it).second->gbox->box.shareNeighbourhood( (*itT).second->gbox->box, EnumAdjacency::DOWNRIGHT, true );
			}
			else if( distance.y == -1 && distance.x == -1 )
			{
				(*itT).second->gbox->box.shareNeighbourhood( (*it).second->gbox->box, EnumAdjacency::DOWNRIGHT, true );
				(*it).second->gbox->box.shareNeighbourhood( (*itT).second->gbox->box, EnumAdjacency::UPLEFT, true );
			}
			else if( distance.y == 1 && distance.x == -1 )
			{
				(*itT).second->gbox->box.shareNeighbourhood( (*it).second->gbox->box, EnumAdjacency::UPRIGHT, true );
				(*it).second->gbox->box.shareNeighbourhood( (*itT).second->gbox->box, EnumAdjacency::DOWNLEFT, true );
			}
			else if( distance.y == -1 && distance.x == 1 )
			{
				(*itT).second->gbox->box.shareNeighbourhood( (*it).second->gbox->box, EnumAdjacency::DOWNLEFT, true );
				(*it).second->gbox->box.shareNeighbourhood( (*itT).second->gbox->box, EnumAdjacency::UPRIGHT, true );
			}
		}
	}

	for( auto it = newbox.flatList.begin(); it != newbox.flatList.end(); it++ )
	{
		if( (*it).second->gbox->box.isEdge() )
			addBox( (*it).second.get()->gbox );
	}

	for( auto itT = this->flatList.begin(); itT != this->flatList.end();  )
	{

		if( !(*itT).second->gbox->box.isEdge() )
		{
			//int segment = (*itT).second->placement;
			//this->segmentedList[ segment ].remove( (*itT).second ); //linear
			itT = this->flatList.erase( itT ); //const
		}
		else
		{
			++itT;
		}
	}
}

void EdgeBoxList::ClearList()
{
	this->flatList.clear();
}
