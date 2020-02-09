#include"BoxList.h"	

BoxList::BoxList( const BoxList& other)
{
	this->lowestPosition = other.lowestPosition;

	for( std::list<Coord>::const_iterator it = other.getList().begin(); it != other.getList().end(); ++it )
	{
		this->list.push_back(*it);
	}

	this->pHeight = other.pHeight;
	this->pWidth = other.pWidth;
}

BoxList::BoxList()
{
	this->list = std::list<Coord>();
	this->lowestPosition = Coord(0,0);
	this->pHeight = 0;
	this->pWidth = 0;
	//initialize();

}

BorderData BoxList::GetBorderFromCoord( const Coord& pos , bool edgePositive ) const
{
	BorderData result = BorderData();
	result.right = (pos.x == this->lowestPosition.x) == edgePositive;
	result.up = (pos.y == this->lowestPosition.y) == edgePositive;
	result.left = (pos.x ==  getHighestPosition().x) == edgePositive;
	result.down = (pos.y == getHighestPosition().y) == edgePositive;
	return result;
}

//std::list<CoordWithColor> BoxList::getList( ColorRGB col ) const //too slow
//{
//	//std::list<CoordWithColor> result = std::list<CoordWithColor>();
//	//for( std::list<Coord>::const_iterator it = this->begin(); it != this->end(); ++it )
//	//{
//	//	result.push_back(CoordWithColor((*it).x, (*it).y,col));
//	//}
//	//return result;
//}

BoxList::BoxList( int width, int height )
{
	this->list = std::list<Coord>();
	this->generateRectangle(width,height);
}

	void BoxList::generateSquare( int size )
	{
		generateRectangle( size, size );
		initialize();
	}

	void BoxList::generateRectangle( int w, int h )
	{
		this->list.clear();

		for( int i = 0; i<w; i++ )
		{
			for( int j = 0; j<h; j++ )
			{
				this->list.push_back( Coord( i,j ) ); 
			}
		}
		initialize();
	}

void BoxList::copyList( const std::list<Coord>& src, const Coord& relativePosition )
{
	this->list.clear();
	for( std::list<Coord>::const_iterator it = src.begin(); it != src.end(); ++it )
	{
		this->list.push_back(Coord((*it).x-relativePosition.x, (*it).y-relativePosition.y));
	}
	initialize();
}

void BoxList::copyList( const std::vector<Coord>& src, const Coord& relativePosition )
{
	this->list.clear();
	for( std::vector<Coord>::const_iterator it = src.begin(); it != src.end(); ++it )
	{
		this->list.push_back(Coord((*it).x-relativePosition.x, (*it).y-relativePosition.y));
	}
	initialize();
}

void BoxList::initialize()
{
	this->list.sort(compare_coords); //takes 8 sec for frigate class
	if( this->list.size() > 0 )
	{
		this->lowestPosition = *this->list.begin();
		Coord highestPosition = this->list.back();
		this->pWidth = highestPosition.x - this->lowestPosition.x + 1;
		this->pHeight = highestPosition.y - this->lowestPosition.y + 1;
	}

	//int highestX = -100000; //arbitrary large number
	//int highestY = -100000;
	//int lowestX = 100000;
	//int lowestY = 100000;

	//for( std::list<Coord>::const_iterator it = this->list.begin(); it != this->list.end(); ++it )
	//{
	//	if((*it).x > highestX )
	//		highestX = (*it).x;
	//	if((*it).y > highestY )
	//		highestY = (*it).y;
	//	if( (*it).x < lowestX )
	//		lowestX = (*it).x;
	//	if( (*it).y < lowestY )
	//		lowestY = (*it).y;

	//}
	
	//this->lowestPosition = Coord( lowestX, lowestY );
	//this->pWidth = highestX - this->lowestPosition.x + 1;
	//this->pHeight = highestY - this->lowestPosition.y + 1;

}

void BoxList::Translate( const Coord& offset)
{
	for( std::list<Coord>::iterator it = this->list.begin(); it != this->list.end(); ++it )
	{
		(*it) = (*it) + offset;
	}
	this->lowestPosition = *this->list.begin();
}

Coord BoxList::GetCenter() const 
{
	//int xLength = this->pWidth - this->lowestPosition.x;
	//int yLength = this->pHeight - this->lowestPosition.y;
	Coord result = Coord(this->pWidth/2, this->pHeight/2 ); //correct answer??;
	return result;
}

spriteSize BoxList::GetSize() const
{
	int size = std::fmax( this->pWidth, this->pHeight );

	int maxSize = 1024;

	while( size < maxSize )
		maxSize /= 2;

	maxSize *= 2;
	return spriteSize::GetMaxSize();

}

bool BoxList::isEdge( const Coord& pos) const
{
	if( pos.x == this->lowestPosition.x )
		return true;
	if(pos.y == this->lowestPosition.y )
		return true;
	if( pos.x ==  getHighestPosition().x )
		return true;
	if( pos.y == getHighestPosition().y )
		return true;

	return false;
}

bool BoxList::isCorner( const Coord& pos) const
{
	if( pos == this->lowestPosition )
		return true;
	if( pos == this->getHighestPosition() )
		return true;

	if( pos.x == this->lowestPosition.x && pos.y == this->getHighestPosition().y )
		return true;
	if( pos.y == lowestPosition.y && pos.x == this->getHighestPosition().x )
		return true;

	return false;
}

void BoxList::Clear()
{
	list.clear();
}


