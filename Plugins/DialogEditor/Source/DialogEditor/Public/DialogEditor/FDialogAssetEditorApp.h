#pragma once

#include "CoreMinimal.h"
#include "DialogNode/DialogGraphNode.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FDialogAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	static const FName DefaultMode;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void InitEditor(const EToolkitMode::Type mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UObject* ObjectToEdit);
	
	// call back for updating the details view tab
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& NewSelection);
	// call back for node detial changed, will trigger rerender to update node name,pin..
	void OnNodeDetailsUpdated(const FPropertyChangedEvent& PropertyChangedEvent);
	// call back for graph changed, will save the graph
	void OnWorkingAssetPreSave();
	// hook up details view delegates
	void SetSelectedNodeDetailsView(TSharedPtr<class IDetailsView> InSelectedNodeDetailsView);

	// some getter setters
	class UDialogAsset* GetWorkingDialogAsset() const {return WorkingDialogAsset;}
	class UEdGraph* GetWorkingDialogGraph() const {return WorkingDialogGraph;}
	void SetWorkingGraphUI(TSharedPtr<class SGraphEditor> InWorkingGraphEditor);

public: //FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override {return FName("DialogAssetEditor");}
	virtual FText GetBaseToolkitName() const override {return NSLOCTEXT("DialogEditorModule", "DialogAssetEditorApp", "Dialog Asset Editor");}
	virtual FString GetWorldCentricTabPrefix() const override {return TEXT("DialogAssetEditor");}
	virtual FLinearColor GetWorldCentricTabColorScale() const override {return FLinearColor::Black;}
	virtual FString GetDocumentationLink() const override {return TEXT("Not Available");}
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnClose() override;
	
	
protected:
	// save graph to asset
	void UpdateWorkingAssetFromGraph();
	// load graph from asset
	void UpdateEditorGraphFromWorkingAsset();
private:
	class UDialogAsset* WorkingDialogAsset = nullptr;
	class UEdGraph* WorkingDialogGraph = nullptr;
	class UDialogGraphNodeBase* SelectedNode = nullptr;
	FDelegateHandle GraphChangedDelegateHandle;
	
	TSharedPtr<SGraphEditor> WorkingGraphEditor = nullptr;
	TSharedPtr<IDetailsView> SelectedNodeDetailsView = nullptr;
};

