#include"Region.h"

Region::Region()
{
	pShape = nullptr;
	Initialize();
}

Region::Region(std::shared_ptr<Shapes::Shape> shape)
{
	pShape = shape;
	Initialize();
}

Region::Region(int x, int y, int w, int h)
{
	pShape = std::shared_ptr<Shapes::Shape>(new Shapes::Rectangle(Coord(x, y), Coord(w, h)));
	Initialize();
}
Region::Region(const Coord& pos, const Coord& size)
{
	pShape = std::shared_ptr<Shapes::Shape>(new Shapes::Rectangle(pos, size));
	Initialize();
}

void Region::Initialize()
{
	pName = "noname";
	pIsActive = true;
}

CollRegion::CollRegion()
{

}


Region CollRegion::GetRegion(std::string name)
{
	for (int i = 0; i < this->size(); i++)
		if (((*this)[i].GetName() == name))
			return (*this)[i];
	return Region();
}

Region CollRegion::GetRegion(const Coord pos)
{
	for (int i = 0; i < this->size(); i++)
		if (((*this)[i].IsPointInRegion(pos)))
			return (*this)[i];
	return Region();
}

bool CollRegion::IsRegionInPoint(const Coord pos)
{
	return GetRegion(pos).IsEmpty();
}