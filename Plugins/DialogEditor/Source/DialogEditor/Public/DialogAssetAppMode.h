#pragma once
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"


class FDialogAssetAppMode : public FApplicationMode
{
public:
	FDialogAssetAppMode(TSharedPtr<class FDialogAssetEditorApp> InEditorApp);
	
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;
	
private:
	TWeakPtr<class FDialogAssetEditorApp> EditorApp;
	FWorkflowAllowedTabSet AllowedTabs;
	
};
