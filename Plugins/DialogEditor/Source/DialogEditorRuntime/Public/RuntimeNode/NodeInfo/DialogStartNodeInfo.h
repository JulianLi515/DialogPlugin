// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeNode/NodeInfo/DialogNodeInfoBase.h"
#include "GameplayTagContainer.h"
#include "DialogStartNodeInfo.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGEDITORRUNTIME_API UDialogStartNodeInfo : public UDialogNodeInfoBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer TagContainer;
};

