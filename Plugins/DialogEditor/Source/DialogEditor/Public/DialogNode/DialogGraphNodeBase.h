// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "RuntimeNode/DialogGraphNodeRuntime.h"
#include "DialogGraphNodeBase.generated.h"

class UDialogNodeInfoBase;
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
	virtual UEdGraphPin* CreateDefaultInputPin() {return nullptr;}
	virtual void CreateDefaultOutputPins() {}
	
	virtual EDialogNodeType GetNodeType() const {return EDialogNodeType::Unknown;}
	virtual void InitNodeInfo(UObject* Outer){}
	
	void SetNodeInfo(UDialogNodeInfoBase* InNodeInfo) {NodeInfo = InNodeInfo;};
	UDialogNodeInfoBase* GetNodeInfo() const {return NodeInfo;}
	template<typename T>
	T* GetNodeInfo() const
	{
		if constexpr(TIsDerivedFrom<T, UDialogNodeInfo>::Value)
		{
			return Cast<T>(NodeInfo);
		}else
		{
			return nullptr;
		}
	}
	
	
	virtual void OnPropertiesChanged(){}
protected:
	UPROPERTY()
	UDialogNodeInfoBase* NodeInfo;
};
