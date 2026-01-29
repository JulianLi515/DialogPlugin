// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogGraphNode.h"

#include "DialogEditor.h"
#include "SDialogGraphPin.h"

FText UDialogGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	// return Super::GetNodeTitle(TitleType);
	return FText::FromString("DialogNode");
}

FLinearColor UDialogGraphNode::GetNodeTitleColor() const
{
	// return Super::GetNodeTitleColor();
	return FLinearColor::Green;
}

bool UDialogGraphNode::CanUserDeleteNode() const
{
	// return Super::CanUserDeleteNode();
	return true;
}

void UDialogGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu,
	class UGraphNodeContextMenuContext* Context) const
{
	// Super::GetNodeContextMenuActions(Menu, Context);
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString("Dialog Node Actions"));
	UDialogGraphNode* MutableThis = const_cast<UDialogGraphNode*>(this);
	Section.AddMenuEntry(
		TEXT("AddPin"),
		FText::FromString("Add Pin"),
		FText::FromString("Creates a new pin"),
		FSlateIcon(FDialogEditorModule::DialogEditorStyleSetName, 
			TEXT("DialogEditor.NodeAddPinIcon")),
			FUIAction(FExecuteAction::CreateLambda(
				[MutableThis]()
					{
						MutableThis->CreatePin(
							EGPD_Output,
							TEXT("Outputs"),
							TEXT("SomeOutput"));
						MutableThis->GetGraph()->NotifyGraphChanged();
						MutableThis->GetGraph()->Modify();
					}
				)
			)	
	);
	
	Section.AddMenuEntry(
		TEXT("DeletePin"),
		FText::FromString("Delete Pin"),
		FText::FromString("Delete the last pin"),
		FSlateIcon(FDialogEditorModule::DialogEditorStyleSetName, 
			TEXT("DialogEditor.NodeDeletePinIcon")),
			FUIAction(FExecuteAction::CreateLambda(
				[MutableThis]()
					{
						UEdGraphPin* LastPin = MutableThis->Pins[MutableThis->Pins.Num() - 1];
						if (LastPin->Direction != EGPD_Input)
						{
							MutableThis->Pins.Remove(LastPin);
							MutableThis->GetGraph()->NotifyGraphChanged();
							MutableThis->GetGraph()->Modify();
						}
					}
				)
			)	
	);
	
	Section.AddMenuEntry(
		TEXT("DeleteNode"),
		FText::FromString("Delete Node"),
		FText::FromString("Delete this node"),
		FSlateIcon(FDialogEditorModule::DialogEditorStyleSetName, 
			TEXT("DialogEditor.DeleteNodeIcon")),
			FUIAction(FExecuteAction::CreateLambda(
				[MutableThis]()
					{
						MutableThis->GetGraph()->RemoveNode(MutableThis);
						MutableThis->GetGraph()->NotifyGraphChanged();
						MutableThis->GetGraph()->Modify();
					}
				)
			)	
	);
}

UEdGraphPin* UDialogGraphNode::CreateCustomPin(EEdGraphPinDirection Direction, FName name)
{
	FName category = Direction == EGPD_Input ? TEXT("Inputs") : TEXT("Outputs");
	FName subCategory = SDialogGraphPin::DialogGraphPinSubCategory;
	
	UEdGraphPin* NewPin = CreatePin(Direction, category, subCategory, name);
	NewPin->PinType.PinSubCategory = subCategory;
	return NewPin;
	
}

