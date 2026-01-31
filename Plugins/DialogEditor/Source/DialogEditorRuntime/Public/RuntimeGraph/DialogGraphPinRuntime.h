// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogGraphPinRuntime.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGEDITORRUNTIME_API UDialogGraphPinRuntime : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName PinName;
	UPROPERTY()
	FGuid PinId;
	UPROPERTY()
	UDialogGraphPinRuntime* Connection = nullptr;
};
