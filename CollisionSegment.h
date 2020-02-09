#pragma once
#include"Graphics.h"
#include"InGameGraphicalObjectLink.h"

class CollisionSegment
{

public:

	CollisionSegment();
	CollisionSegment(Coord pos, Coord size);

	bool PointInSegment(const Coord&);

	bool HasLink(std::shared_ptr<InGameGraphicalObjectLink> link);
	void AddLink(std::shared_ptr<InGameGraphicalObjectLink> link, const Coord& pos);
	bool RemoveLinkID(int id);
	//std::unique_ptr<InGameGraphicalObjectLink>* GetHitBox(const Coord& pos);

	std::vector<std::shared_ptr<InGameGraphicalObjectLink>>::iterator HB_begin() { return pHBCollection.begin(); };
	std::vector<std::shared_ptr<InGameGraphicalObjectLink>>::iterator HB_end() { return pHBCollection.end(); };

private:

	bool SegmentIsInfinite;
	std::vector<std::shared_ptr<InGameGraphicalObjectLink>> pHBCollection;
	Coord pos;
	Coord size;

};