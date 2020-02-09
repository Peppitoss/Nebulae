#include"TextBoxScreen.h"
#include"GameManager.h"

TextBoxScreen::TextBoxScreen()
{

}

TextBoxScreen::TextBoxScreen(Text initTextName, std::function<void(string)> onOkClickFunction, std::function<void(string)> onTextNotAllowedError) : OkCancelForm(Coord(417, 285), Coord(256, 256))
{
	this->Setup(initTextName, Coord(417, 285), Coord(256, 256));
	this->pOnOkClick = onOkClickFunction;
	this->pOnTextNotAllowedError = onTextNotAllowedError;
	init();
}

TextBoxScreen::TextBoxScreen(Text initTextName, std::function<void(string)> onOkClickFunction, std::function<void()> onCancelClickFunction, std::function<void(string)> onTextNotAllowedError) : OkCancelForm( Coord(417,285), Coord( 256,256 ) )
{
	this->Setup(initTextName, Coord(417,285), Coord( 256,256 ));
	this->pOnCancelClickFunction = onCancelClickFunction;
	this->pOnOkClick = onOkClickFunction;
	this->pOnTextNotAllowedError = onTextNotAllowedError;
	init();
}

TextBoxScreen::TextBoxScreen(const TextBoxScreen& other)
{
	this->pTextNotAllowed = other.pTextNotAllowed;
	this->pText = other.pText;
	this->pOnOkClick = other.pOnOkClick;
	this->pOnCancelClickFunction = other.pOnCancelClickFunction;
	this->pOnTextNotAllowedError = other.pOnTextNotAllowedError;
}

void TextBoxScreen::AddInfo(Text addInfo)
{
	auto infoText = std::shared_ptr<Widget>(new Link(addInfo, 14, this->GetPosition() + Coord(96, 96), 16, 260));
	infoText->disable();
	this->Control()->AddWidget(infoText);
}

void TextBoxScreen::SetInitialText(Text text)
{
	this->pText->RefreshText(text.GetCurrentValue());
}

void TextBoxScreen::Setup(Text inittext, Coord dimensions, Coord pos)
{
	OkCancelForm::Setup(dimensions,pos);

	this->pText = std::shared_ptr<Link>( new Link(inittext,14, this->GetPosition() + Coord(36,128),16,350) );
	this->Control()->AddWidget( pText );

}

void TextBoxScreen::init()
{
	pTextNotAllowed = std::set<string>();
}

void TextBoxScreen::HandleEvent( SDL_Event& e)
{
	Screen::HandleEvent(e);
	if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP )
	{
		pReader.HandleEvent(e);
		UpdateLink();
	}
}

Link* TextBoxScreen::GetLink()
{
	return ((Link*)this->pText.get());
}

void TextBoxScreen::UpdateLink()
{
	if( this->pReader.GetText() == "" )
		this->GetLink()->RenewText( " " );
	else
		this->GetLink()->RenewText(this->pReader.GetText());
}

bool TextBoxScreen::CurrentTextAllowed()
{
	string currentText = pReader.GetText();
	auto found = this->pTextNotAllowed.find(currentText);

	return found == this->pTextNotAllowed.end();
}

void TextBoxScreen::close_click( Widget* w)
{
	if (this->pOnCancelClickFunction)
		this->pOnCancelClickFunction();
	this->pReader.ResetText();
}

void TextBoxScreen::ok_click( Widget* w)
{
	string text = pReader.GetText();
	if (CurrentTextAllowed())
	{
		this->CloseScreen();
		if (this->pOnOkClick)
			this->pOnOkClick(text);
	}
	else if (this->pOnTextNotAllowedError)
		pOnTextNotAllowedError(text);
	//GameManager::saveVessel(text);
}