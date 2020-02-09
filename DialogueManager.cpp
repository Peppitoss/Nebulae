#include"DialogueManager.h"
#include"DialogueReader.h"

//const string dialogfilename = "dialog";

std::vector<std::shared_ptr<Dialogue>> DialogueManager::pDialogues = std::vector<std::shared_ptr<Dialogue>>();
string DialogueManager::pCurrentFileName = "";
FileReader DialogueManager::pFileReader = FileReader();
double DialogueManager::ZoomModifier = 0.4;

void DialogueManager::LoadDialogues(LanguageCode code, string filename, bool clearExistingDialogues)
{
	if (clearExistingDialogues)
		ClearDialogues();

	SetupFileReader(code, filename, true);
	while (pFileReader.IterateOneHeader())
	{
		auto dialog = DialogueReader::GenerateDialoguePointer();
		AddDalogue(dialog);
		DialogueReader::Reset();
	}
}

void DialogueManager::SetupFileReader(LanguageCode code, string filename, bool UpdateManagerFileName)
{
	DialogueReader::SetLanguage(code);
	DialogueReader::SetFilename(filename);
	pFileReader.LoadFile("../Data/Dialogue/", filename);
	if(UpdateManagerFileName)
		pCurrentFileName = filename;
}

void DialogueManager::ConvertDialoguesToLanguage(LanguageCode code)
{
	LoadDialogues(code, DialogueManager::pCurrentFileName, true);
}

Text DialogueManager::GetTextForLanguage(LanguageCode code, string dialogueID, int indexID)
{
	return GetTextForLanguage(code, pCurrentFileName, dialogueID, indexID);
}

Text DialogueManager::GetTextForLanguage(LanguageCode code, string filename, string dialogueID, int indexID)
{
	SetupFileReader(code, filename, false);
	return Text();

	while (pFileReader.IterateOneHeader())
	{
		Dialogue currentdialog = DialogueReader::GenerateDialogue();
		if (currentdialog.GetID() == dialogueID)
		{
			auto result = currentdialog.GetText(indexID);
			if (result != nullptr)
				return result->GetText();
		}
	}

	return Text();
}

void DialogueManager::AddDalogue(std::shared_ptr<Dialogue> dia)
{
	if (dia == nullptr)
		return;
	if (ValidateNewDialogue(dia) == false)
		throw new std::exception("New dialogue already exists in dialogue set");
	pDialogues.push_back(dia);
}

std::shared_ptr<Dialogue> DialogueManager::GetDialogue(string name)
{
	for (int i = 0; i < pDialogues.size(); i++)
	{
		if (pDialogues[i]->GetID() == name)
			return pDialogues[i];
	}
	return nullptr;
}

bool DialogueManager::EnableDialogue(string name)
{
	auto dia = GetDialogue(name);
	if (dia != nullptr)
	{
		EnableDialogue(dia);
		return true;
	}
	return false;
}

bool DialogueManager::DisableDialogue(string name)
{
	auto dia = GetDialogue(name);
	if (dia != nullptr)
	{
		dia->Enable(false);
		return true;
	}
	return false;
}

void DialogueManager::DisableAllDialogues()
{
	for (int i = 0; i < pDialogues.size(); i++)
		pDialogues[i]->Enable(false);
}

bool DialogueManager::ToggleDialogue(string name)
{
	auto dia = GetDialogue(name);
	if (dia != nullptr)
	{
		ToggleDialogue(dia);
		return true;
	}
	return false;
}

void DialogueManager::ToggleDialogue(std::shared_ptr<Dialogue> dia)
{
	bool ToggleIsEnable = !dia->IsEnabled();
	if (ToggleIsEnable)
		EnableDialogue(dia);
	else
		dia->Enable(false);
}

void DialogueManager::EnableDialogue(std::shared_ptr<Dialogue> dia )
{
	dia->SetZoomModifier(DialogueManager::ZoomModifier);
	if (dia->DialogueIsShared() == false)
		DisableAllDialogues();
	dia->Enable(true);
}

bool DialogueManager::ExecuteDialogueConversation(string& name)
{
	auto dia = GetDialogue(name);
	if (dia != nullptr)
	{
		if (dia->IsEnabled())
			dia->TriggerFinishedCurrentScript();
		else
			EnableDialogue(dia);
		return true;
	}
	return false;
}

bool DialogueManager::ValidateNewDialogue(std::shared_ptr<Dialogue> dia)
{
	for (int i = 0; i < pDialogues.size(); i++)
	{
		if (pDialogues[i]->GetID() == dia->GetID())
			return false;
	}
	return true;
}

void DialogueManager::ClearDialogues()
{
	for (int i = 0; i < pDialogues.size(); i++)
		pDialogues[i].reset();
	pDialogues.clear();
}