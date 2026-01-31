// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "DialogGraphSchema.generated.h"

/**
 * 
 */
USTRUCT()
struct FNewNodeAction :public FEdGraphSchemaAction
{
	GENERATED_BODY()
public:
	FNewNodeAction() {};
	FNewNodeAction(UClass* InClassTemplate, FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), ClassTemplate(InClassTemplate)	
	{}
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
protected:
	UClass* ClassTemplate;
};


UCLASS()
class DIALOGEDITOR_API UDialogGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
public:
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
};
