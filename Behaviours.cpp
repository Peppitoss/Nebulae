#include"Behaviours.h"
#include"GameManager.h"

const int PlayerModelID = -1;

AIModel::AIModel()
{


}
//
//std::shared_ptr<VesselAI> AIModel::GetAIModel(enAIModel model, std::shared_ptr<Vessel> vessel)
//{
//	switch (model)
//	{
//	case(enAIModel::NOACTION):
//		return std::shared_ptr<VesselAI>(new VesselAI(vessel.get()));
//	case(enAIModel::MOVETOTARGETCOORD):
//		return std::shared_ptr<VesselAI>(new VesselAI(vessel.get(), GenerateTravelToCoordTree(model.GetTargetPosition())));
//	case(enAIModel::AIPROBER):
//		return std::shared_ptr<VesselAI>(new VesselAI(vessel.get(), ProberAI((Vessel*)GameManager::CurrentInstanceManager()->->AsUnitEntity())));
//		break;
//		//case(enActions::HOLDPOSITION):
//		//	SetupVesselStandardAI(model, vessel);
//		//	break;
//		//case(enActions::TEMPLATEFUNCTION):
//		//	SetupVesselStandardAI(model, vessel);
//		//	break;
//		//case(enActions::TRACKTARGETENTITY):
//		//	SetupVesselStandardAI(model, vessel);
//		//	break;
//
//	}
//}

BehaviorTree AIModel::ProberAI()
{
	return ProberAI(PlayerModelID);
}

BehaviorTree AIModel::ProberAI(int targetvesselID)
{
	BehaviorTree result = BehaviorTree(enNodeType::SELECTOR);
	auto rootNode = result.RootNode;

	result.VesselInitialization = [](Vessel& vessel)
	{
		vessel.SetDrive(EnumEntityDrive::DynamicVelocityHeading);
		vessel.SetAbsVelocity(50); 
	};

	auto toAdd = BehaviorNode(enNodeType::ACTION);
	toAdd.GetActionArg().Active = true;
	toAdd.GetActionArg().TargetVesselID = targetvesselID;
	toAdd.GetActionArg().Function = [&](ActionArgs& arg, Vessel& vessel)
	{
		vessel.setGuidanceWaypoint(primitives::waypoint(arg.TargetObject->getPosition(), 400));
		FireWhenTargetInSight(arg.TargetObject->getPosition(), vessel, 100);
		return true;
	};

	result.setTreeLoopIterations(-1);
	result.AddNode(rootNode, toAdd);
	return result;
}


BehaviorTree AIModel::GenerateTravelToCoordTree(const primitives::waypoint& wp)
{
	BehaviorTree bTree = BehaviorTree(enNodeType::SELECTOR);

	auto toAdd = BehaviorNode(enNodeType::ACTION);
	toAdd.GetActionArg().Active = true;
	toAdd.GetActionArg().Function = [](ActionArgs& arg, Vessel& vessel) {	return true; };//->TargetCoord = new primitives::waypoint(wp);

	bTree.AddNode(bTree.RootNode, toAdd);
	return bTree;
}

void AIModel::FireWhenTargetInSight(const Coord& TargetVesselPosition, Vessel & vessel, double fireAtDistance)
{
	if (geometry::realDistanceQuadratic(TargetVesselPosition, vessel.getPosition()) >= std::pow(fireAtDistance, 2))
		return;

	double angleDifference = vessel.AngleDifferenceBasedOnVesselHeading(TargetVesselPosition);
	FireWhenTargetInSight(angleDifference, std::_Pi / 8, vessel);
}

void AIModel::FireWhenTargetInSight(double angleDifferenceToTarget, double angleBound, Vessel & currentVessel)
{
	if (abs(angleDifferenceToTarget) < std::_Pi / 8)
		currentVessel.Fire();
	else
		currentVessel.DeFire();
}

