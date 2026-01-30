// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogGraphNodeBase.generated.h"

class UDialogNodeInfo;
/**
 * 
 */
UCLASS()
class DIALOGEDITOR_API UDialogGraphNodeBase : public UEdGraphNode
{
	GENERATED_BODY()
public:
	virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection InDirection, FName InName) {return nullptr;}
	void SetNodeInfo(UDialogNodeInfo* InNodeInfo);
	UDialogNodeInfo* GetNodeInfo() const {return NodeInfo;}
protected:
	UPROPERTY()
	UDialogNodeInfo* NodeInfo;
};
