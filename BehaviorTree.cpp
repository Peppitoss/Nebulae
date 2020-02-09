#include"BehaviorTree.h"
#include"Vessel.h"

BehaviorTree::BehaviorTree() : BehaviorTree(enNodeType::EMPTYNODE)
{
	NodesCount = 1;
}

BehaviorTree::BehaviorTree(enNodeType node)
{
	BehaviorNode rootNode = BehaviorNode(node);
	pCurrentNode = rootNode;
	switch (node)
	{
	case(enNodeType::SELECTOR):
		break;
	case(enNodeType::INVERTER):
		break;
	case(enNodeType::SEQUENCE):
		break;
	case(enNodeType::ACTION):
		break;
	case(enNodeType::EMPTYNODE):
		break;
	default:
		break;
	}
	this->RootNode = node;

	pIsFinished = false;
	loopingIterations = 1;
	NodesCount = 1;
	//currentAction = enActions::NOACTION;
	//pTreeIterationFinish = false;

}

BehaviorTree::BehaviorTree(const BehaviorTree& other)
{
	this->pCurrentNode = other.pCurrentNode;
	this->RootNode = other.RootNode;
	this->NodesCount = other.NodesCount;
	this->pIsFinished = other.pIsFinished;
	this->loopingIterations = other.loopingIterations;
	this->VesselInitialization = other.VesselInitialization;

	this->EstablishProperParentsOfChildren();
}
BehaviorTree::BehaviorTree(const BehaviorTree&& other)
{
	this->pCurrentNode = std::move(other.pCurrentNode);
	this->RootNode = std::move(other.RootNode);
	this->NodesCount = std::move(other.NodesCount);
	this->pIsFinished = std::move(other.pIsFinished);
	this->loopingIterations = std::move(other.loopingIterations);
	this->VesselInitialization = std::move(other.VesselInitialization);

	this->EstablishProperParentsOfChildren();
}

BehaviorTree& BehaviorTree::operator=(const BehaviorTree& other)
{
	this->pCurrentNode = std::move(other.pCurrentNode);
	this->RootNode = std::move(other.RootNode);
	this->NodesCount = std::move(other.NodesCount);
	this->pIsFinished = std::move(other.pIsFinished);
	this->loopingIterations = std::move(other.loopingIterations);
	this->VesselInitialization = std::move(other.VesselInitialization);

	this->EstablishProperParentsOfChildren();
	return *this;
}
BehaviorTree& BehaviorTree::operator=(const BehaviorTree&& other)
{
	this->pCurrentNode = other.pCurrentNode;
	this->RootNode = other.RootNode;
	this->NodesCount = other.NodesCount;
	this->pIsFinished = other.pIsFinished;
	this->loopingIterations = other.loopingIterations;
	this->VesselInitialization = other.VesselInitialization;

	this->EstablishProperParentsOfChildren();
	return *this;
}

void BehaviorTree::EstablishProperParentsOfChildren()
{
	auto nodes = this->Nodes();
	for (int i = 0; i < nodes.size(); i++)
	{
		std::vector<BehaviorNode>& childrenNodes = nodes[i]->ChildrenNodes();
		for (int j = 0; j < childrenNodes.size(); j++)
			childrenNodes[j].SetParent(nodes[i]);
	}
}

bool BehaviorTree::operator==(const BehaviorTree& other) const
{
	return this->pCurrentNode == other.pCurrentNode &&
		this->RootNode == other.RootNode &&
		this->pIsFinished == other.pIsFinished &&
		this->loopingIterations == other.loopingIterations &&
		this->NodesCount == other.NodesCount;
}

bool BehaviorTree::IsEmpty()
{
	if (this->NodesCount == 1)
		return this->RootNode.Type() == enNodeType::EMPTYNODE;
	return false;
}

void BehaviorTree::setTreeLoopIterations( int set )
{
	//double loops = set;
	////if( loops > 0 )
	////{
	//	loops = ;
		loopingIterations = primitives::limitDouble(set, 10);
	//}
}

std::vector<BehaviorNode*> BehaviorTree::Nodes()
{
	std::vector<BehaviorNode*> result = std::vector<BehaviorNode*>();
	result.push_back(&this->RootNode);

	std::vector<BehaviorNode>& rootChildren = this->RootNode.ChildrenNodes();

	for (int j = 0; j < rootChildren.size(); j++)
		result.push_back(&rootChildren[j]);

	for (int i = 0; i < result.size(); i++)
	{
		std::vector<BehaviorNode>& newChildren = result[i]->ChildrenNodes();
		for (int j = 0; j < newChildren.size(); j++)
			result.push_back(&newChildren[j]);
	}
	return result;
}

