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
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	void SetPreSaveCallback(std::function<void()> InCallback) { PreSaveCallback = InCallback; }
	
	UPROPERTY(EditAnywhere)
	FText DialogName = FText::FromString("Enter Dialog Name Here");
	
	UPROPERTY()
	UDialogGraphRuntime* DialogGraph = nullptr;
	
	
private:
	std::function<void()> PreSaveCallback;
};
