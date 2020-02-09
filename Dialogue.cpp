#include"Dialogue.h"
#include"DialogueManager.h"
#include"TimedTextDialogue.h"
#include"Statistics.h"




Dialogue::Dialogue()
{
	pBackColor = ColorRGB(200,200,200);
	pTextColor = ColorRGB(255,255,255);
	pBorderColor = ColorRGB();
	Initialize();
}

Dialogue::Dialogue(const ColorRGB& backColor, const ColorRGB& textColor)
{
	pBackColor = backColor;
	pTextColor = textColor;
	pBorderColor = ColorRGB();
	Initialize();
}

void Dialogue::Initialize()
{
	pExecutionType = DialogueExecution::dUnordered;
	pIndexIterator = 0;
	pFinished = false;
	this->pRepeat = false;
	this->pDrawBorder = true;
	this->pSharedDialogue = false;
	this->pBoundaryType = enDialogBoundaryType::bVariable;
	this->pZoomModifier = 1;
	this->ResetBoundary();
	this->pTxtSize = 12;
	ResetLinks();

	this->pEnabled = false;
	this->pState = enDialogState::stFirstLine;

}
void Dialogue::ResetBoundary()
{
	this->pBoundary = { 0,0,-1,-1 };
}


void Dialogue::RunDialoge(int ms)
{
	if (IsFinished() || !IsEnabled())
		return;

	std::shared_ptr<TextDialogue> cText;
	std::shared_ptr<TextDialogue> pText;
	std::shared_ptr<Link> link;

	switch (this->pState)
	{
	case(enDialogState::stFinalLine):

		pText = GetPreviousText();
		pText->Run(ms);
		link = GetPreviousLink();
		link->FadeText(pText->Progress());
		if (pText->IsFinished())
			TriggerFinishedCurrentScript(stFinalLine);
		break;
	case(enDialogState::stTwoLine):

		cText = GetCurrentText();
		pText = GetPreviousText();
		cText->Run(ms);
		pText->Run(ms);
		link = GetPreviousLink();
		link->FadeText(pText->Progress());

		if (cText->IsFinished())
			TriggerFinishedCurrentScript(stTwoLine);
		break;
	case(enDialogState::stFirstLine):
		cText = GetCurrentText();
		cText->Run(ms);

		if (cText->IsFinished())
			TriggerFinishedCurrentScript(stFirstLine);
		break;
	}
}

void Dialogue::TriggerFinishedCurrentScript()
{
	if (!IsEnabled())
		return;

	if (IsFinished())
	{
		if(this->pRepeat)
			this->pFinished = false;
		return;
	}

	TriggerFinishedCurrentScript(this->pState);
}

void Dialogue::TriggerFinishedCurrentScript(enDialogState state)
{
	switch (state)
	{
	case(enDialogState::stFinalLine):
		HandleIsFinished();
		break;
	case(enDialogState::stTwoLine):
		IncrementScript();
		if (pIndexIterator == this->pScript.size())
			this->pState = enDialogState::stFinalLine;
		else
			this->pState = enDialogState::stTwoLine;
		
		break;
	case(enDialogState::stFirstLine):
		if (this->pScript.size() == 1)
			HandleIsFinished();
		else
		{
			IncrementScript();
			this->pState = enDialogState::stTwoLine;
		}
		break;
	}
	this->UpdateBoundary();
}

void Dialogue::UpdateBoundary()
{
	this->pBoundary = GetDialogueBoundary();
}

SDL_Rect Dialogue::GetDialogueBoundary()
{
	SDL_Rect result;
	SDL_Rect boundarybox1;
	SDL_Rect boundarybox2;
	Coord position;
	switch (this->pState)
	{
	case(enDialogState::stFinalLine):
		position = this->GetPreviousLink()->GetPosition();
		result.x = position.x;
		result.y = position.y;
		if (pBoundaryType == enDialogBoundaryType::bVariable)
		{
			boundarybox1 = *this->GetPreviousLink()->GetBoundaryBox();
			result.h = boundarybox1.h;
			result.w = boundarybox1.w;
		}
		else
		{
			if (pBoundaryType == enDialogBoundaryType::bBestFit)
				this->CalculateBestFitBoundary();
			result.h = pBoundary.h;
			result.w = pBoundary.w;
		}
		break;
	case(enDialogState::stTwoLine):
		position = this->GetPreviousLink()->GetPosition();
		result.x = position.x;
		result.y = position.y;
		if (pBoundaryType == enDialogBoundaryType::bVariable)
		{
			boundarybox1 = *this->GetPreviousLink()->GetBoundaryBox();
			boundarybox2 = *this->GetCurrentLink()->GetBoundaryBox();
			result.h = boundarybox1.h + boundarybox2.h;
			result.w = std::fmax(boundarybox1.w, boundarybox2.w);
		}
		else
		{
			if (pBoundaryType == enDialogBoundaryType::bBestFit)
				this->CalculateBestFitBoundary();
			result.h = pBoundary.h;
			result.w = pBoundary.w;
		}
		break;
	case(enDialogState::stFirstLine):
		position = this->GetCurrentLink()->GetPosition();
		result.x = position.x;
		result.y = position.y;
		if (pBoundaryType == enDialogBoundaryType::bVariable)
		{
			boundarybox1 = *this->GetCurrentLink()->GetBoundaryBox();
			result.h = boundarybox1.h;
			result.w = boundarybox1.w;
		}
		else
		{
			if (pBoundaryType == enDialogBoundaryType::bBestFit)
				this->CalculateBestFitBoundary();
			result.h = pBoundary.h;
			result.w = pBoundary.w;
		}
		break;
	}
	return result;
}

