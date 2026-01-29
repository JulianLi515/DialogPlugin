// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogAsset.generated.h"

class UDialogGraphRuntime;
/**
 * 
 */
UCLASS()
class DIALOGEDITORRUNTIME_API UDialogAsset : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FString TestStringData = "Hello";
	
	UPROPERTY(EditAnywhere)
	int32 TestIntData = 42;
	
	UPROPERTY(EditAnywhere)
	bool TestBoolData = true;
	
	UPROPERTY()
	UDialogGraphRuntime* DialogGraph = nullptr;
};
