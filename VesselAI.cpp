#include "VesselAI.h"


VesselAI::VesselAI()
{
	ctrlVessel = NULL;
	init();
	SetBehaviorTree(BehaviorTree(enNodeType::SELECTOR));
}

VesselAI::VesselAI(Vessel* vessel)
{
	ctrlVessel = vessel;
	init();
}

VesselAI::VesselAI( Vessel* vessel, BehaviorTree tree)
{
	ctrlVessel = vessel;
	init();
	SetBehaviorTree(tree);
}

void VesselAI::init()
{
	pMode = enVesselAIStatus::EMPLOYED;
	//actionEvent = NULL;
	//LastTreeReference = NULL;
	pLocalTimer = 0;

}

//void VesselAI::initActionArgs()
//{	
//	//AIActionSet.standardInit();
//}

void VesselAI::SetBehaviorTree(BehaviorTree tree)
{
	this->pBehaviourTree = tree;
	if(this->pBehaviourTree.VesselInitialization)
		this->pBehaviourTree.VesselInitialization(*this->ctrlVessel);
	//this->pBehaviourTree.ContinueTreeRun();
}

//void VesselAI::RunBehaviorTree()
//{
//	this->BehaviourTree.runTree();
//}

bool VesselAI::isAlive()
{
	return !this->ctrlVessel->isDestroyed();
}

void VesselAI::disableAIAndVessel()
{
	/*this->ctrlVessel->generateSingleLink(Coord(-25,0),"Vessel Scrap");*/
	this->ctrlVessel->SetDisableDrive();
	this->ctrlVessel->DeFire();
	//delete this; //safe delete??

}
//
//void VesselAI::setAction( enActions action, ActionArgs & args )
//{
//	//if( args.active == false )
//	//{
//	//	this->setStatus ( enVesselAIStatus::FINISH );
//	//	return true;
//	//}
//
//	setVesselStateVariables( args );
//
//	switch( action )
//	{
//	case enActions::NOACTION :
//		this->setStatus ( enVesselAIStatus::FINISH );
//		initActionArgs();
//		//setAction( enActions::NOACTION , AIActionSet );
//		pLocalTimer = 0;
//		break;
//	case enActions::MOVETOTARGETCOORD :
//		actionEvent = &VesselAI::ActionMoveToCoord;
//		this->setStatus( enVesselAIStatus::EMPLOYED );
//		break;
//	case enActions::MOVETOTARGETENTITY :
//		actionEvent = &VesselAI::ActionMoveToTarget;
//		this->setStatus( enVesselAIStatus::EMPLOYED );
//		break;
//	case enActions::INTERCEPTTARGETENTITY :
//		actionEvent = &VesselAI::ActionInterceptTarget;
//		this->setStatus( enVesselAIStatus::EMPLOYED );
//		break;
//	case enActions::HOLDPOSITION :
//   		actionEvent = &VesselAI::ActionMoveHoldPosition;
//		this->setStatus( enVesselAIStatus::EMPLOYED );
//		break;
//	case enActions::TRACKTARGETENTITY:
//		actionEvent = &VesselAI::ActionTrackTarget;
//		this->setStatus( enVesselAIStatus::EMPLOYED );
//	//case enActions::TEMPLATEFUNCTION :
//	//	actionEvent = &VesselAI::ActionRunTemplateFunction;
//	//	this->setStatus( enVesselAIStatus::EMPLOYED );
//		break;
//	default:
//		this->setStatus ( enVesselAIStatus::IDLE );
//	}
//
//}

//void VesselAI::setVesselStateVariables( const ActionArgs & set )
//{
//	AIActionSet.Active = set.Active;
//	AIActionSet.TargetObject = set.targetObject;
//	AIActionSet.targetRadiusOfIntercept = set.targetRadiusOfIntercept;
//	AIActionSet.templateActionFunction = set.templateActionFunction;
//	this->setActionTimeSpan(set);
//}

void VesselAI::RunAI( int milliseconds )
{
	//if( this->pMode == enVesselAIStatus::EMPLOYED && this->currentActionCompleted() )
	//{
	//	this->pMode = enVesselAIStatus::IDLE;
	//	this->LastTreeReference->continueTreeRun(true);
	//	this->pLocalTimer = 0;
	//}
	//else
	HandleVesselAction(pBehaviourTree, milliseconds );

}

