// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogNode/DialogGraphStartNode.h"

#include "DialogGraph/SDialogGraphPin.h"

FText UDialogGraphStartNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Start");
}

FLinearColor UDialogGraphStartNode::GetNodeTitleColor() const
{
	return FLinearColor::Red;
}

bool UDialogGraphStartNode::CanUserDeleteNode() const
{
	return true;
}

void UDialogGraphStartNode::GetNodeContextMenuActions(class UToolMenu* Menu,
	class UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);
}

UEdGraphPin* UDialogGraphStartNode::CreateDialogPin(EEdGraphPinDirection InDirection, FName InName)
{
	FName Category = TEXT("Outputs");
	FName SubCategory = SDialogGraphStartPin::DialogGraphPinSubCategory;
	
	UEdGraphPin* NewPin = CreatePin(InDirection, Category, SubCategory, InName);
	NewPin->PinType.PinSubCategory = SubCategory;
	return NewPin;
	
}
