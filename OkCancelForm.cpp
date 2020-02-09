#include"OkCancelForm.h"
#include"SurfaceButton.h"
#include"ScreenButton.h"
#include"BackgroundCollection.h"

using namespace std::placeholders;

OkCancelForm::OkCancelForm() : Screen()
{
}

OkCancelForm::OkCancelForm( Coord dimensions, Coord pos )
{
	screenPosition = pos;
	pScreenWidth = dimensions.x;
	pScreenHeight = dimensions.y;
	
	//Setup(dimensions,pos);
}

void OkCancelForm::Setup( Coord dimensions, Coord pos )
{
	this->SetBackground(ImportImage_OkCancelBorder, 255);
	this->AddBackgroundLayer(ImportImage_OKCancelScreen, 255);
	this->CenterScreenPositionBasedOnBackgroundImage();
	

	int rect_h = STANDARD_BUTTON_HEIGHT-16;
	int rect_w = 75;

	auto btnOK = std::shared_ptr<Widget>(new ScreenButton(Coord(this->GetPosition().x + dimensions.x - rect_w * 2 - 60, this->GetPosition().y + dimensions.y - rect_h - 32), rect_w, rect_h, BtnText("OK")));
	this->Control()->AddWidget(btnOK, std::bind(&OkCancelForm::ok_click, this, _1));

	auto btnCancel = std::shared_ptr<Widget>(new ScreenButton(Coord(this->GetPosition().x + dimensions.x - rect_w - 60, this->GetPosition().y + dimensions.y - rect_h - 32), rect_w, rect_h, BtnText("Cancel")));
	this->Control()->AddWidget(btnCancel, std::bind(&OkCancelForm::close_click, this, _1));

}

void OkCancelForm::close_click(Widget*)
{

}
void OkCancelForm::ok_click( Widget*)
{

}
