// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogAssetAction.h"
#include "DialogAsset.h"
#include "FDialogAssetEditorApp.h"
#define LOCTEXT_NAMESPACE "DialogAssetAction"

FDialogAssetAction::FDialogAssetAction(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
	
}

FText FDialogAssetAction::GetName() const
{
	return LOCTEXT("DialogAssetName", "Dialog Asset");
}

FColor FDialogAssetAction::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FDialogAssetAction::GetSupportedClass() const
{
	return UDialogAsset::StaticClass();
}

void FDialogAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (auto Obj : InObjects)
	{
		if (UDialogAsset* DialogAsset = Cast<UDialogAsset>(Obj))
		{
			TSharedRef<FDialogAssetEditorApp> EditorApp = MakeShareable(new FDialogAssetEditorApp());
			EditorApp->InitEditor(Mode, EditWithinLevelEditor, DialogAsset);
		}
	}
	// FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}

uint32 FDialogAssetAction::GetCategories()
{
	return AssetCategory;
}

#undef LOCTEXT_NAMESPACE
