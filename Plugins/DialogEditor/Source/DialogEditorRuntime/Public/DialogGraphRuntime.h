// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogGraphRuntime.generated.h"

class UDialogGraphNodeRuntime;
/**
 * 
 */
UCLASS()
class DIALOGEDITORRUNTIME_API UDialogGraphRuntime : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<UDialogGraphNodeRuntime*> DialogNodes;
};
