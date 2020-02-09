#pragma once
#include"BackNextTablet.h"
#include"Graphics.h"
#include<string>

class ShipSelectTablet : public BackNextTablet
{

public:

	ShipSelectTablet(int, int, const Coord pos, const imagehandling::IMGToSurface*, string vesselName, bool next, bool back);

	void HandleActiveWidget(SDL_Event& e, Widget* widget) override;

protected:

	void Initialize(bool next, bool back);

private:

	string pVesselName;

};