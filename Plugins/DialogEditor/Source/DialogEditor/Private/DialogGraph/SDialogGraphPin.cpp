#include "DialogGraph/SDialogGraphPin.h"

const FName SDialogGraphPin::DialogGraphPinSubCategory("DialogGraphPin");

void SDialogGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

FSlateColor SDialogGraphPin::GetPinColor() const
{
	return FSlateColor(FLinearColor(0.2f, 1.f, 0.2f));
}

