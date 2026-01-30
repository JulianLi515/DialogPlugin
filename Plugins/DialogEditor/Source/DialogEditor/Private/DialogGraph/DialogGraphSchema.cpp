// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogGraph/DialogGraphSchema.h"

#include "DialogNode/DialogGraphNode.h"
#include "DialogNodeInfo.h"


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
	ResultNode->GetNodeInfo()->DialogResponses.Add(FText::FromString(DefaultResponse));
	ParentGraph->AddNode(ResultNode, true, true);
	
	if (FromPin)
	{
		ResultNode->GetSchema()->TryCreateConnection(FromPin, InputPin);
	}
	return ResultNode;
	
}

void UDialogGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> NewNodeAction = MakeShareable(new FNewNodeAction(
		FText::FromString("Nodes"),
		FText::FromString("New node"),
		FText::FromString("Makes a new node"),
		0));
	ContextMenuBuilder.AddAction(NewNodeAction);
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

