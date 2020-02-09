#include"TabletGenerator.h"
#include"BackgroundCollection.h"

std::shared_ptr<SingleButtonTablet> TabletGenerator::GenerateTablet(std::string name)
{
	if (name == "hydra")
	{
		return std::shared_ptr<SingleButtonTablet>(new SingleButtonTablet(466, 280, Coord(350, 350), ImportImage_TabletHydra, "close"));
	}
	if (name == "irgenbunkroom")
	{
		return std::shared_ptr<SingleButtonTablet>(new SingleButtonTablet(450, 250, Coord(350, 350), ImportImage_TabletIrgenBunkRoom, "close"));
	}
	return nullptr;


}

std::vector<std::shared_ptr<ShipSelectTablet>> TabletGenerator::GenerateShipSelector(std::string name)
{
	if (name == "chiron" || name == "gibbs")
	{
		std::vector<std::shared_ptr<ShipSelectTablet>> result = std::vector<std::shared_ptr<ShipSelectTablet>>();

		int w = 500;
		int h = 257;
		if (name == "gibbs")
		{
			result.push_back(std::shared_ptr<ShipSelectTablet>(new ShipSelectTablet(w, h, Coord(350, 350), ImportImage_TableSelectGibbs, "PLAYERW2", true, true)));	//"GIBBS"
			//result.push_back(std::shared_ptr<ShipSelectTablet>(new ShipSelectTablet(w, h, Coord(350, 350), ImportImage_TableSelectChiron, "CHIRON", true, true)));
		}
		else
		{
			result.push_back(std::shared_ptr<ShipSelectTablet>(new ShipSelectTablet(w, h, Coord(350, 350), ImportImage_TableSelectChiron, "CHIRON", true, true)));	//""
			//result.push_back(std::shared_ptr<ShipSelectTablet>(new ShipSelectTablet(w, h, Coord(350, 350), ImportImage_TableSelectGibbs, "GIBBS", true, true)));
		}


		return result;
	}
}

std::shared_ptr<MessageTablet> TabletGenerator::GenerateMessageTablet(MessageTabletID msgID, string Message, string ButtonText, bool unpauseOnExit)
{
	return std::shared_ptr<MessageTablet>(new MessageTablet(msgID, Message, ButtonText, unpauseOnExit));
}

std::shared_ptr<scrError> TabletGenerator::GenerateErrorMessage(std::string name, string Message)
{
	if (name == "assert")
	{
		return std::shared_ptr<scrError>(new scrError(enErrorScreenType::errAssert, Message));
	}
	else
	{
		return std::shared_ptr<scrError>(new scrError(enErrorScreenType::errException, Message));
	}
}

std::shared_ptr<scrTransaction> TabletGenerator::GenerateTransactionMessage(string type, string Message)
{
	if (type == "revert" || type == "Revert")
	{
		return std::shared_ptr<scrTransaction>(new scrTransaction(enTransactionType::trRevert, Message));
	}
	else
	{
		return std::shared_ptr<scrTransaction>(new scrTransaction(enTransactionType::trFailed, Message));
	}
	
}