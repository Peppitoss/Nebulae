#pragma once
#include"OkCancelForm.h"
#include"KeyReader.h"
#include<functional>
#include"Link.h"

class TextBoxScreen : public OkCancelForm
{

public:

	TextBoxScreen();
	TextBoxScreen(Text initTextName, std::function<void(string)> onOkClickFunction, std::function<void(string)> onTextNotAllowedError);
	TextBoxScreen(Text initTextName, std::function<void(string)> onOkClickFunction, std::function<void()> onCancelClickFunction, std::function<void(string)> onTextNotAllowedError );
	TextBoxScreen(const TextBoxScreen& other);

	void AddInfo(Text addInfo);
	void SetInitialText(Text text);

	void SetTextNotAllowed(std::set<string> set) { this->pTextNotAllowed = set; }
	void HandleEvent( SDL_Event& e) override;

protected:

	KeyReader pReader;
	std::shared_ptr<Link> pText;

	Link* GetLink();

	void init();
	void UpdateLink();
	void close_click( Widget* w);
	void ok_click(Widget* w);
	void Setup(Text inittext, Coord dimensions, Coord pos );

private:

	bool CurrentTextAllowed();

	std::set<string> pTextNotAllowed;

	std::function<void(string)> pOnOkClick;
	std::function<void()> pOnCancelClickFunction;
	std::function<void(string)> pOnTextNotAllowedError;

};