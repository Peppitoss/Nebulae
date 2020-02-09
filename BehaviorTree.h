#pragma once
#include<vector>
#include"Vessel.h"
#include<functional>
#include"Statistics.h"
//#include"VesselAI.h"

//SEE http://www.gamasutra.com/blogs/ChrisSimpson/20140717/221339/Behavior_trees_for_AI_How_they_work.php


enum enNodeType
{
	SEQUENCE = 0,
	SELECTOR,
	INVERTER,
	ACTION,
	EMPTYNODE,
};

class ActionArgs
{
public:

	ActionArgs();

	bool ActionArgs::operator==(const ActionArgs& other) const;

	void StandardInit();

	bool Active;
	Vessel* TargetObject;
	int TargetVesselID;
	bool TimedAction;
	int ActionTimeSpan;

	std::function<bool(ActionArgs&, Vessel&)> Function;

};

//const extern BehaviorNode NullNode;

class BehaviorNode
{
public:

	BehaviorNode();
	BehaviorNode(enNodeType); //, BehaviorTree& );

	BehaviorNode(const BehaviorNode&);
	BehaviorNode(const BehaviorNode&&);

	BehaviorNode& operator=(const BehaviorNode& other);
	BehaviorNode& operator=(const BehaviorNode&& other);

	bool operator==(const BehaviorNode& other) const;
	bool operator!=(const BehaviorNode& other) const;
	bool CompareChildren(const BehaviorNode& other) const;

	enNodeType Type() { return enType; };

	void AddChild(BehaviorNode& child);
	bool IsNodeFinalChild();

	BehaviorNode& GetChild(int index) { return this->children[index]; }
	BehaviorNode& GetRandomChild() { return this->children[statistics::GetIntInRange(0, this->children.size()-1)]; }

	int NodeIndex() { return this->pNodeIndex; }

	void SetParent(BehaviorNode* set) { parent = set; };
	BehaviorNode* GetParent() { return this->parent; };

	ActionArgs& GetActionArg() { return ActionData; };

	bool RunFunction(Vessel&, int stepTime);

	std::vector<BehaviorNode>& ChildrenNodes() { return children; }

private:

	void Init();
	bool isRoot() { return (parent == nullptr); };

	int pNodeIndex;
	ActionArgs ActionData;
	enNodeType enType;

	BehaviorNode* parent;
	std::vector<BehaviorNode> children;

};
class BehaviorTree
{
public:

	BehaviorTree();
	BehaviorTree(enNodeType root );

	BehaviorTree(const BehaviorTree& other);
	BehaviorTree(const BehaviorTree&& other);


	BehaviorTree& operator=(const BehaviorTree& other);
	BehaviorTree& operator=(const BehaviorTree&& other);

	bool operator==(const BehaviorTree& other) const;

	int GetNodeAmount() { return NodesCount; };
	void AddNode(BehaviorNode parentNode, BehaviorNode node);

	//BehaviorNode RootNode() { return this->RootNode; }
	bool IsEmpty();

	void ContinueTreeRun();
	void TreeFinish();

	void SetCurrentNode( BehaviorNode& set ) { pCurrentNode = set; };
	BehaviorNode& GetCurrentNode() { return this->pCurrentNode; }

	void EstablishProperParentsOfChildren();

	//BehaviorNode CopyNode(const BehaviorNode& toCopy, const BehaviorTree& newTree);
	//BehaviorNode CopyNode(const BehaviorNode&& toCopy, const BehaviorTree& newTree);
	
	//BehaviorNode& GetLastInsertedNode() { return Nodes[ Nodes.size() -1 ]; };

	void setTreeLoopIterations( int set );
	bool IsFinished() { return this->pIsFinished; }

	BehaviorNode RootNode;
	std::vector<BehaviorNode*> Nodes();
	std::function<void(Vessel&)> VesselInitialization;

private:


	BehaviorNode pCurrentNode;
	int NodesCount;

	void RunNode(BehaviorNode& node);
	void RunChildrenNode(BehaviorNode& node);
	void RunTreeFromStart();

	
	bool pIsFinished; //tree continues to run after finished
	int loopingIterations;
};


