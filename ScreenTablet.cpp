#include"ScreenTablet.h"
#include"SingleButtonTablet.h"


ScreenTablet::ScreenTablet()
	:ScreenHandler()
{



}

ScreenTablet::ScreenTablet(int screen_width, int screen_height, Coord pos, bool stretch, const imagehandling::IMGToSurface* surf, ScreenTabletType type)
	:ScreenHandler()
{
	std::shared_ptr<SingleButtonTablet> scr = std::shared_ptr<SingleButtonTablet>(new SingleButtonTablet(screen_width, screen_height, pos, surf, "close"));
	this->AddScreen(scr, "screen");

	pTabletType = type;
}

void ScreenTablet::Initialize()
{

}

bool ScreenTablet::HasErrorScreens()
{
	return this->GetScreenByName(ScreenTablet::ErrorScreenName(), true) != nullptr;
}


//
//void CloseTablet(SDL_Event& e, Widget* button)
//{
//	auto state = button->GetState();
//	switch (button->GetState())
//	{
//		case WS_HOVER:
//
//			break;
//		case WS_ACTIVE:
//			GameManager
//			break;
//		case WS_ON:
//			break;
//	}
//}