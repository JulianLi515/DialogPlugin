// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogPluginGameMode.h"
#include "DialogPluginCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADialogPluginGameMode::ADialogPluginGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