void BehaviorTree::AddNode(BehaviorNode parentNode, BehaviorNode node)
{

	std::vector<BehaviorNode*> children = std::vector<BehaviorNode*>();
	children.push_back(&this->RootNode);

	std::vector<BehaviorNode>& rootChildren = this->RootNode.ChildrenNodes();
	
	for (int k = 0; k < rootChildren.size(); k++)
		children.push_back(&rootChildren[k]);

	for (int i = 0; i < children.size(); i++)
	{
		if (parentNode == *children[i])
		{
			children[i]->AddChild(node);
			this->NodesCount++;
			return;
		}
		else
		{
			std::vector<BehaviorNode>& newChildren = (*children[i]).ChildrenNodes();
			for (int j = 0; j < newChildren.size(); j++)
				children.push_back(&newChildren[j]);
		}

	}
}

void BehaviorTree::RunTreeFromStart()
{
	RunNode(this->RootNode);
}

void BehaviorTree::RunNode(BehaviorNode& node)
{
	if (this->pCurrentNode == node)
	{
		ContinueTreeRun();
	}
	else
	{

		this->pCurrentNode = node;
		switch (node.Type())
		{
		case enNodeType::ACTION:
			//runActionNode();
			break;
		case enNodeType::SELECTOR:
			RunNode(this->pCurrentNode.GetRandomChild()); //should select a random child
			break;
		case enNodeType::SEQUENCE:
			RunNode(this->pCurrentNode.GetChild(0));
			break;
			//case enNodeType::INVERTER:
			//	RunChild();
			//	break;
			//case enNodeType::EMPTY:
			//	RunChild();
			//default:
			//{
			//	//should not get here!
			//	//this->getRootTree().SetCurrentNode(nullptr);
			//}
		}
	}
}


void BehaviorTree::RunChildrenNode(BehaviorNode& node)
{
	if (node.IsNodeFinalChild())
	{
		RunNode(*node.GetParent());
	}
	else
	{
		this->RunNode(node.GetParent()->GetChild(node.NodeIndex() + 1));
	}
}

//void BehaviorTree::RunSelectorNode(BehaviorNode& node)
//{
//	RunChildrenNode(node);
//}
////
//void BehaviorTree::RunSelectorNode(bool lastResult)
//{
//	if (lastResult)
//		this->getParent()->runParent(true);
//	else if (isIteratorEnd())
//		this->getParent()->runParent(false);
//	else
//		runSelectorNode();
//}
//
//void BehaviorTree::RunSequenceNode()
//{
//	RunChildrenNode();
//}
//
//void BehaviorNode::runSequenceNode(bool lastResult)
//{
//	if (lastResult)
//	{
//		if (isIteratorEnd())
//			this->getParent()->runParent(true);
//		else
//			runSequenceNode();
//	}
//	else
//		this->getParent()->runParent(false);
//}


void BehaviorTree::TreeFinish()
{
	if (loopingIterations < 0)
	{
		this->RunTreeFromStart();
	}
	else if( loopingIterations > 0)
	{
		loopingIterations--;
		this->RunTreeFromStart();
	}
	else if(loopingIterations == 0)
	{
		pIsFinished = true;
		/*this->currentAction = enActions::NOACTION;*/
	}
}

void BehaviorTree::ContinueTreeRun()
{
	if (pIsFinished == true)
		return;

	auto parent = this->pCurrentNode.GetParent();
	if (parent == nullptr)
	{
		TreeFinish();
		return;
	}

	switch (parent->Type())
	{
	case enNodeType::SELECTOR:
		while (parent->GetParent() != nullptr && parent->GetParent()->Type() == enNodeType::SELECTOR)
			parent = parent->GetParent();
		if (parent == nullptr)
			TreeFinish();
		else
			RunNode(*parent);
		break;
	case enNodeType::SEQUENCE:
		RunChildrenNode(this->pCurrentNode);
		break;
	case enNodeType::INVERTER:
		//RunChild();
		break;
	case enNodeType::EMPTYNODE:
		break;
		//RunChild();
	}
}

//BehaviorNode::BehaviorNode( enNodeType type )
//{
//	pRootTree = NULL;
//	enType = type;
//	Init();
//}

const BehaviorNode NullNode = BehaviorNode(enNodeType::EMPTYNODE);


BehaviorNode::BehaviorNode()
{
	pNodeIndex = 0;
}

