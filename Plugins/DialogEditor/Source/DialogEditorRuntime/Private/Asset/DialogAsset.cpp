// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset/DialogAsset.h"

#include "UObject/ObjectSaveContext.h"

void UDialogAsset::PreSave(FObjectPreSaveContext SaveContext)
{
	if (PreSaveCallback)
	{
		PreSaveCallback();
	}
	Super::PreSave(SaveContext);
}

