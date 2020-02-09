#pragma once
#include<string>
#include"Language.h"

using std::string;

extern const LanguageCode CURRENTLANGUAGE;

class Text
{
public:

	Text();
	Text(string customText);
	Text(string filename, string name);
	Text(LanguageCode code, string filename, string dialogeID, int id);

	Text ChangeTextFromCode(LanguageCode code) const;
	static Text LoadTextForLanguage(LanguageCode language, string filename, string dialogeID, int id, bool dialogueText );

	int GetIndex() const { return this->pIndexID; }
	void SetIndex(int set) { this->pIndexID = set; }

	string GetID() const { return this->pDIalogueID; }
	void SetID(string set) { this->pDIalogueID = set; }

	string GetFilename() const { return this->pFileName; }
	void SetFilename(string set) { this->pFileName = set; }

	LanguageCode GetLanguageCode() const { return pCurrentCode; }
	void SetLanguageCode(LanguageCode set) { this->pCurrentCode = set; }

	static Text TextEmpty();

	string GetCurrentValue() const;
	void SetCurrentValue(string set);
	void SetIsDialogueType(bool set) { this->pIsFromDialogue = set; }

	bool Hasvalue() const { return this->pCurrentValue != ""; }

private:

	void Initialize();
	bool IsCustom() const { return this->pCurrentCode == CUSTOM; }

	int pIndexID;
	bool pIsFromDialogue;
	string pDIalogueID;
	string pFileName;

	string pCurrentValue;
	LanguageCode pCurrentCode;

};

class BtnText : public Text
{
public:

	BtnText();
	BtnText(string name);
};

class MissionText : public Text
{
public:

	MissionText();
	MissionText(string name);
};