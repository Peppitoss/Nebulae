#include"Text.h"
#include"TextGlobalList.h"
#include"DialogueManager.h"

const LanguageCode CURRENTLANGUAGE = LanguageCode::ENG;

Text::Text()
{
	Initialize();
}

//Generates a custom text, with no language id
Text::Text(string customText)
{
	this->pCurrentValue = customText;
	this->pCurrentCode = CUSTOM;
	this->pDIalogueID = customText;
}

Text::Text(string filename, string name)
	: Text( LanguageCode::ENG, filename, name, 0)
{

}

Text::Text(LanguageCode code, string filename, string dialogeID, int id)
{
	this->pCurrentValue = "";
	Text loadedText = LoadTextForLanguage(code, filename, dialogeID, id, false);
	*this = loadedText;
}

Text Text::ChangeTextFromCode(LanguageCode code) const
{
	if (pCurrentCode == CUSTOM)
		return *this;

	Text newText = LoadTextForLanguage(code, this->pFileName, this->pDIalogueID, this->pIndexID, this->pIsFromDialogue);
	return *this;
}

Text Text::LoadTextForLanguage(LanguageCode language, string filename, string dialogeID, int id, bool dialogueText)
{
	if (dialogueText)
		return DialogueManager::GetTextForLanguage(language, filename, dialogeID, id);
	return TextGlobalList::GetText(language, filename, dialogeID);
}

string Text::GetCurrentValue() const
{
	if (this->pCurrentCode == CUSTOM)
		return this->pCurrentValue;

	if (this->pCurrentCode != CURRENTLANGUAGE)
		return ChangeTextFromCode(CURRENTLANGUAGE).GetCurrentValue();
	return this->pCurrentValue;
}
void Text::SetCurrentValue(string set)
{
	this->pCurrentValue = set;
}

Text Text::TextEmpty()
{
	Text result = Text("");
	return result;
}

void Text::Initialize()
{
	pIndexID = 0;
	pDIalogueID = "";
	pFileName = "";
	pCurrentValue = "";
	pCurrentCode = CURRENTLANGUAGE;
	pIsFromDialogue = false;
}

BtnText::BtnText() : Text()
{
}

BtnText::BtnText(string name) : Text("buttontxt", name)
{
}

MissionText::MissionText() : Text()
{
}

MissionText::MissionText(string name) : Text("missiontxt", name)
{

}