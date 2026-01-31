// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogNode/DialogGraphNode.h"

#include "DialogEditor.h"
#include "RuntimeNode/NodeInfo/DialogNodeInfo.h"
#include "DialogGraph/SDialogGraphPin.h"

namespace
{
	void LogPinConnections(const UDialogGraphNode* Node)
	{
		if (!Node)
		{
			UE_LOG(LogTemp, Warning, TEXT("DialogGraphNode: null node"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("DialogGraphNode %s pins=%d"),
			*Node->GetName(),
			Node->Pins.Num());

		for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
		{
			const UEdGraphPin* Pin = Node->Pins[PinIndex];
			if (!Pin)
			{
				UE_LOG(LogTemp, Warning, TEXT("  Pin[%d] <null>"), PinIndex);
				continue;
			}

			FString LinkedPins;
			for (const UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				if (!LinkedPin)
				{
					if (!LinkedPins.IsEmpty())
					{
						LinkedPins += TEXT(", ");
					}
					LinkedPins += TEXT("<null>");
					continue;
				}

				const UEdGraphNode* LinkedNode = LinkedPin->GetOwningNode();
				if (!LinkedPins.IsEmpty())
				{
					LinkedPins += TEXT(", ");
				}
				LinkedPins += FString::Printf(TEXT("%s:%s:%s"),
					LinkedNode ? *LinkedNode->GetName() : TEXT("<no-node>"),
					*LinkedPin->PinName.ToString(),
					*LinkedPin->PinId.ToString(EGuidFormats::Digits));
			}

			if (LinkedPins.IsEmpty())
			{
				LinkedPins = TEXT("(none)");
			}

			const TCHAR* Direction = Pin->Direction == EGPD_Input ? TEXT("Input") : TEXT("Output");
			UE_LOG(LogTemp, Warning, TEXT("  Pin[%d] %s Id=%s Dir=%s LinkedTo=%s"),
				PinIndex,
				*Pin->PinName.ToString(),
				*Pin->PinId.ToString(EGuidFormats::Digits),
				Direction,
				*LinkedPins);
		}
	}
}

FText UDialogGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	// return Super::GetNodeTitle(TitleType);
	if (UDialogNodeInfo* DialogNodeInfo = GetNodeInfo<UDialogNodeInfo>())
	{
		if (DialogNodeInfo->Title.IsEmpty())
		{
			FString DialogText = DialogNodeInfo->DialogText.ToString();
			if (DialogText.Len()>15)
			{
				DialogText = DialogText.Left(15) + FString("...");
			}
			return FText::FromString(DialogText);
		}
		return DialogNodeInfo->Title;
	}
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

void UDialogGraphNode::GetNodeContextMenuActions(UToolMenu* Menu,
	UGraphNodeContextMenuContext* Context) const
{
	// Super::GetNodeContextMenuActions(Menu, Context);
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString("Dialog Node Actions"));
	UDialogGraphNode* MutableThis = const_cast<UDialogGraphNode*>(this);
	Section.AddMenuEntry(
		TEXT("AddResponse"),
		FText::FromString("Add Response"),
		FText::FromString("Creates a new response"),
		FSlateIcon(FDialogEditorModule::DialogEditorStyleSetName, 
			TEXT("DialogEditor.NodeAddPinIcon")),
			FUIAction(FExecuteAction::CreateLambda(
				[MutableThis]()
					{
						if (UDialogNodeInfo* NodeInfo = MutableThis->GetNodeInfo<UDialogNodeInfo>())
						{
							NodeInfo->DialogResponses.Add(FText::FromString("New Response"));
						}
						MutableThis->SyncPinWithResponses();
						MutableThis->GetGraph()->NotifyGraphChanged();
						MutableThis->GetGraph()->Modify();
					}
				)
			)	
	);
	
	Section.AddMenuEntry(
		TEXT("DeleteResponse"),
		FText::FromString("Delete Response"),
		FText::FromString("Delete the last Response"),
		FSlateIcon(FDialogEditorModule::DialogEditorStyleSetName, 
			TEXT("DialogEditor.NodeDeletePinIcon")),
			FUIAction(FExecuteAction::CreateLambda(
				[MutableThis]()
					{
						UEdGraphPin* LastPin = MutableThis->Pins[MutableThis->Pins.Num() - 1];
						if (LastPin->Direction != EGPD_Input)
						{
							if (UDialogNodeInfo* NodeInfo = MutableThis->GetNodeInfo<UDialogNodeInfo>())
							{
								if (NodeInfo->DialogResponses.Num() > 0)
								{
									NodeInfo->DialogResponses.RemoveAt(NodeInfo->DialogResponses.Num() - 1);
									MutableThis->SyncPinWithResponses();
								}
							}
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
						LogPinConnections(MutableThis);
						MutableThis->GetGraph()->RemoveNode(MutableThis);
						MutableThis->GetGraph()->NotifyGraphChanged();
						MutableThis->GetGraph()->Modify();
					}
				)
			)	
	);
}

UEdGraphPin* UDialogGraphNode::CreateDialogPin(EEdGraphPinDirection InDirection, FName InName)
{
	FName Category = InDirection == EGPD_Input ? TEXT("Inputs") : TEXT("Outputs");
	FName SubCategory = SDialogGraphPin::DialogGraphPinSubCategory;
	
	UEdGraphPin* NewPin = CreatePin(InDirection, Category, SubCategory, InName);
	NewPin->PinType.PinSubCategory = SubCategory;
	return NewPin;
	
}

void UDialogGraphNode::InitNodeInfo(UObject* Outer)
{
	// Super::InitNodeInfo(Outer);
	NodeInfo = NewObject<UDialogNodeInfo>(Outer);
}

void UDialogGraphNode::OnPropertiesChanged()
{
	// Super::OnPropertiesChanged();
	SyncPinWithResponses();
	Modify();
}

UEdGraphPin* UDialogGraphNode::CreateDefaultInputPin()
{
	return CreateDialogPin(EGPD_Input, TEXT("In"));
}

void UDialogGraphNode::CreateDefaultOutputPins()
{
	FString DefaultResponse = FString("Continue");
	CreateDialogPin(EGPD_Output, *DefaultResponse);
	if (UDialogNodeInfo* DialogNodeInfo = GetNodeInfo<UDialogNodeInfo>())
	{
		DialogNodeInfo->DialogResponses.Add(FText::FromString(DefaultResponse));
	}
}

void UDialogGraphNode::SyncPinWithResponses()
{
	if (!NodeInfo) return;
	// we will assume we only have one input pin and that is the first pin
	int NumOutPin = Pins.Num() - 1;
	UDialogNodeInfo* DialogNodeInf = Cast<UDialogNodeInfo>(NodeInfo);
	if (!DialogNodeInf) return;
	int NumResponses = DialogNodeInf->DialogResponses.Num();
	// adjust pin counts
	while (NumOutPin < NumResponses)
	{
		CreateDialogPin(EGPD_Output, TEXT("New Response"));
		++NumOutPin;
	}
	while (NumOutPin > NumResponses)
	{
		RemovePinAt(NumOutPin-1, EGPD_Output);
		--NumOutPin;
	}
	
	// sync pin names
	int index = 1;
	for (auto Text : DialogNodeInf->DialogResponses)
	{
		GetPinAt(index)->PinName = FName(Text.ToString());
		++index;
	}
	
}



