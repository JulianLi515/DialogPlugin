// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogNode/DialogGraphEndNode.h"

#include "DialogEditor.h"
#include "DialogGraph/SDialogGraphPin.h"
#include "RuntimeNode/NodeInfo/DialogEndNodeInfo.h"

FText UDialogGraphEndNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (UDialogEndNodeInfo* DialogNodeInfo = Cast<UDialogEndNodeInfo>(NodeInfo))
	{
		if (DialogNodeInfo->EndAction != EDialogEndAction::None)
		{
			return UEnum::GetDisplayValueAsText(DialogNodeInfo->EndAction); 
		}
	}
	return FText::FromString("End");
}

void UDialogGraphEndNode::GetNodeContextMenuActions(class UToolMenu* Menu,
	class UGraphNodeContextMenuContext* Context) const
{
	// Super::GetNodeContextMenuActions(Menu, Context);
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString("Dialog Node Actions"));
	UDialogGraphEndNode* MutableThis = const_cast<UDialogGraphEndNode*>(this);
	
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

UEdGraphPin* UDialogGraphEndNode::CreateDialogPin(EEdGraphPinDirection InDirection, FName InName)
{
	FName Category = TEXT("Inputs");
	FName SubCategory = SDialogGraphEndPin::DialogGraphPinSubCategory;
	
	UEdGraphPin* NewPin = CreatePin(InDirection, Category, SubCategory, InName);
	NewPin->PinType.PinSubCategory = SubCategory;
	return NewPin;
}

void UDialogGraphEndNode::InitNodeInfo(UObject* Outer)
{
	// Super::InitNodeInfo(Outer);
	NodeInfo = NewObject<UDialogEndNodeInfo>(Outer);
}

void UDialogGraphEndNode::OnPropertiesChanged()
{
	// Super::OnPropertiesChanged();
	Modify();
}

UEdGraphPin* UDialogGraphEndNode::CreateDefaultInputPin()
{
	return CreateDialogPin(EGPD_Input, TEXT("In"));
}

