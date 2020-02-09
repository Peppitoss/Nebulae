#pragma once
#include"SingleButtonTablet.h"

enum enErrorScreenType
{
	errAssert = 1,
	errException = 2
};

class scrError : public SingleButtonTablet
{
public:

	scrError(enErrorScreenType type, string ErrorMessage);

	void Initialize( string ErrorMessage );
	void HandleActiveWidget(SDL_Event& e, Widget* widget) override;

private:

	string GetButtonText(enErrorScreenType type);
	string AbortTxt() const { return "Abort"; }
	string ExceptionTxt() const { return "Continue"; }

	enErrorScreenType pType;

};