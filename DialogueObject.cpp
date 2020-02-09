#include"DialogueObject.h"
#include"TimedTextDialogue.h"

DialogueObject::DialogueObject()
{
	Initialize();
}

void DialogueObject::Initialize()
{
	pText = Text();
	pReaderID = "";
	timer = 0;
	pType = TextDialogueType::txTimed;
	//language = LanguageCode::ENG;
}

std::shared_ptr<TextDialogue> DialogueObject::GenerateTextDialogue()
{
	switch (pType)
	{
	case(TextDialogueType::txConditional):
		return std::shared_ptr<TextDialogue>(nullptr);
	case(TextDialogueType::txTimed):
		TimedTextDialogue* result = new TimedTextDialogue();
		result->SetInitialTimeRemaining(timer);
		Text text = Text(pText.GetLanguageCode(), pText.GetFilename(), pText.GetID(), pText.GetIndex());
		result->SetText(text);
		return std::shared_ptr<TextDialogue>(result);
	}
}

