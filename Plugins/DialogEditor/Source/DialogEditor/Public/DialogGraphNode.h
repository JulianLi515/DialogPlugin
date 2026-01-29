// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGEDITOR_API UDialogGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	UEdGraphPin* CreateCustomPin(EEdGraphPinDirection Direction, FName name);
};
