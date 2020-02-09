#include"GBDisplayBtn.h"


GBDisplayBtn::GBDisplayBtn(const char* text, TextSize size, Coord position, int h, int w, std::shared_ptr<GridObject> gb)
{
	this->pBtn = SelectLink(text, size, position, h,w );
	this->pGridObjectDisplay = gb;
}