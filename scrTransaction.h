#pragma once
#include"SingleButtonTablet.h"

enum enTransactionType
{
	trRevert = 1,
	trFailed = 2
};


class scrTransaction : public SingleButtonTablet
{
public:

	scrTransaction(enTransactionType type, string message);

	void Initialize(string ErrorMessage);
	void HandleActiveWidget(SDL_Event& e, Widget* widget) override;

private:

	enTransactionType pType;

	string GetButtonText(enTransactionType type);
	string AbortTxt() const { return "Abort Transaction"; }
	string RevertText() const { return "Revert Transaction"; }

};