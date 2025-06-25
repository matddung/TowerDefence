#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWidget.generated.h"

UCLASS()
class TOWERDEFENCE_API UTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void SetTooltipText(const FText& InText);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TooltipTextBlock;
};
