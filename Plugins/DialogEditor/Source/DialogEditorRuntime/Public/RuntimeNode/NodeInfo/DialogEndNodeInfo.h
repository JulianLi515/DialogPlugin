// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeNode/NodeInfo/DialogNodeInfoBase.h"
#include "DialogEndNodeInfo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDialogEndAction : uint8
{
	None,
	GrantItem,
	StartQuest
};
UCLASS()
class DIALOGEDITORRUNTIME_API UDialogEndNodeInfo : public UDialogNodeInfoBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EDialogEndAction EndAction;
};

