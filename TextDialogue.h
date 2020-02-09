#pragma once
#include"Text.h"
#include"Link.h"

enum TextDialogeState
{
	txDisable = 1,
	txDisplayed = 2,
	txtFinished = 3

};

enum TextDialogueType
{
	txTimed = 1,
	txConditional = 2
};

class TextDialogue
{
public:

	TextDialogue();

	virtual void Run(int ms);
	bool IsDisabled() const { return this->pState == TextDialogeState::txDisable; }
	bool IsActive() const { return this->pState == TextDialogeState::txDisplayed; }
	bool IsFinished() const { return this->pState == TextDialogeState::txtFinished; }

	std::shared_ptr<Link> GenerateLinkFromText(int txtSize, ColorRGB& backgrounColor, ColorRGB& textColor);

	void SetState(TextDialogeState set) { this->pState = set; }
	TextDialogeState GetState() const { return this->pState; }

	TextDialogueType GetType() const { return this->pType; }

	void SetReaderID(string set) { this->pReader = set; }
	string GetReaderID() const { return this->pReader; }

	void SetText(Text&& set) { this->pText = std::move(set); }
	void SetText(Text set) { this->pText = set; }
	Text GetText() { return this->pText; }

	void SetID(int set) { this->pID = set; }
	int GetID() const { return this->pID; }

	Coord GetTextDimension( int txtSize );

	virtual double Progress() { return 1; };

	virtual void ResetState(TextDialogeState newState) { SetState(newState); };

protected:

	void SetType(TextDialogueType set) { this->pType = set; }

private:

	void Initialize();

	TextDialogeState pState;
	TextDialogueType pType;
	string pReader;
	Text pText;
	int pID;

};