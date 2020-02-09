#pragma once
#include"TextDialogue.h"
#include"Language.h"
#include<string>
#include<memory>

using std::string;



class DialogueObject
{

public:
	DialogueObject();

	std::shared_ptr<TextDialogue> GenerateTextDialogue();

	Text pText;
	string pReaderID;
	int timer;
	TextDialogueType pType;

	void Initialize();


};