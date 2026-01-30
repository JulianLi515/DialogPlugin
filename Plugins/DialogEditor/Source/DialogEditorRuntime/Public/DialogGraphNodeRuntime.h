// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogGraphNodeRuntime.generated.h"

class UDialogNodeInfo;
class UDialogGraphPinRuntime;
/**
 * 
 */

UENUM()
enum class EDialogNodeType
{
	Start,
	Dialog,
	End
};
UCLASS()
class DIALOGEDITORRUNTIME_API UDialogGraphNodeRuntime : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	EDialogNodeType NodeType = EDialogNodeType::Dialog;
	
	UPROPERTY()
	UDialogGraphPinRuntime* InputPin;
	
	UPROPERTY()
	TArray<UDialogGraphPinRuntime*> OutputPins;
	
	UPROPERTY()
	FVector2D Position;
	
	UPROPERTY()
	UDialogNodeInfo* NodeInfo = nullptr;
};
