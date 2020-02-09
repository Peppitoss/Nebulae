#pragma once
#include"BehaviorTree.h"
//#include"VesselAI.h"

//bool InterceptAggressive(ActionArgs & args, Vessel & currentVessel);
//bool InterceptNoFire(ActionArgs & args, Vessel & currentVessel);

//enum enAIModel
//{
//	NOACTION = 0,
//	//HOLDPOSITION,
//	MOVETOTARGETCOORD,
//	//MOVETOTARGETENTITY,
//	AIPROBER,
//	//TRACKTARGETENTITY,
//	//TEMPLATEFUNCTION,
//
//};

const extern int PlayerModelID;


class AIModel
{
public:

	AIModel();

	static BehaviorTree ProberAI();
	static BehaviorTree ProberAI(int targetvesselID);

	static BehaviorTree GenerateTravelToCoordTree(const primitives::waypoint& wp);

private:

	static void FireWhenTargetInSight(double angleDifferenceToTarget, double angleBound,  Vessel & currentVessel);
	static void FireWhenTargetInSight(const Coord& TargetVesselPosition, Vessel & currentVessel, double fireAtDistance );

};

//namespace AITree
//{

	//BehaviorTree ProberAI(Vessel* target, bool fireOn);

	////BehaviorTree* generateTravelToTargetIteration(DynamicEntity* target);
	//BehaviorTree GenerateTravelToCoordTree(const primitives::waypoint& wp);//use delete afterwards

	//BehaviorTree GenerateInterceptTarget(Vessel* target, bool fireOn);

	//BehaviorTree* GenerateInterceptTarget(Vessel* target, bool fire);
	//BehaviorTree* generateTrackVessel(DynamicEntity* target);

	////obsolete?
	//BehaviorTree* generateEngageTargetIteration_1(Vessel* target);
	//BehaviorTree* generateTravelToTargetTree(DynamicEntity* target);
	////BehaviorTree* generateTravelThroughWaypoints( std::vector<primitives::waypoint*> waypoints );

	//bool CrossInterceptTarget(ActionArgs & args, Vessel & currentVessel);
	//bool IntereceptAggressive(ActionArgs & args, Vessel & currentVessel);
	//bool IntereceptNoFire(ActionArgs & args, Vessel & currentVessel);
	//bool Dock(ActionArgs & args, Vessel & currentVessel);



//}