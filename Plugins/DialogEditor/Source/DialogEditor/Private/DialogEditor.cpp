// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogEditor.h"
#include "DialogAssetAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "DialogEditorModule"

void FDialogEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetTools = IAssetTools::Get();
	EAssetTypeCategories::Type AssetType = AssetTools.RegisterAdvancedAssetCategory(
		FName(TEXT("DialogAssets")),
		LOCTEXT("DialogAssetsCategory", "Dialog Assets")
	);
	TSharedPtr<FDialogAssetAction> DialogAssetAction = MakeShareable(new FDialogAssetAction(AssetType));
	AssetTools.RegisterAssetTypeActions(DialogAssetAction.ToSharedRef());
}

void FDialogEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogEditorModule, DialogEditor)