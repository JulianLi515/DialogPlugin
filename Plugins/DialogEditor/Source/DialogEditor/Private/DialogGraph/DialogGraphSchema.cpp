// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogGraph/DialogGraphSchema.h"

#include "DialogNode/DialogGraphNode.h"
#include "RuntimeNode/NodeInfo/DialogNodeInfoBase.h"
#include "RuntimeNode/NodeInfo/DialogNodeInfo.h"
#include "DialogNode/DialogGraphStartNode.h"


UEdGraphNode* FNewNodeAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                            bool bSelectNewNode)
{
	UDialogGraphNode* ResultNode = NewObject<UDialogGraphNode>(ParentGraph);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;
	ResultNode->SetNodeInfo(NewObject<UDialogNodeInfo>(ResultNode));
	// create default input pin
	UEdGraphPin* InputPin = ResultNode->CreateDialogPin(EGPD_Input, TEXT("In"));
	// create default response pin
	FString DefaultResponse = FString("Continue");
	ResultNode->CreateDialogPin(EGPD_Output, *DefaultResponse);
	if (UDialogNodeInfo* NodeInfo = Cast<UDialogNodeInfo>(ResultNode->GetNodeInfo()))
	{
		NodeInfo->DialogResponses.Add(FText::FromString(DefaultResponse));
	}
	ParentGraph->AddNode(ResultNode, true, true);
	
	if (FromPin)
	{
		ResultNode->GetSchema()->TryCreateConnection(FromPin, InputPin);
	}
	return ResultNode;
	
}

UEdGraphNode* FNewStartNodeAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	UDialogGraphStartNode* ResultNode = NewObject<UDialogGraphStartNode>(ParentGraph);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;
	ResultNode->SetNodeInfo(NewObject<UDialogNodeInfoBase>(ResultNode));
	ResultNode->CreateDialogPin(EGPD_Output, FName("Start"));
	ParentGraph->AddNode(ResultNode, true, true);
	
	return ResultNode;
}

void UDialogGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> NewNodeAction = MakeShareable(new FNewNodeAction(
		FText::FromString("Nodes"),
		FText::FromString("New node"),
		FText::FromString("Makes a new node"),
		0));
	TSharedPtr<FNewStartNodeAction> NewStartNodeAction = MakeShareable(new FNewStartNodeAction(
		FText::FromString("Start Node"),
		FText::FromString("New Start Node"),
		FText::FromString("Makes a new Start Node"),
		0));
	ContextMenuBuilder.AddAction(NewNodeAction);
	ContextMenuBuilder.AddAction(NewStartNodeAction);
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
	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, "");
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