void VesselAI::HandleVesselAction(BehaviorTree& tree, int milliseconds )
{
	bool complete = false;
	switch( pMode )
	{
		case enVesselAIStatus::EMPLOYED :
			pLocalTimer += milliseconds;
			complete = tree.GetCurrentNode().RunFunction(*this->ctrlVessel, milliseconds);
			if (complete)
			{
				if (this->pBehaviourTree.IsFinished() == false)
					this->pBehaviourTree.ContinueTreeRun();
				else
					this->pMode = enVesselAIStatus::FINISH;
				//this->pMode = enVesselAIStatus::IDLE;
				this->pLocalTimer = 0;
			}
			break;
		//case enVesselAIStatus::IDLE :
		//	
		//	break;
		case enVesselAIStatus::FINISH :
			break;
		default:
			return; 		//no reason to get here!
	}
}

bool VesselAI::CurrentActionCompleted()
{

	auto node = this->pBehaviourTree.GetCurrentNode();

	if(node.GetActionArg().Active == false)
		return true;

	return false;
	////if( this->AIActionSet.templateActionFunction != NULL && templateActionReturnValue == false )
	////	return false;

	//if( this->AIActionSet.timedAction && pLocalTimer < this->AIActionSet.ActionTimeSpan )
	//	return false;

	////check position condition
	//if( currentActionType == enActions::MOVETOTARGETCOORD &&  this->AIActionSet.targetCoord != NULL && !this->AIActionSet.targetCoord->positionWithinWaypoint( this->ctrlVessel->getPosition() ) )
	//	return false;

	////check target position condition
 //	if( currentActionType == enActions::MOVETOTARGETENTITY && this->AIActionSet.targetObject != NULL && !primitives::positionWithinWaypoint( primitives::waypoint( this->AIActionSet.targetObject->getPosition(), this->AIActionSet.targetRadiusOfIntercept ) ,  this->ctrlVessel->getPosition() ) )
	//	return false;

	//if( currentActionType == enActions::INTERCEPTTARGETENTITY || currentActionType == enActions::TRACKTARGETENTITY )
	//	return false;


	//return true;
}

//bool VesselAI::ActionMoveToCoord( ActionArgs & args, Vessel & )
//{
//	this->ctrlVessel->SetDrive( EnumEntityDrive::DynamicPosition);
//	this->ctrlVessel->setGuidanceWaypoint( primitives::waypoint( *args.TargetCoord ) );
//	//this->setActionTimeSpan(args);
//	return true; //really need a return value?
//}
//
//bool ActionMoveToTarget( ActionArgs & args, Vessel & vessel)
//{
//	vessel.SetDrive( EnumEntityDrive::DynamicPosition );
//	vessel.setGuidanceWaypoint( primitives::waypoint( args.TargetObject->getPosition(), 100 ) );
//	//this->ctrlVessel->changeGuidanceCurrentWaypoint( primitives::waypoint( args.targetObject->getPosition(), 2 ) );
//	//this->setActionTimeSpan(args);
//	return true; //really need a return value?
//
//}
//
//bool ActionInterceptTarget( ActionArgs & args, Vessel & vessel)
//{
//
//	//this->ctrlVessel->changeGuidanceCurrentWaypoint( primitives::waypoint( args.targetObject->getPosition(), 2 ) );
//
//	return true; //really need a return value?
//
//}
//
//bool ActionTrackTarget( ActionArgs & args, Vessel & vessel)
//{
//	if( vessel.GetUnitState() != enumUnitState::DOCKED )
//		vessel.SetDrive( EnumEntityDrive::DynamicTargetTracking );
//	vessel.setTargetState( primitives::TargetState(args.TargetObject->getPosition(), args.TargetObject->getVelocity(), args.TargetObject->getGlobalHeading() ));
//	vessel.setGuidanceWaypoint( primitives::waypoint( args.TargetObject->getPosition(), args.TargetObject->getSize()/2 ) );
//	//this->ctrlVessel->changeGuidanceCurrentWaypoint( primitives::waypoint( args.targetObject->getPosition(), 2 ) );
//
//	return true; //really need a return value?
//
//}
//
//bool ActionMoveHoldPosition( ActionArgs & args, Vessel & vessel)
//{
//	vessel.SetDrive( EnumEntityDrive::DynamicPosition );
//	vessel.setGuidanceWaypoint( primitives::waypoint( vessel.getPosition(),10 ) );
//	//this->setActionTimeSpan(args);
//	return true; //really need a return value?
//}

//bool VesselAI::ActionRunTemplateFunction( ActionArgs & args, Vessel & )
//{
//	return args.templateActionFunction(args, *this->ctrlVessel);
//}
//
//void VesselAI::setActionTimeSpan(const ActionArgs & args)
//{
//	if( args.TimedAction == true && args.ActionTimeSpan > 0 )
//	{
//		AIActionSet.TimedAction = true;
//		AIActionSet.ActionTimeSpan = args.ActionTimeSpan;
//	}
//	else if (args.TimedAction == false )
//	{
//		AIActionSet.timedAction = false;
//		AIActionSet.ActionTimeSpan = 0;
//	}
//}
//
