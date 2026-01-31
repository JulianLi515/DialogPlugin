// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogGraph/DialogGraphSchema.h"

#include "DialogNode/DialogGraphEndNode.h"
#include "DialogNode/DialogGraphNode.h"
#include "RuntimeNode/NodeInfo/DialogNodeInfoBase.h"
#include "RuntimeNode/NodeInfo/DialogNodeInfo.h"
#include "DialogNode/DialogGraphStartNode.h"


UEdGraphNode* FNewNodeAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                            bool bSelectNewNode)
{
	UDialogGraphNodeBase* ResultNode = NewObject<UDialogGraphNodeBase>(ParentGraph, ClassTemplate);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;
	ResultNode->InitNodeInfo(ResultNode);
	// create default input pin
	UEdGraphPin* InputPin = ResultNode->CreateDefaultInputPin();
	// create default response pi
	ResultNode->CreateDefaultOutputPins();
	
	ParentGraph->AddNode(ResultNode, true, true);
	
	if (FromPin)
	{
		ResultNode->GetSchema()->TryCreateConnection(FromPin, InputPin);
	}
	return ResultNode;
	
}

void UDialogGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> NewDialogNodeAction = MakeShareable(new FNewNodeAction(
		UDialogGraphNode::StaticClass(),
		FText::FromString("Nodes"),
		FText::FromString("New node"),
		FText::FromString("Makes a new node"),
		0));
	TSharedPtr<FNewNodeAction> NewEndNodeAction = MakeShareable(new FNewNodeAction(
		UDialogGraphEndNode::StaticClass(),
		FText::FromString("Nodes"),
		FText::FromString("New end node"),
		FText::FromString("Makes a new end node"),
		0));
	ContextMenuBuilder.AddAction(NewDialogNodeAction);
	ContextMenuBuilder.AddAction(NewEndNodeAction);
}

const FPinConnectionResponse UDialogGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!A || !B) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Invalid pin");
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Input can only connect to output");
	}
	if (A->LinkedTo.Contains(B))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Pin already connected");
	}
	if (A->Direction == EGPD_Output && B->Direction == EGPD_Input)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, "Connect to next action");
	}
	if (A->Direction == EGPD_Input && B->Direction == EGPD_Output)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, "Connect a response");
	}
	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Invalid connection");
}

void UDialogGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	Super::CreateDefaultNodesForGraph(Graph);
	UDialogGraphStartNode* StartNode = NewObject<UDialogGraphStartNode>(&Graph);
	StartNode->CreateNewGuid();
	StartNode->NodePosX = 0;
	StartNode->NodePosY = 0;
	StartNode->SetNodeInfo(NewObject<UDialogNodeInfoBase>(StartNode));
	StartNode->CreateDialogPin(EGPD_Output, TEXT("Start"));
	Graph.AddNode(StartNode, true, true);
	Graph.Modify();
}