BehaviorNode::BehaviorNode( enNodeType type )
{
	//pRootTree = root;
	enType = type;
	pNodeIndex = 0;
	//enAction = enActions::NOACTION;
	Init();
}

BehaviorNode::BehaviorNode(const BehaviorNode& other)
{
	this->pNodeIndex = other.pNodeIndex;
	this->ActionData = other.ActionData;
	this->enType = other.enType;
	this->parent = other.parent;
	this->children = other.children;
}

BehaviorNode::BehaviorNode(const BehaviorNode&& other)
{
	this->pNodeIndex = std::move(other.pNodeIndex);
	this->ActionData = std::move(other.ActionData);
	this->enType = std::move(other.enType);
	this->parent = std::move(other.parent);
	this->children = std::move(other.children);
}

BehaviorNode& BehaviorNode::operator=(const BehaviorNode& other)
{
	this->pNodeIndex = other.pNodeIndex;
	this->ActionData = other.ActionData;
	this->enType = other.enType;
	this->parent = other.parent;
	this->children = other.children;
	return *this;
}
BehaviorNode& BehaviorNode::operator=(const BehaviorNode&& other)
{
	this->pNodeIndex = std::move(other.pNodeIndex);
	this->ActionData = std::move(other.ActionData);
	this->enType = std::move(other.enType);
	this->parent = std::move(other.parent);
	this->children = std::move(other.children);
	return *this;
}

bool BehaviorNode::operator==(const BehaviorNode& other) const
{
	return
		this->pNodeIndex == other.pNodeIndex &&
		this->ActionData == other.ActionData &&
		this->enType == other.enType &&
		this->parent == other.parent &&
		this->CompareChildren(other);
}

bool BehaviorNode::operator!=(const BehaviorNode& other) const
{
	return !(*this == other);
}

bool BehaviorNode::CompareChildren(const BehaviorNode& other) const
{
	if (this->children.size() == other.children.size())
	{
		for (int i = 0; i < this->children.size(); i++)
			if (this->children[i] != other.children[i])
				return false;
		return true;
	}
	else
		return false;
}

//BehaviorNode::BehaviorNode( enActions type , BehaviorTree* root)
//{
//	pRootTree = root;
//	enType = enNodeType::ACTION;
//	enAction = type;
//	init();
//}

void BehaviorNode::Init()
{
	parent = nullptr;
	/*previousIndex = 0;*/
	ActionData = ActionArgs();
	//aArgs = new ActionArgs();
}

//BehaviorNode* BehaviorNode::NewChild( enNodeType type )
//{
//	return new BehaviorNode( type, this->getRootTree() );
//}

//BehaviorNode* BehaviorNode::newChild( enActions action )
//{
//	return new BehaviorNode( action, this->getRootTree() );
//}
//
//void BehaviorNode::initActionArgs()
//{
//	//this->Data->standardInit();
//	//this->Data->TreeRef = this->getRootTree();
//}

//void BehaviorNode::setActionArgs( const ActionArgs & set )
//{
//	this->aArgs->active = true;
//	this->aArgs->targetCoord = set.targetCoord;
//	this->aArgs->treeRef = this->getRootTree();
//	this->aArgs->timedAction = set.timedAction;
//	this->aArgs->ActionTimeSpan = set.ActionTimeSpan;
//	this->aArgs->targetRadiusOfIntercept = set.targetRadiusOfIntercept;
//	this->aArgs->templateActionFunction = set.templateActionFunction;
//
//}



void BehaviorNode::AddChild( BehaviorNode& newChild )
{

	switch( this->enType )
	{
	case enNodeType::ACTION :
			break;
	case enNodeType::SELECTOR :
		newChild.SetParent (this);
		newChild.pNodeIndex = this->children.size();
		children.push_back( newChild );
		break;
	case enNodeType::SEQUENCE :
		newChild.SetParent( this );
		newChild.pNodeIndex = this->children.size();
		children.push_back( newChild );
		break;
	default :
		break;

	}
}

bool BehaviorNode::IsNodeFinalChild()
{
	return this->pNodeIndex == (this->GetParent()->children.size() - 1);
}

