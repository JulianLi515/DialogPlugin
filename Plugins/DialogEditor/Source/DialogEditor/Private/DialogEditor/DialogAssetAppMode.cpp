#include "DialogEditor/DialogAssetAppMode.h"

#include "DialogAsset/DialogAssetPropertiesTabFactory.h"
#include "DialogEditor/FDialogAssetBPEditorTabFactory.h"
#include "DialogEditor/FDialogAssetEditorApp.h"

FDialogAssetAppMode::FDialogAssetAppMode(TSharedPtr<class FDialogAssetEditorApp> InEditorApp)
	:FApplicationMode(FDialogAssetEditorApp::DefaultMode), EditorApp(InEditorApp)
{
	AllowedTabs.RegisterFactory(MakeShareable(new FDialogAssetPropertiesTabFactory(InEditorApp)));
	AllowedTabs.RegisterFactory(MakeShareable(new FDialogAssetBPEditorTabFactory(InEditorApp)));
	TabLayout = FTabManager::NewLayout("DialogAssetAppMode_Layout_v1.1")
	->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split(
					FTabManager::NewStack()
						->SetSizeCoefficient(0.75f)
						->AddTab(FDialogAssetBPEditorTabFactory::TabID, ETabState::OpenedTab)
					)
				->Split(
					FTabManager::NewStack()
						->SetSizeCoefficient(0.25f)
						->AddTab(FDialogAssetPropertiesTabFactory::TabID, ETabState::OpenedTab)
					)
			)
		);
	
}

void FDialogAssetAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FDialogAssetEditorApp> App = EditorApp.Pin();
	if (App.IsValid())
	{
		App->PushTabFactories(AllowedTabs);
	}
	
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FDialogAssetAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FDialogAssetAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}

