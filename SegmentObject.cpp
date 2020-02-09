#include"SegmentObject.h"
#include<string>

SegmentObject::SegmentObject()
{
	this->position = Coord(0);
	this->size = Coord(0);
}

SegmentObject::SegmentObject(const Coord pos, const Coord size)
{
	this->position = pos;
	this->size = size;
}

SegmentObject SegmentObject::ReadSegment(std::string content)
{
	std::string xRelativePos = "";
	std::string yRelativePos = "";

	int i = 0;
	for (; i < content.size(); i++)
	{
		if (content[i] != ',')
		{
			xRelativePos += content[i];
		}
		else
			break;
	}

	i++;
	for (; i < content.size(); i++)
	{
		if (content[i] != ':')
		{
			yRelativePos += content[i];
		}
		else
			break;
	}
	Coord position = Coord(std::stoi(xRelativePos), std::stoi(yRelativePos));

	std::string xRelativeSize = "";
	std::string yRelativeSize = "";

	i++;
	for (; i < content.size(); i++)
	{
		if (content[i] != ',')
		{
			xRelativeSize += content[i];
		}
		else
			break;
	}

	i++;
	for (; i < content.size(); i++)
	{
		if (content[i] != ':')
		{
			yRelativeSize += content[i];
		}
		else
			break;
	}
	Coord size = Coord(std::stoi(xRelativeSize), std::stoi(yRelativeSize));
	return SegmentObject(position, size);
}