//
//bool BehaviorNode::isIteratorEnd()
//{
//	return ( this->previousIndex >= children.size() );
//}
//
//void BehaviorNode::RunChild()
//{
//	switch( this->enType )
//	{
//	case enNodeType::ACTION :
//		this->getRootTree().SetCurrentNode( this );
//		//runActionNode();
//		break;
//	case enNodeType::SELECTOR :
//		this->getRootTree().SetCurrentNode( this );
//		runSelectorNode();
//		break;
//	case enNodeType::SEQUENCE :
//		this->getRootTree().SetCurrentNode( this );
//		runSequenceNode();
//		break;
//	case enNodeType::INVERTER :
//		this->getRootTree().SetCurrentNode( this );
//		runChild();
//		break;
//	default :
//		{
//			//should not get here!
//			this->getRootTree().SetCurrentNode( nullptr );
//		}
//
//	}
//}

bool BehaviorNode::RunFunction(Vessel& de, int stepTime)
{
	ActionArgs& data = this->ActionData;
	if (data.TimedAction)
	{
		data.ActionTimeSpan -= stepTime;
		if (data.ActionTimeSpan <= 0)
			return true;
	}
	if (data.Function == false)
		return true;

	return data.Function(this->ActionData, de);
}
//
//void BehaviorNode::RunParent(bool lastResult )
//{
//	
//	if(isRoot())
//	{
//		if( isIteratorEnd() )
//			this->previousIndex = 0;
//		this->getRootTree().treeFinish();
//		return;
//	}
//
//	if( isIteratorEnd() )
//	{
//		this->previousIndex = 0;
//		this->parent->runParent( lastResult );
//		return;
//	}
//
//	switch( this->enType )
//	{
//	case enNodeType::SELECTOR :
//		if( lastResult )
//		{
//			this->parent->runParent( lastResult );
//		}
//		else
//		{
//			this->getRootTree().SetCurrentNode( this );
//			runSelectorNode();
//		}
//		break;
//	case enNodeType::SEQUENCE :
//		if( !lastResult )
//		{
//			this->parent->runParent( lastResult );
//		}
//		else
//		{
//			this->getRootTree().SetCurrentNode( this );
//			runSequenceNode();
//		}
//		break;
//	case enNodeType::INVERTER :
//		this->parent->runParent( !lastResult );
//		break;
//	default :
//		{
//			//should not get here!
//			this->getRootTree().SetCurrentNode( NULL );
//		}
//
//	}
//}


//
//void BehaviorNode::runChildrenNode()
//{
//	if( !isIteratorEnd() )
//	{
//		children[previousIndex]->runChild();
//		this->previousIndex++;
//	}
//	else
//	{   
//		previousIndex = 0;
//		this->runParent(true);
//	}
//}
//
//void BehaviorNode::runSelectorNode()
//{
//	runChildrenNode();
//}
//
//void BehaviorNode::runSelectorNode(bool lastResult)
//{
//	if( lastResult )
//		this->getParent()->runParent(true);
//	else if( isIteratorEnd() )
//		this->getParent()->runParent(false);
//	else
//		runSelectorNode();
//}
//
//void BehaviorNode::runSequenceNode()
//{
//	runChildrenNode();
//}
//
//void BehaviorNode::runSequenceNode(bool lastResult)
//{
//	if( lastResult )
//	{
//		if( isIteratorEnd() )
//			this->getParent()->runParent(true);
//		else
//			runSequenceNode();
//	}
//	else
//		this->getParent()->runParent(false);
//}

//bool BehaviorNode::RunActionNode()
//{
//	//if(this->Function)
//	//	return Function(this->pa)
//	//if( this->getRootTree()->getAppliedVessel()->getAIStatus() == enVesselAIStatus::IDLE )
//	//{
//		//this->getRootTree()->setCurrentAction( this->enAction );
//		//this->getRootTree()->setCurrentActionArgs( this->aArgs );
//		//this->getRootTree()->getAppliedVessel()->setAction( this->enAction, *this->aArgs );
//	//}
//}

//BehaviorNode::~BehaviorNode()
//{
//	//delete this->getActionArg()->targetCoord;
//
//	//for( int i = 0; i< children.size(); i++ )
//	//	this->children[i]->children.clear();
//	//this->children.clear();
//
//	//delete Data;
//
//}

ActionArgs::ActionArgs()
{
	StandardInit();
};

void ActionArgs::StandardInit()
{
	this->Active = false;
	this->ActionTimeSpan = 0;
	this->TimedAction = false;
	this->TargetObject = nullptr;
	this->TargetVesselID = -1;	//-1 is player
}

bool ActionArgs::operator==(const ActionArgs& other) const
{
	return
		this->Active == other.Active &&
		this->ActionTimeSpan == other.ActionTimeSpan &&
		this->TimedAction == other.TimedAction &&
		this->TargetObject == other.TargetObject &&
		this->TargetVesselID == other.TargetVesselID;
		//this->Function == other.Function;
}