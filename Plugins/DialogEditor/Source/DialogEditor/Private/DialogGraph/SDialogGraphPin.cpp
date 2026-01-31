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

void SDialogGraphStartPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

const FName SDialogGraphStartPin::DialogGraphPinSubCategory("DialogGraphStartPin");

FSlateColor SDialogGraphStartPin::GetPinColor() const
{
	return FSlateColor(FLinearColor(0.8, 0.1, 0.1));
}

const FName SDialogGraphEndPin::DialogGraphPinSubCategory("DialogGraphEndPin");

FSlateColor SDialogGraphEndPin::GetPinColor() const
{
	// return SGraphPin::GetPinColor();
	return FSlateColor(FLinearColor(0.1, 0.1, 0.8));
}

void SDialogGraphEndPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

