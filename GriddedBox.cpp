#include"GriddedBox.h"

Neighbour::Neighbour()
{
	present = nullptr;
}

Neighbour::Neighbour( const Neighbour& other)
{
	this->oppositeAdj = other.oppositeAdj;
	this->pos = other.pos;
	this->present = other.present;
	this->properAdj = other.properAdj;
}

Neighbour::Neighbour( const Neighbour&& other)
{
	this->oppositeAdj = std::move(other.oppositeAdj);
	this->pos = std::move(other.pos);
	this->present = std::move(other.present);
	this->properAdj = std::move(other.properAdj);
}

Neighbour Neighbour::operator=(const Neighbour& other)
{
	this->oppositeAdj = other.oppositeAdj;
	this->pos = other.pos;
	this->present = other.present;
	this->properAdj = other.properAdj;
	return *this;
}
Neighbour Neighbour::operator=(Neighbour&& other)
{
	this->oppositeAdj = std::move(other.oppositeAdj);
	this->pos = std::move(other.pos);
	this->present = std::move(other.present);
	this->properAdj = std::move(other.properAdj);
	return *this;
}

GriddedBox::GriddedBox()
{
	placed = false; 
	box = PixelBox();
	pCoord = Coord();
	pEdgeBox = false;
	hull = game::Hull();
	//initHull = game::Hull();
}

GriddedBox::GriddedBox( const GriddedBox & other )
{
	//checked ok
	placed = other.placed;
	box = other.box;
	pCoord = other.pCoord;
	pNeighbourIterator = other.pNeighbourIterator;
	hull = other.hull;
	//initHull = other.initHull;
	pEdgeBox = other.pEdgeBox;
}

GriddedBox::GriddedBox( const GriddedBox && other )
{
	//checked ok
	placed = std::move(other.placed);
	box = std::move(other.box);
	pCoord = std::move(other.pCoord);
	pNeighbourIterator = std::move(other.pNeighbourIterator);
	hull = std::move(other.hull);
	//initHull = std::move(other.initHull);
	pEdgeBox = std::move(other.pEdgeBox);
}

GriddedBox::GriddedBox(PixelBox setBox, Coord setCoord, bool setPlaced )
{
	//collision = isCollision;
	placed = setPlaced; 
	box = setBox;
	pCoord = setCoord;
	pEdgeBox = false;
	hull = game::Hull();
	//initHull = game::Hull();

	Neighbour up;
	Neighbour down;
	Neighbour left;
	Neighbour right;

	Neighbour upleft;
	Neighbour upright;
	Neighbour downleft;
	Neighbour downright;

	up.properAdj = EnumAdjacency::UP;
	down.properAdj = EnumAdjacency::DOWN;
	left.properAdj = EnumAdjacency::LEFT;
	right.properAdj = EnumAdjacency::RIGHT;

	upleft.properAdj = EnumAdjacency::UPLEFT;
	upright.properAdj = EnumAdjacency::UPRIGHT;
	downleft.properAdj = EnumAdjacency::DOWNLEFT;
	downright.properAdj = EnumAdjacency::DOWNRIGHT;

	up.oppositeAdj = EnumAdjacency::DOWN;
	down.oppositeAdj = EnumAdjacency::UP;
	left.oppositeAdj = EnumAdjacency::RIGHT;
	right.oppositeAdj = EnumAdjacency::LEFT;

	upleft.oppositeAdj = EnumAdjacency::DOWNRIGHT;
	upright.oppositeAdj = EnumAdjacency::DOWNLEFT;
	downleft.oppositeAdj = EnumAdjacency::UPRIGHT;
	downright.oppositeAdj = EnumAdjacency::UPLEFT;

	up.present = &box.up;//box->neighbour( PixelBox::EnumAdjacency::UP );
	down.present = &box.down;//box->neighbour( PixelBox::EnumAdjacency::DOWN );
	left.present = &box.left;//box->neighbour( PixelBox::EnumAdjacency::LEFT );
	right.present = &box.right;//box->neighbour( PixelBox::EnumAdjacency::RIGHT );
	upleft.present = &box.upleft;//box->neighbour( PixelBox::EnumAdjacency::UPLEFT );
	upright.present = &box.upright;//box->neighbour( PixelBox::EnumAdjacency::UPRIGHT );
	downleft.present = &box.downleft;//box->neighbour( PixelBox::EnumAdjacency::DOWNLEFT );
	downright.present = &(box.downright);//box->neighbour( PixelBox::EnumAdjacency::DOWNRIGHT );

	up.pos = Coord(setCoord.x,setCoord.y-1);
	down.pos = Coord(setCoord.x,setCoord.y+1);
	left.pos = Coord(setCoord.x+1,setCoord.y);
	right.pos = Coord(setCoord.x-1,setCoord.y);

	upleft.pos = Coord(setCoord.x+1,setCoord.y-1);
	upright.pos = Coord(setCoord.x-1,setCoord.y-1);
	downleft.pos = Coord(setCoord.x+1,setCoord.y+1);
	downright.pos = Coord(setCoord.x-1,setCoord.y+1);

	pNeighbourIterator.reserve(8);
	pNeighbourIterator.push_back(up);
	pNeighbourIterator.push_back(upleft);
	pNeighbourIterator.push_back(left);
	pNeighbourIterator.push_back(downleft);
	pNeighbourIterator.push_back(down);
	pNeighbourIterator.push_back(downright);
	pNeighbourIterator.push_back(right);
	pNeighbourIterator.push_back(upright);

}

double GriddedBox::GetPercentageVitalHitPoints()
{
	double result = this->hull.getHitPoints(game::enHitPointType::HP_MATERIAL).PercentageLeft();
	if(result == 0 )
		return 0;
	return result;
}

void GriddedBox::SetMaxHitPointValue( int value )
{
	this->hull.setHitpoints(game::enHitPointType::HP_MATERIAL,value, true);
	//this->initHull.setHitpoints(value);
}

Neighbour GriddedBox::getIteratorNeighbour( int i ) const
{
	return this->pNeighbourIterator[i];
}


void GriddedBox::setPlaced(bool set)
{
	if(set)
	{
		this->placed = true;
	}
	else
	{
		this->placed = false;
	}
}

GriddedBox& GriddedBox::operator=(const GriddedBox& other)
{
	placed = other.placed;
	box = other.box;
	pCoord = other.pCoord;
	pNeighbourIterator = other.pNeighbourIterator;
	hull = other.hull;
	//initHull = other.initHull;
	pEdgeBox = other.pEdgeBox;
	return *this;
}

GriddedBox& GriddedBox::operator=(const GriddedBox&& other)
{
	placed = std::move(other.placed);
	box = std::move(other.box);
	pCoord = std::move(other.pCoord);
	pNeighbourIterator = std::move(other.pNeighbourIterator);
	hull = std::move(other.hull);
	//initHull = std::move(other.initHull);
	pEdgeBox = std::move(other.pEdgeBox);
	return *this;
}
