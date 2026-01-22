#include "DialogAssetPrimaryTabFactory.h"
#include "FDialogAssetEditorApp.h"
#include "DialogAsset.h"

#define LOCTEXT_NAMESPACE "DialogEditorModule"

const FName FDialogAssetPrimaryTabFactory::TabID("DialogAssetPrimaryTab");

FDialogAssetPrimaryTabFactory::FDialogAssetPrimaryTabFactory(TSharedPtr<class FDialogAssetEditorApp> InEditorApp)
	: FWorkflowTabFactory(TabID, InEditorApp)
{
	EditorApp = InEditorApp;
	TabLabel = FText::FromString("Primary");
	ViewMenuDescription = FText::FromString("Primary tab for Dialog Asset Editor");
	ViewMenuTooltip = FText::FromString("Primary tab for Dialog Asset Editor");
}

TSharedRef<SWidget> FDialogAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogAssetEditorApp> App = EditorApp.Pin();
	if (!App.IsValid()) return SNullWidget::NullWidget;
	
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.bHideSelectionTip = true;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bSearchInitialKeyFocus = true;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.NotifyHook = nullptr;
		DetailsViewArgs.bShowOptions = true;
		DetailsViewArgs.bShowModifiedPropertiesOption = false;
		DetailsViewArgs.bShowScrollBar = true;
	}
	
	TSharedPtr<IDetailsView> DetailsView = PropertyModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(App->GetWorkingDialogAsset());
		
	
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		];
}

FText FDialogAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DialogAssetPrimaryTabToolTip", "Primary tab for Dialog Asset Editor");
}

#undef LOCTEXT_NAMESPACE
