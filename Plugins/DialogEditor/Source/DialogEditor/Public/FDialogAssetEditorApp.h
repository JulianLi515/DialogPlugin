#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FDialogAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	static const FName DefaultMode;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void InitEditor(const EToolkitMode::Type mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UObject* ObjectToEdit);
	class UDialogAsset* GetWorkingDialogAsset() const {return WorkingDialogAsset;}
	class UEdGraph* GetWorkingDialogGraph() const {return WorkingDialogGraph;}
public: //FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override {return FName("DialogAssetEditor");}
	virtual FText GetBaseToolkitName() const override {return NSLOCTEXT("DialogEditorModule", "DialogAssetEditorApp", "Dialog Asset Editor");}
	virtual FString GetWorldCentricTabPrefix() const override {return TEXT("DialogAssetEditor");}
	virtual FLinearColor GetWorldCentricTabColorScale() const override {return FLinearColor::Black;}
	virtual FString GetDocumentationLink() const override {return TEXT("Not Available");}
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnClose() override;
	void OnGraphChanged(const FEdGraphEditAction& EditAction);
	
protected:
	void UpdateWorkingAssetFromGraph();
	void UpdateEditorGraphFromWorkingAsset();
private:
	class UDialogAsset* WorkingDialogAsset = nullptr;
	class UEdGraph* WorkingDialogGraph = nullptr;
	FDelegateHandle GraphChangedDelegateHandle;
};
