// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct FDialogGraphPinFactory;

class FDialogEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	static const FName DialogEditorStyleSetName;
private:
	TSharedPtr<FSlateStyleSet> StyleSet;
	TSharedPtr<FDialogGraphPinFactory> PinFactory;
	
	
};
