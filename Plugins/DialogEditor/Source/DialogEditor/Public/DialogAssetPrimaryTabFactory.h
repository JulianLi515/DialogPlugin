#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FDialogAssetPrimaryTabFactory : public FWorkflowTabFactory
{
public:
	static const FName TabID;
	FDialogAssetPrimaryTabFactory(TSharedPtr<class FDialogAssetEditorApp> InEditorApp);
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
private:
	TWeakPtr<class FDialogAssetEditorApp> EditorApp;
};
