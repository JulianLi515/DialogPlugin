#include "DialogEditor/FDialogAssetBPEditorTabFactory.h"
#include "DialogEditor/FDialogAssetEditorApp.h"

const FName FDialogAssetBPEditorTabFactory::TabID("DialogAssetBPEditorTab");

FDialogAssetBPEditorTabFactory::FDialogAssetBPEditorTabFactory(TSharedPtr<class FDialogAssetEditorApp> InEditorApp)
	:FWorkflowTabFactory(TabID, InEditorApp)
{
	EditorApp = InEditorApp;
	TabLabel = FText::FromString("BP Editor");
	ViewMenuDescription = FText::FromString("BP Editor for Dialog Asset");
	ViewMenuTooltip = FText::FromString(TEXT("A blueprint editor for dialog"));
}

TSharedRef<SWidget> FDialogAssetBPEditorTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogAssetEditorApp> App = EditorApp.Pin();
	
	SGraphEditor::FGraphEditorEvents GraphEditorEvents;
	GraphEditorEvents.OnSelectionChanged.BindRaw(App.Get(), &FDialogAssetEditorApp::OnGraphSelectionChanged);
	
	TSharedPtr<SGraphEditor> GraphEditor = SNew(SGraphEditor)
			.IsEditable(true)
			.GraphEvents(GraphEditorEvents)
			.GraphToEdit(App->GetWorkingDialogGraph());
	
	App->SetWorkingGraphUI(GraphEditor);
	
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			GraphEditor.ToSharedRef()
		];
}

FText FDialogAssetBPEditorTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("A blueprint editor for dialog"));
}


