#pragma once
#include<string>
#include"TextDialogue.h"
#include<vector>
#include<memory>

enum DialogueExecution
{
	dOrdered = 1,
	dUnordered = 2
};

enum enDialogState
{
	stFirstLine = 1,
	stTwoLine = 2,
	stFinalLine = 3

};

enum enDialogBoundaryType
{
	bFixed = 1,
	bVariable = 2,
	bBestFit = 3
};

struct DialogLayout
{


};

class Dialogue
{

public:

	Dialogue();
	Dialogue( const ColorRGB& backColor, const ColorRGB& textColor );
	
	SDL_Rect GetDialogueBoundary(double zoomModifier);

	std::shared_ptr<TextDialogue> GetCurrentText();
	std::shared_ptr<TextDialogue> GetPreviousText();
	std::shared_ptr<TextDialogue> GetText(int index);
	void RunDialoge(int ms);

	bool IsFinished();

	void SetExecutionType(DialogueExecution set) { pExecutionType = set; }
	DialogueExecution GetExecutionType() const { return this->pExecutionType; }
	bool IsOrdered() { return this->GetExecutionType() == DialogueExecution::dOrdered; }
	bool IsUnordered() { return this->GetExecutionType() == DialogueExecution::dUnordered; }

	void SetID(std::string set) { this->pID = set; }
	std::string GetID() const { return this->pID; }

	void AddTimedScript(const Text, string reader, int timer);
	void AddConditionalScript(const Text, string reader);
	std::shared_ptr<Link> GetCurrentLink() { return pCurrentTextAsLink; }
	std::shared_ptr<Link> GetPreviousLink() { return pSecondaryTextAsLink; }

	void ChangeCurrentIndex(int index);
	void TriggerFinishedCurrentScript(); 

	bool Repeat() const { return this->pRepeat; }
	void SetRepeat(bool value) { this->pRepeat = value; }

	Coord GetPosition() { return this->pPosition; }
	void SetPosition(Coord set);

	ColorRGB GetTextColor() const { return this->pTextColor; }
	void SetTextColor(SDL_Color& col);

	ColorRGB GetBackgroundColor() const { return this->pBackColor; }
	void SetBackgroundColor(ColorRGB& col) { this->pBackColor = col; }

	ColorRGB GetBorderColor() const { return this->pBorderColor; }
	void SetBorderColor(ColorRGB& col) { this->pBorderColor = col; }

	bool DrawBorder() const { return this->pDrawBorder; }
	void SetDrawBorder(bool set) { this->pDrawBorder = set; }

	bool IsEnabled() const { return this->pEnabled; }
	void Enable(bool set) { this->pEnabled = set; }

	void SetZoomModifier(double set) { this->pZoomModifier = set; }
	double GetZoomModifier() const { return this->pZoomModifier; }
		
	bool DialogueIsShared() const { return this->pSharedDialogue; }
	enDialogState GetState() const { return this->pState; }

	void SetBestFitBoundary();
	void SetFixedBoundary(Coord& size);
	void SetVariableBoundary();

	int GetTextSize() const { return this->pTxtSize; }
	void SetTextSize(int set) { this->pTxtSize = set; }

private:

	void ResetBoundary();
	void CalculateBestFitBoundary();
	void HandleIsFinished();
	void ResetDialog();
	void ResetTexts();
	void ResetLinks();
	void Initialize();
	void UpdateIsFinished();
	void UpdateBoundary();
	SDL_Rect GetDialogueBoundary();
	bool ValidateTextData(int indexID);

	Coord GetLargestTextDimension();
	
	
	void IncrementScript();
	void TriggerFinishedCurrentScript(enDialogState state);

	std::vector<std::shared_ptr<TextDialogue>> pScript;
	
	std::shared_ptr<Link> pCurrentTextAsLink;
	std::shared_ptr<Link> pSecondaryTextAsLink;
	
	//std::shared_ptr<Link> pdummyLink;
	int GetWidthMargin();

	void DialogUpdateCurrentText();
	void DialogUpdatePreviousText();

	Coord pPosition;
	bool pFinished;
	bool pEnabled;
	bool pRepeat;
	bool pDrawBorder;
	bool pSharedDialogue;	//if non shared (if false) => then enabling this dialogue, will disable all other currently active
	
	double pZoomModifier;
	int pIndexIterator;
	int pTxtSize;

	std::string pID;
	DialogueExecution pExecutionType;
	SDL_Rect pBoundary;
	enDialogBoundaryType pBoundaryType;

	

	ColorRGB pBackColor;
	ColorRGB pTextColor;
	ColorRGB pBorderColor;

	enDialogState pState;

};