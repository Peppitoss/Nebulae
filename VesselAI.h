#pragma once
#include"Vessel.h"
#include"BehaviorTree.h"

//const static int maxTimedAction = 100000; // 100 seconds //action can only run a limited amount of time

enum enVesselAIStatus
{
	IDLE = 0,
	EMPLOYED,
	FINISH,
};

//class VesselAI;

//class ActionArgs;
//class Vessel;
//
//
//class BehaviorTree;
//enum enActions;


class VesselAI
{

public:

	//typedef bool (VesselAI::*ptrActionEvent)( ActionArgs &, Vessel & );

	VesselAI();
	VesselAI( Vessel* vessel );
	VesselAI( Vessel* vessel, BehaviorTree tree);

	void SetBehaviorTree(BehaviorTree tree);

	void RunAI( int millisecond );
	//ptrActionEvent actionEvent;

	bool CurrentActionCompleted();

	void setStatus( enVesselAIStatus set ) { pMode = set; };
	//void setAction( enActions , ActionArgs & );
	enVesselAIStatus getAIStatus() { return pMode; };

	Vessel* GetControlVessel() { return ctrlVessel; }

	bool isAlive();
	void disableAIAndVessel();

protected:

	BehaviorTree pBehaviourTree;

private:

	void init();
	//void initActionArgs();

	Vessel* ctrlVessel;
	
	int pLocalTimer;
	enVesselAIStatus pMode;
	//ActionArgs AIActionSet;

	//void setVesselStateVariables( const ActionArgs & );


	//action Functions for vessel
	//bool ActionMoveToCoord( ActionArgs &, Vessel & AIvessel);
	
	//bool actionTriggered;

	//void setActionTimeSpan(const ActionArgs & args);
	bool templateActionReturnValue;

	//void handleCurrentActionComplete();
	void HandleVesselAction(BehaviorTree& tree, int milliseconds );
};

//bool ActionMoveToTarget(ActionArgs & args, Vessel & AIvessel);
//bool ActionMoveHoldPosition(ActionArgs & args, Vessel & AIvessel);
//bool ActionRunTemplateFunction(ActionArgs & args, Vessel & AIvessel);
//bool ActionInterceptTarget(ActionArgs & args, Vessel & AIvessel);
//bool ActionTrackTarget(ActionArgs & args, Vessel & AIvessel);