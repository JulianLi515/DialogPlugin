#include "FDialogAssetEditorApp.h"

#include "DialogAsset.h"
#include "DialogAssetAppMode.h"
#include "DialogGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"

const FName FDialogAssetEditorApp::DefaultMode("DialogAssetAppMode");

void FDialogAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FDialogAssetEditorApp::InitEditor(const EToolkitMode::Type mode,
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* ObjectToEdit)
{
	if (ObjectToEdit)
	{
		WorkingDialogAsset = Cast<UDialogAsset>(ObjectToEdit);
		WorkingDialogGraph = FBlueprintEditorUtils::CreateNewGraph(
			WorkingDialogAsset,
			NAME_None,
			UEdGraph::StaticClass(),
			UDialogGraphSchema::StaticClass()
			);
	}
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(ObjectToEdit);
	InitAssetEditor(
		mode,
		InitToolkitHost,
		TEXT("DialogAssetEditor"),
		FTabManager::FLayout::NullLayout,
		true,
		true,
		ObjectsToEdit);
	
	AddApplicationMode(DefaultMode, MakeShareable(new FDialogAssetAppMode(SharedThis(this))));
	SetCurrentMode(DefaultMode);
}
