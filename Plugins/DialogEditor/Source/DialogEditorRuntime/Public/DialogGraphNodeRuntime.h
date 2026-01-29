// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogGraphNodeRuntime.generated.h"

class UDialogGraphPinRuntime;
/**
 * 
 */
UCLASS()
class DIALOGEDITORRUNTIME_API UDialogGraphNodeRuntime : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<UDialogGraphPinRuntime*> InputPins;
	
	UPROPERTY()
	TArray<UDialogGraphPinRuntime*> OutputPins;
	
	UPROPERTY()
	FVector2D Position;
};
