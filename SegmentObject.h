#pragma once
#include"Graphics.h"

class SegmentObject
{

public:

	SegmentObject();
	SegmentObject(const Coord pos, const Coord size);

	static SegmentObject ReadSegment(std::string content);

	Coord position;
	Coord size;

};