#include"LinkObject.h"
#include"GameManager.h"
#include"LinkSelectionHandlers.h"


LinkObject::LinkObject()
{
	this->text = "";
	this->type = "";
	this->size = Coord(0);
	this->relativepos = Coord(0);

}


enLinkType LinkObject::GetLinkType()
{
	return LinkObject::GetLinkType(this->type);
}

enLinkType LinkObject::GetLinkType(string type)
{
	if (type == "dock" || type == "DOCK" || type == "Dock" || TypeIsShop(type))
		return enLinkType::LINKDOCKBUTTON;
	else if (type == "target" || type == "TARGET" || type == "Target")
		return enLinkType::LINKTARGETSELECTED;
	else if (TypeIsLocation(type))
		return enLinkType::LINKHUDBUTTON;
	else if (TypeIsSimpleTablet(type))
		return enLinkType::LINKHUDBUTTON;
	else if (TypeIsShipSelect(type))
		return enLinkType::LINKHUDBUTTON;
	else if (TypeIsDialogue(type))
		return enLinkType::LINKDIALOG;
	else
		return enLinkType::LINKSTANDARD;
}

LinkObject LinkObject::ReadLink(string content)
{
	string xPos = "";
	string yPos = "";
	string linktext = "";
	string xSize = "";
	string ySize = "";
	string type = "";


	int i = 0;
	for (; i < content.size(); i++)
	{
		if (content[i] != ',')
		{
			xPos += content[i];
		}
		else
			break;
	}

	i++;
	for (; i < content.size(); i++)
	{
		if (content[i] != ':')
		{
			yPos += content[i];
		}
		else
			break;
	}

	i++;
	for (; i < content.size(); i++)
	{
		if (content[i] != ':')
		{
			linktext += content[i];
		}
		else
			break;
	}

	i++;
	for (; i < content.size(); i++)
	{
		if (content[i] != ',')
		{
			xSize += content[i];
		}
		else
			break;
	}

	i++;
	for (; i < content.size(); i++)
	{
		if (content[i] != ':')
		{
			ySize += content[i];
		}
		else
			break;
	}

	i++;
	for (; i < content.size(); i++)
	{
		if (content[i] != ',')
		{
			type += content[i];
		}
		else
			break;
	}

	Coord pos = Coord(std::stoi(xPos), std::stoi(yPos));
	Coord size = Coord(std::stoi(xSize), std::stoi(ySize));

	LinkObject newLink = LinkObject();
	newLink.relativepos = pos;
	newLink.size = size;
	newLink.text = linktext;
	newLink.type = type;
	return newLink;
}