SDL_Rect Dialogue::GetDialogueBoundary(double zoomModifier)
{
	if (this->pBoundary.h <= 0 || this->pBoundary.w <= 0)
		if (this->pCurrentTextAsLink.get())
			this->UpdateBoundary();
	SDL_Rect result = this->pBoundary;
	result.x *= zoomModifier;
	result.y *= zoomModifier;
	if (this->pState == enDialogState::stTwoLine && this->pBoundaryType == enDialogBoundaryType::bBestFit)
		result.h *= 2;
	return result;
}

void Dialogue::IncrementScript()
{
	switch (this->GetExecutionType())
	{
	case(DialogueExecution::dOrdered):
		this->ChangeCurrentIndex(pIndexIterator + 1);
		break;
	case(DialogueExecution::dUnordered):
		ChangeCurrentIndex(statistics::GetIntInRange(0, pScript.size() - 1));
		break;
	}
}

void Dialogue::SetBestFitBoundary()
{
	this->pBoundaryType = enDialogBoundaryType::bBestFit;

}

void Dialogue::CalculateBestFitBoundary()
{
	if (this->pBoundary.w < 0 || this->pBoundary.h < 0)
	{
		Coord bestFit = GetLargestTextDimension();
		this->pBoundary.h = std::abs(bestFit.y);
		this->pBoundary.w = std::abs(bestFit.x);
	}
}

void Dialogue::SetFixedBoundary(Coord& size)
{
	this->pBoundaryType = enDialogBoundaryType::bFixed;
	this->pBoundary.h = std::abs(size.y);
	this->pBoundary.w = std::abs(size.x);
}
void Dialogue::SetVariableBoundary()
{
	this->pBoundaryType = enDialogBoundaryType::bVariable;
}

void Dialogue::UpdateIsFinished()
{
	for (int i = 0; i < this->pScript.size(); i++)
	{
		if (pScript[i]->IsFinished() == false)
		{
			pFinished = false;
			return;
		}
	}
	pFinished = true;
}

Coord Dialogue::GetLargestTextDimension()
{
	int largestx = 0;
	int largesty = 0;
	for (int i = 0; i < this->pScript.size(); i++)
	{
		Coord size = this->pScript[i]->GetTextDimension(this->pTxtSize);
		if (size.x > largestx)
			largestx = size.x;
		if (size.y > largesty)
			largesty = size.y;
	}
	return Coord(largestx, largesty);
}

bool Dialogue::IsFinished()
{
	return pFinished;
}

std::shared_ptr<TextDialogue> Dialogue::GetCurrentText()
{
	if (pIndexIterator >= this->pScript.size())
		return nullptr;
	return this->pScript[pIndexIterator];
}

std::shared_ptr<TextDialogue> Dialogue::GetPreviousText()
{
	if (pIndexIterator < 1 || this->pScript.size() < 2)
		return nullptr;
	return this->pScript[pIndexIterator - 1];
}

std::shared_ptr<TextDialogue> Dialogue::GetText(int index)
{
	for (int i = 0; i < this->pScript.size(); i++)
	{
		if (pScript[i]->GetID() == index)
			return pScript[i];
	}
	return nullptr;
}

bool Dialogue::ValidateTextData(int indexID)
{
	if (this->IsOrdered())
	{
		if (this->pScript.size() >= indexID)
			throw new std::exception("Error script size is smaller than the index of added script");
	}
	return true;
}

void Dialogue::HandleIsFinished()
{
	//if (this->pRepeat)
		ResetDialog();
	//else
	//	pFinished = true;
}

