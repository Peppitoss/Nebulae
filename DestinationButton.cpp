#include"DestinationButton.h"
#include"ButtonSurfaceCollection.h"
#include"StyleFactory.h"

DestinationButton::DestinationButton(Coord pos, Text Header, Text location) 
	: SurfaceButton(location.GetID(), pos, StyleFactory::DestinationButtonStyle(ImportImage_DestinationBtn, Header, location))
{
	this->Refresh();
}