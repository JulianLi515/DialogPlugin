// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogNode/DialogGraphNodeBase.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogGraphNode.generated.h"

class UDialogNodeInfo;
/**
 * 
 */
UCLASS()
class DIALOGEDITOR_API UDialogGraphNode : public UDialogGraphNodeBase
{
	GENERATED_BODY()
public:
	// UEdGraph 
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	
	// My functions
	virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection InDirection, FName InName) override;
	void SyncPinWithResponses();



};

