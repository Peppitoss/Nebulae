#include"CollisionSegment.h"

CollisionSegment::CollisionSegment(Coord pos, Coord size)
{
	if (pos == Coord(-1) && size == Coord(-1))
		SegmentIsInfinite = true;
	else
		SegmentIsInfinite = false;

	this->pos = pos;
	this->size = size;
}

CollisionSegment::CollisionSegment()
{
	SegmentIsInfinite = true;
	this->pos = Coord(-1);
	this->size = Coord(-1);
}

bool CollisionSegment::PointInSegment(const Coord& pos)
{
	if (SegmentIsInfinite)
		return true;
	return geometry::pointWithinRectangle_inclusive(pos, this->pos, this->size.x, this->size.y);
 }


//std::unique_ptr<InGameGraphicalObjectLink>* CollisionSegment::GetHitBox(const Coord& pos)
//{
//	if (PointInSegment(pos) == true)
//	{
//		for (auto it = this->pHBCollection.begin(); it != this->pHBCollection.end(); ++it)
//		{
//			Coord linkObjectSize = (*it)->pGObject.get()->getSize().GetDimension();
//			if ((*it)->pGObject.get()->CollisionIsEnabled() && (*it)->pGObject.get()->coordinateIscollision(Coord(pos) - *(*it)->relationalZeroHeadingPosition.get() + linkObjectSize / 2))
//				return &(*it);
//		}
//	}
//	return nullptr;
//}

bool CollisionSegment::HasLink(std::shared_ptr<InGameGraphicalObjectLink> link)
{
	for (auto it = this->pHBCollection.begin(); it != pHBCollection.end(); ++it)
	{
		if ((*it)->GetID() == link->GetID())
			return true;
	}
	return false;
}

bool CollisionSegment::RemoveLinkID(int id)
{
	for (auto it = this->pHBCollection.begin(); it != pHBCollection.end(); ++it)
	{
		if ((*it)->GetID() == id)
		{
			this->pHBCollection.erase(it);
			return true;
		}
	}
	return false;
}

void CollisionSegment::AddLink(std::shared_ptr<InGameGraphicalObjectLink> hitbox, const Coord& pos)
{
	if (HasLink(hitbox) == false)
	{
		if (PointInSegment(pos))
			this->pHBCollection.push_back(std::shared_ptr<InGameGraphicalObjectLink>(hitbox));
		else
			throw std::exception("Error hitbox not within segment bounds, it cannot be added!");
	}
}