//namespace AITree
//{

	//BehaviorTree GenerateInterceptTarget(Vessel* target, bool fireOn)
	//{
	//	BehaviorTree result = BehaviorTree(enNodeType::SELECTOR);
	//	auto rootNode = result.RootNode();

	//	auto toAdd = BehaviorNode(enNodeType::ACTION);
	//	toAdd.GetActionArg().Active = true;
	//	toAdd.GetActionArg().TargetObject = target;

	//	if (fireOn)
	//		toAdd.GetActionArg().Function = InterceptAggressive;
	//	else
	//		toAdd.GetActionArg().Function = InterceptNoFire;

	//	result.setTreeLoopIterations(-1);
	//	result.AddNode(rootNode, toAdd);
	//	return result;
	//}

	


	/*BehaviorTree* GenerateInterceptTarget(Vessel* target, bool fireOn)
	{
		BehaviorNode* rootNode = new BehaviorNode(enNodeType::SEQUENCE);
		BehaviorTree* bTree = new BehaviorTree(rootNode);

		rootNode->addChild(new BehaviorNode(enNodeType::ACTION, bTree));
		bTree->getLastInsertedNode()->getActionArg().Active = true;
		bTree->getLastInsertedNode()->getActionArg().TargetObject = target;
		if (fireOn)
			bTree->getLastInsertedNode()->Function = IntereceptAggressive;
		else
			bTree->getLastInsertedNode()->Function = IntereceptNoFire;

		bTree->setTreeLoopIterations(-1);
		return bTree;

	}*/

	//BehaviorTree* generateTrackVessel(DynamicEntity* target)
	//{
	//	BehaviorNode* rootNode = new BehaviorNode(enNodeType::SELECTOR);
	//	BehaviorTree* bTree = new BehaviorTree(rootNode);
	//	rootNode->addChild(new BehaviorNode(enActions::TRACKTARGETENTITY, bTree));
	//	bTree->getLastInsertedNode()->getActionArg()->active = true;
	//	bTree->getLastInsertedNode()->getActionArg()->targetObject = target;
	//	//bTree->getLastInsertedNode()->getActionArg()->templateActionFunction = &Dock;
	//	bTree->setTreeLoopIterations(5000); //see treeFinish() loops are disabled, runs infinite
	//	return bTree;
	//}


	//BehaviorTree* generateTravelToTargetIteration(DynamicEntity* target) //use delete afterwards
	//{
	//	BehaviorNode* rootNode = new BehaviorNode(enNodeType::SEQUENCE);
	//	BehaviorTree* bTree = new BehaviorTree(rootNode);

	//	rootNode->addChild(new BehaviorNode(enActions::HOLDPOSITION, bTree));
	//	bTree->getLastInsertedNode()->getActionArg()->active = true;
	//	bTree->getLastInsertedNode()->getActionArg()->ActionTimeSpan = 1000 * 100;
	//	bTree->getLastInsertedNode()->getActionArg()->timedAction = true;

	//	rootNode->addChild(new BehaviorNode(enActions::MOVETOTARGETENTITY, bTree));
	//	bTree->getLastInsertedNode()->getActionArg()->active = true;
	//	bTree->getLastInsertedNode()->getActionArg()->targetObject = target;

	//	bTree->setTreeLoopIterations(5000); //see treeFinish() loops are disabled, runs infinite
	//	return bTree;
	//}

	


	//old type, is this tree obsolete?
	//BehaviorTree* generateEngageTargetIteration_1(Vessel* target) //use delete afterwards
	//{
	//	BehaviorNode* rootNode = new BehaviorNode(enNodeType::SEQUENCE);
	//	BehaviorTree* bTree = new BehaviorTree(rootNode);

	//	rootNode->addChild(new BehaviorNode(enActions::HOLDPOSITION, bTree));
	//	bTree->getLastInsertedNode()->getActionArg()->active = true;
	//	bTree->getLastInsertedNode()->getActionArg()->ActionTimeSpan = 1000 * 15;
	//	bTree->getLastInsertedNode()->getActionArg()->timedAction = true;

	//	rootNode->addChild(new BehaviorNode(enActions::MOVETOTARGETENTITY, bTree));
	//	bTree->getLastInsertedNode()->getActionArg()->active = true;
	//	bTree->getLastInsertedNode()->getActionArg()->targetObject = target;
	//	bTree->getLastInsertedNode()->getActionArg()->targetRadiusOfIntercept = 50;

	//	rootNode->addChild(new BehaviorNode(enActions::MOVETOTARGETCOORD, bTree));
	//	bTree->getLastInsertedNode()->getActionArg()->active = true;
	//	bTree->getLastInsertedNode()->getActionArg()->targetObject = target;
	//	bTree->getLastInsertedNode()->getActionArg()->targetCoord = new primitives::waypoint();
	//	bTree->getLastInsertedNode()->getActionArg()->templateActionFunction = &CrossInterceptTarget;

	//	bTree->setTreeLoopIterations(5000); //see treeFinish() loops are disabled, runs infinite
	//	return bTree;
	//}

	//obsolete??
	//BehaviorTree* generateTravelToTargetTree(DynamicEntity* target) //Pure DP
	//{
	//	BehaviorNode* rootNode = new BehaviorNode(enNodeType::SELECTOR);
	//	BehaviorTree* bTree = new BehaviorTree(rootNode);
	//	rootNode->addChild(new BehaviorNode(enActions::MOVETOTARGETENTITY, bTree));
	//	bTree->getLastInsertedNode()->getActionArg()->active = true;
	//	bTree->getLastInsertedNode()->getActionArg()->targetObject = target;
	//	//bTree->getLastInsertedNode()->getActionArg()->ActionTimeSpan = 1000*50;
	//	//bTree->getLastInsertedNode()->getActionArg()->timedAction = true;
	//	bTree->setTreeLoopIterations(5000); //see treeFinish() loops are disabled, runs infinite
	//	return bTree;
	//}

	//bool CrossInterceptTarget(ActionArgs & args, Vessel & currentVessel)
	//{
	//	Coord targetposition = args.targetObject->getPosition();
	//	double angleToTarget = -geometry::angleBetweenTwoPoints(currentVessel.getPosition(), targetposition);
	//	Coord newCoordTarget = geometry::pointAlongLine(targetposition, angleToTarget, 150);
	//	*args.targetCoord = primitives::waypoint(newCoordTarget, 5);
	//	//args.targetObject = NULL;
	//	return true;

	//}

	//bool Dock( ActionArgs & args, Vessel & currentVessel )
	//{
	//	double distanceToTarget = geometry::realDistance( currentVessel.getPosition(), args.targetObject->getPosition() );
	//	double angleToTarget = -geometry::angleBetweenTwoPoints( currentVessel.getPosition(),args.targetObject->getPosition() ); 
	//	double angleDifference = geometry::shortestAngleDifference( angleToTarget, currentVessel.getHeading() - std::_Pi/2 );
	//	double angleDifferenceToVessel = geometry::shortestAngleDifference( currentVessel.getHeading(), currentVessel.getHeading() - std::_Pi/2 );

	//	if( distanceToTarget < 400 )
	//	{
	//		//if( abs(angleDifferenceToVessel) < std::_Pi/10 || abs(angleDifferenceToVessel) > std::_Pi/1.5) 
	//		//{
	//		//	currentVessel.velocitySetPointAbsValue = currentVessel.velocitySetPointAbsValue + distanceToTarget;
	//		//	return true;
	//		//}
	//		if( abs(angleDifference) < std::_Pi/10 || abs(angleDifference) > std::_Pi/1.5)
	//		{
	//			if( currentVessel.currentWaypointReached() )
	//				currentVessel.velocitySetPointAbsValue = args.targetObject->getVelocity().ABS()*0.99;
	//			else
	//				currentVessel.velocitySetPointAbsValue = args.targetObject->getVelocity().ABS()*1.5;

	//			//currentVessel.shipController->
	//			return true;
	//		}
	//		//else
	//		//{
	//		//	currentVessel.velocitySetPointAbsValue *= 0.7;
	//		//}
	//	}
	//	else
	//		currentVessel.velocitySetPointAbsValue = 50;
	//	
	//	return true;
	//}

	