void Dialogue::ResetDialog()
{
	ResetTexts();
	ResetLinks();
	pFinished = true;

	this->pState = enDialogState::stFirstLine;
	switch (this->GetExecutionType())
	{
	case(DialogueExecution::dOrdered):
		this->ChangeCurrentIndex(0);
		break;
	case(DialogueExecution::dUnordered):
		ChangeCurrentIndex(statistics::GetIntInRange(0, pScript.size() - 1));//pIndexIterator = statistics::GetIntInRange(0, pScript.size() - 1);
		break;
	}
}

void Dialogue::ResetTexts()
{
	int size = this->pScript.size();
	for (int i = 0; i < size; i++)
	{
		pScript[i]->ResetState(TextDialogeState::txDisable);
	}
}

void Dialogue::ResetLinks()
{
	this->pCurrentTextAsLink = std::shared_ptr<Link>(new Link(Text::TextEmpty(), this->pTxtSize, Coord(0, 0), 12, 32, pTextColor, colorFromEnum(EnumColor::INVISIBLE) ));
	//this->pCurrentTextAsLink->autosize = true;

	this->pSecondaryTextAsLink = std::shared_ptr<Link>(new Link(Text::TextEmpty(), this->pTxtSize, Coord(0, 0), 12, 32, pTextColor, colorFromEnum(EnumColor::INVISIBLE)));
	//this->pSecondaryTextAsLink->autosize = true;
}

void Dialogue::ChangeCurrentIndex(int index)
{
	pIndexIterator = index;
	DialogUpdateCurrentText();
	if(pIndexIterator>0)
		DialogUpdatePreviousText();
}

void Dialogue::DialogUpdateCurrentText()
{
	auto cText = GetCurrentText();
	if (cText.get())
	{
		this->pCurrentTextAsLink->RenewText(cText->GetText().GetID());
		this->pCurrentTextAsLink->SetPosition(this->pPosition);
		cText->SetState(TextDialogeState::txDisplayed);
	}
	else
	{
		this->pCurrentTextAsLink.reset();
	}
}

void Dialogue::DialogUpdatePreviousText()
{
	auto cText = GetPreviousText();
	if (cText.get())
	{
		Coord newPosition = this->pPosition;
		this->pSecondaryTextAsLink->RenewText(cText->GetText().GetID());

		if (this->pCurrentTextAsLink.get())
		{
			newPosition = this->pPosition.yadd(-this->pCurrentTextAsLink->GetBoundaryBox()->h*this->pZoomModifier);
			TimedTextDialogue* ttd = (TimedTextDialogue*)cText.get();
			ttd->SetInitialTimeRemaining(((TimedTextDialogue*)GetCurrentText().get())->GetInitialTimeRemaining());
			ttd->SetState(TextDialogeState::txDisable);
		}
		else
			cText->ResetState(TextDialogeState::txDisable);
		this->pSecondaryTextAsLink->SetPosition(newPosition);
	}
	else
	{
		this->pSecondaryTextAsLink.reset();
	}

}

int Dialogue::GetWidthMargin()
{
	if (this->pCurrentTextAsLink.get())
		return std::fmin(7, this->pCurrentTextAsLink->GetBoundaryBox()->w - pSecondaryTextAsLink->GetBoundaryBox()->w);
	else
		return 7;
}

void Dialogue::SetTextColor(SDL_Color& col)
{
	this->pCurrentTextAsLink->RenewText(col);
	this->pSecondaryTextAsLink->RenewText(col);
}

//void Dialogue::SetBackgroundColor(ColorRGB& col)
//{
//	this->pCurrentTextAsLink->SetBackgroundColor(col);
//	this->pSecondaryTextAsLink->SetBackgroundColor(col);
//}

void Dialogue::SetPosition(Coord set)
{
	this->pPosition = set;
	if (this->pCurrentTextAsLink.get())
		this->pCurrentTextAsLink->SetPosition(set);

}

void Dialogue::AddTimedScript(const Text text, string reader, int timer)
{
	if (!ValidateTextData(text.GetIndex()))
		return;

	TimedTextDialogue* newDialoge = new TimedTextDialogue();
	Text pText = text;
	newDialoge->SetText(pText);
	newDialoge->SetReaderID(reader);
	newDialoge->SetID(text.GetIndex());
	newDialoge->SetInitialTimeRemaining(timer);
	this->pScript.push_back(std::shared_ptr<TextDialogue>(newDialoge));
	this->ResetDialog();
}

void Dialogue::AddConditionalScript(const Text text, string reader)
{

}