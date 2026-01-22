// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogAssetFactory.h"

#include "DialogAsset.h"

UDialogAssetFactory::UDialogAssetFactory(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SupportedClass = UDialogAsset::StaticClass();
}

UObject* UDialogAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                               UObject* Context, FFeedbackContext* Warn)
{
	UDialogAsset* NewAsset = NewObject<UDialogAsset>(InParent, InName, Flags);
	return NewAsset;
}

bool UDialogAssetFactory::CanCreateNew() const
{
	return true;
}