//}



//
//bool InterceptAggressive(ActionArgs & args, Vessel & currentVessel)
//{
//	currentVessel.setGuidanceWaypoint(primitives::waypoint(args.TargetObject->getPosition(), 2));
//
//	auto position = args.TargetObject->getPosition();
//	double distanceToTarget = geometry::realDistance(currentVessel.getPosition(), position);
//	double angleToTarget = geometry::angleBetweenTwoPoints(currentVessel.getPosition(), position);
//	double angleDifference = geometry::shortestAngleDifference(angleToTarget, currentVessel.getGlobalHeading());
//
//	if (distanceToTarget < 400)
//	{
//		if (abs(angleDifference) < std::_Pi / 10 || abs(angleDifference) > std::_Pi / 1.5)
//		{
//			currentVessel.SetAbsVelocity(50);
//			return true;
//		}
//		else
//		{
//			currentVessel.SetAbsVelocity(currentVessel.GetAbsVelocity() * 0.7);
//		}
//	}
//	else
//		currentVessel.SetAbsVelocity(50);
//
//	FireWhenTargetInSight(angleDifference, std::_Pi / 8, currentVessel);
//
//
//	return true;
//}
//
//bool InterceptNoFire(ActionArgs & args, Vessel & currentVessel)
//{
//	currentVessel.setGuidanceWaypoint(primitives::waypoint(args.TargetObject->getPosition(), 2));
//
//	auto position = args.TargetObject->getPosition();
//	double distanceToTarget = geometry::realDistance(currentVessel.getPosition(), position);
//	double angleToTarget = geometry::angleBetweenTwoPoints(currentVessel.getPosition(), position);
//	double angleDifference = geometry::shortestAngleDifference(angleToTarget, currentVessel.getGlobalHeading());
//
//	if (distanceToTarget < 400)
//	{
//		if (abs(angleDifference) < std::_Pi / 10 || abs(angleDifference) > std::_Pi / 1.5)
//		{
//			currentVessel.SetAbsVelocity(50);
//			return true;
//		}
//		else
//		{
//			currentVessel.SetAbsVelocity(currentVessel.GetAbsVelocity() * 0.7);
//		}
//	}
//	else
//		currentVessel.SetAbsVelocity(50);
//
//	return true;
//}


