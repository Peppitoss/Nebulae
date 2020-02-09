#pragma once
#include"Dialogue.h"
#include"FileReader.h"
#include<string>

using std::string;

//const extern string dialogfilename;

class DialogueManager
{
public:

	static void LoadDialogues(LanguageCode code, string filename, bool clearExistingDialogues);

	static std::shared_ptr<Dialogue> GetDialogue(string name);
	static bool EnableDialogue(string name);
	static bool DisableDialogue(string name);
	static bool ToggleDialogue(string name);
	static bool ExecuteDialogueConversation(string& name);

	static void ConvertDialoguesToLanguage(LanguageCode code);
	static Text GetTextForLanguage(LanguageCode code, string dialogueID, int indexID);
	static Text GetTextForLanguage(LanguageCode code, string fileName, string dialogueID, int indexID);

	static std::vector<std::shared_ptr<Dialogue>>::const_iterator const begin() { return pDialogues.begin(); }
	static std::vector<std::shared_ptr<Dialogue>>::const_iterator const end() { return pDialogues.end(); }

	static void SetZoomModifier(double set) { ZoomModifier = set; }

private:

	static bool ValidateNewDialogue(std::shared_ptr<Dialogue> dia);
	static void AddDalogue(std::shared_ptr<Dialogue> dia);
	static void SetupFileReader(LanguageCode code, string filename, bool UpdateManagerFileName );

	static void DisableAllDialogues();
	static void EnableDialogue(std::shared_ptr<Dialogue>);
	static void ToggleDialogue(std::shared_ptr<Dialogue>);
	static string pCurrentFileName;
	static void ClearDialogues();
	static std::vector<std::shared_ptr<Dialogue>> pDialogues;
	static FileReader pFileReader;
	static double ZoomModifier;	//1 is no zoom, 0.4 is zoom in. 1.4 is zoom out

};