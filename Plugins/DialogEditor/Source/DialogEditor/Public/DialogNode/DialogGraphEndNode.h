// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogGraphNodeBase.h"
#include "DialogGraphEndNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGEDITOR_API UDialogGraphEndNode : public UDialogGraphNodeBase
{
	GENERATED_BODY()
public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override {return FLinearColor::Blue;}
	virtual bool CanUserDeleteNode() const override {return true;}
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	
	// DialogGraphNodeBase
	virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection InDirection, FName InName) override;
	virtual void InitNodeInfo(UObject* Outer) override;
	virtual void OnPropertiesChanged() override;
	virtual EDialogNodeType GetNodeType() const override {return EDialogNodeType::End;}
	virtual UEdGraphPin* CreateDefaultInputPin() override;
};
