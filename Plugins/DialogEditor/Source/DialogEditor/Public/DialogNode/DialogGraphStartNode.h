// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogGraphNodeBase.h"
#include "DialogGraphStartNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGEDITOR_API UDialogGraphStartNode : public UDialogGraphNodeBase
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	
	virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection InDirection, FName InName) override;
	virtual void CreateDefaultOutputPins() override;
	virtual EDialogNodeType GetNodeType() const override {return EDialogNodeType::Start;}
};
