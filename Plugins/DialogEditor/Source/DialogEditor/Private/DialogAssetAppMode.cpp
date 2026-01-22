#include "DialogAssetAppMode.h"

#include "DialogAssetPrimaryTabFactory.h"
#include "FDialogAssetEditorApp.h"

FDialogAssetAppMode::FDialogAssetAppMode(TSharedPtr<class FDialogAssetEditorApp> InEditorApp)
	:FApplicationMode(FDialogAssetEditorApp::DefaultMode), EditorApp(InEditorApp)
{
	AllowedTabs.RegisterFactory(MakeShareable(new FDialogAssetPrimaryTabFactory(InEditorApp)));\
	TabLayout = FTabManager::NewLayout("DialogAssetAppMode_Layout_v1.1")
	->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(FDialogAssetPrimaryTabFactory::TabID, ETabState::OpenedTab)
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
