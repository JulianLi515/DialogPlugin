// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogGraphSchema.h"

#include "DialogGraphNode.h"


UEdGraphNode* FNewNodeAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                            bool bSelectNewNode)
{
	UDialogGraphNode* ResultNode = NewObject<UDialogGraphNode>(ParentGraph);
	ResultNode->CreateNewGuid();
	ResultNode->NodePosX = Location.X;
	ResultNode->NodePosY = Location.Y;
	UEdGraphPin* InputPin = ResultNode->CreateCustomPin(EGPD_Input, TEXT("Some Input"));
	ResultNode->CreateCustomPin(EGPD_Output, TEXT("Some Output 1"));
	ResultNode->CreateCustomPin(EGPD_Output, TEXT("Some Output 2"));
	if (FromPin)
	{
		ResultNode->GetSchema()->TryCreateConnection(FromPin, InputPin);
	}
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
	ContextMenuBuilder.AddAction(NewNodeAction);
}

const FPinConnectionResponse UDialogGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!A || !B) return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Invalid pin");
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Input can only connect to output");
	}
	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, "");
}
