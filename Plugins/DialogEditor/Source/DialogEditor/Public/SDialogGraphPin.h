#pragma once
#include "EdGraphUtilities.h"
#include "SGraphPin.h"


class SDialogGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogGraphPin){}
	SLATE_END_ARGS()
	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);
	static const FName DialogGraphPinSubCategory;
protected:
	virtual FSlateColor GetPinColor() const override;
	
	
};

struct FDialogGraphPinFactory : public FGraphPanelPinFactory
{
public:
	virtual ~FDialogGraphPinFactory() override {}
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* Pin) const override
	{
		if (Pin->PinType.PinSubCategory == SDialogGraphPin::DialogGraphPinSubCategory)
		{
			return SNew(SDialogGraphPin, Pin);
		}	
		return nullptr;
	}
	
};
