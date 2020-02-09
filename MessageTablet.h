#pragma once
#include"SingleButtonTablet.h"

enum MessageTabletID
{
	msgNoID = -1,
	msgStartLevel = 0,
	msgEndLevel = 1
};

class MessageTablet : public SingleButtonTablet
{
public:

	MessageTablet(MessageTabletID ID, string message, string ButtonText, bool unpauseOnExit );
	void HandleActiveWidget(SDL_Event& e, Widget* widget) override;

private:

	MessageTabletID pID;
	bool pUnpauseOnExit;
	string pButtonText;
	void Initialize(string messagetext);

};