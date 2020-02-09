#include"TimedTextDialogue.h"

TimedTextDialogue::TimedTextDialogue()
	:TextDialogue()
{
	SetType(TextDialogueType::txTimed);
}

void TimedTextDialogue::Run(int ms)
{
	if (this->pTimeRemaining < 0)
		this->SetState(TextDialogeState::txtFinished);

	this->pTimeRemaining -= ms;
	
}

void TimedTextDialogue::ResetState(TextDialogeState newState) 
{ 
	TextDialogue::ResetState(newState);
	pTimeRemaining = pInitialTime; 
};

double TimedTextDialogue::Progress() 
{
	return (double)pTimeRemaining / pInitialTime; 
}