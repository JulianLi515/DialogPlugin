#include "DialogAsset/DialogAssetPropertiesTabFactory.h"
#include "DialogEditor/FDialogAssetEditorApp.h"
#include "Asset/DialogAsset.h"

#define LOCTEXT_NAMESPACE "DialogEditorModule"

const FName FDialogAssetPropertiesTabFactory::TabID("DialogAssetPropertiesTab");

FDialogAssetPropertiesTabFactory::FDialogAssetPropertiesTabFactory(TSharedPtr<class FDialogAssetEditorApp> InEditorApp)
	: FWorkflowTabFactory(TabID, InEditorApp)
{
	EditorApp = InEditorApp;
	TabLabel = FText::FromString("Properties");
	ViewMenuDescription = FText::FromString("Properties tab for Dialog Asset Editor");
	ViewMenuTooltip = FText::FromString("Displays customisable properties of the Dialog Asset");
}

TSharedRef<SWidget> FDialogAssetPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
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
	
	TSharedPtr<IDetailsView> SelectedNodeDetailsView = PropertyModule.CreateDetailView(DetailsViewArgs);
	SelectedNodeDetailsView->SetObject(nullptr);
	App->SetSelectedNodeDetailsView(SelectedNodeDetailsView);
		
	
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			DetailsView.ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			SelectedNodeDetailsView.ToSharedRef()
		];
}

FText FDialogAssetPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DialogAssetPropertiesTabToolTip", "Properties tab for Dialog Asset Editor");
}

#undef LOCTEXT_NAMESPACE


