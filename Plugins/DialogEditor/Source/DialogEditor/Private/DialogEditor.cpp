// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogEditor.h"
#include "DialogAsset/DialogAssetAction.h"
#include "IAssetTools.h"
#include "DialogGraph/SDialogGraphPin.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "DialogEditorModule"

const FName FDialogEditorModule::DialogEditorStyleSetName("DialogEditorStyle");

void FDialogEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	// 1.  We need to register our dialog asset type and provide an AssetAction for it
	//		- The asset action will define how the asset editor is displayed when asset is opened
	IAssetTools& AssetTools = IAssetTools::Get();
	EAssetTypeCategories::Type AssetType = AssetTools.RegisterAdvancedAssetCategory(
		FName(TEXT("DialogAssets")),
		LOCTEXT("DialogAssetsCategory", "Dialog Assets")
	);
	TSharedPtr<FDialogAssetAction> DialogAssetAction = MakeShareable(new FDialogAssetAction(AssetType));
	AssetTools.RegisterAssetTypeActions(DialogAssetAction.ToSharedRef());
	
	// 2. Register Image brush needed in style set
	StyleSet = MakeShareable(new FSlateStyleSet(DialogEditorStyleSetName));
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("DialogEditor");
	FString ContentDir = Plugin->GetContentDir();
	StyleSet->SetContentRoot(ContentDir);
	
	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("DialogEditorThumbnail"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* IconBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("DialogEditorIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* AddPinIconBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("DialogEditorAddPin"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* DeletePinIconBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("DialogEditorDeletePin"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* DeleteNodeIconBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("DialogEditorDeleteNode"), TEXT(".png")), FVector2D(128.0, 128.0));
	
	
	StyleSet->Set("ClassThumbnail.DialogAsset", ThumbnailBrush);
	StyleSet->Set("ClassIcon.DialogAsset", IconBrush);
	StyleSet->Set("DialogEditor.NodeAddPinIcon", AddPinIconBrush);
	StyleSet->Set("DialogEditor.NodeDeletePinIcon", DeletePinIconBrush);
	StyleSet->Set("DialogEditor.DeleteNodeIcon", DeleteNodeIconBrush);
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	
	// 3. Register pin factory for dialog graph
	PinFactory = MakeShareable(new FDialogGraphPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FDialogEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(DialogEditorStyleSetName);
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogEditorModule, DialogEditor)
