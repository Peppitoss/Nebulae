#include"TextDialogue.h"


TextDialogue::TextDialogue()
{
	Initialize();
}

void TextDialogue::Run(int ms)
{


}

Coord TextDialogue::GetTextDimension(int txtSize)
{
	return Coord::GetRectSize(sdlWrapper.GetTextSize(txtSize, standardLinkFont, this->GetText().GetCurrentValue(), TTF_STYLE_BOLD));
}

std::shared_ptr<Link> TextDialogue::GenerateLinkFromText(int txtSize, ColorRGB& backgrounColor, ColorRGB& textColor)
{
	std::shared_ptr<Link> result = std::shared_ptr<Link>(new Link(this->GetText(), txtSize, Coord(0, 0), 12, 32, textColor, backgrounColor ));
	result->Refresh();
	return result;
}

void TextDialogue::Initialize()
{
	this->SetState(TextDialogeState::txDisable);
}