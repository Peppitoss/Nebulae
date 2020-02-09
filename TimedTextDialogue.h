#pragma once
#include"TextDialogue.h"

class TimedTextDialogue : public TextDialogue
{

public:

	TimedTextDialogue();

	void Run(int ms) override;
	void SetInitialTimeRemaining(int set) { this->pTimeRemaining = set; pInitialTime = set; }
	void SetTimeRemaining(int set) { this->pTimeRemaining = this->pTimeRemaining > pInitialTime ? pInitialTime : this->pTimeRemaining; }
	int GetInitialTimeRemaining() const { return this->pInitialTime; }
	void ResetState(TextDialogeState newState) override;

	double Progress() override;

protected:



private:

	int pInitialTime;
	int pTimeRemaining;

};