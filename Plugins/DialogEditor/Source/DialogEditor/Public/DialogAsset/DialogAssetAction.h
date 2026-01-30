// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * 
 */
class DIALOGEDITOR_API FDialogAssetAction: public FAssetTypeActions_Base
{
public:
	FDialogAssetAction(EAssetTypeCategories::Type InAssetCategory);
// FAssetTypeActions_Base Interface	
public:
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;

	
private:
	EAssetTypeCategories::Type AssetCategory;
};
