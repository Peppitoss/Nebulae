#pragma once
#include<string>
#include"SingleButtonTablet.h"
#include"ShipSelectTablet.h"
#include"scrError.h"
#include"scrTransaction.h"
#include"MessageTablet.h"

class TabletGenerator
{
public:
	static std::shared_ptr<SingleButtonTablet> GenerateTablet(std::string name);
	static std::vector<std::shared_ptr<ShipSelectTablet>> GenerateShipSelector(std::string name);
 
	static std::shared_ptr<MessageTablet> GenerateMessageTablet(MessageTabletID msgID, string Message, string ButtonText, bool UnpauseOnExit);
	static std::shared_ptr<scrError> GenerateErrorMessage(std::string name, string Message);
	static std::shared_ptr<scrTransaction> GenerateTransactionMessage(string type, string Message);
};