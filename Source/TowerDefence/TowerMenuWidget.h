#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerMenuWidget.generated.h"

UCLASS()
class TOWERDEFENCE_API UTowerMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void Init(class ATowerBase* InOwner);
    void UpdateInfo();

protected:
    UFUNCTION()
    void OnSellClicked();

    UFUNCTION()
    void OnUpgradeClicked();

    UPROPERTY(meta = (BindWidget))
    class UButton* SellButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* UpgradeButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CurrentLevelText;

private:
    UPROPERTY()
    ATowerBase* OwnerTower;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UTooltipWidget> TooltipWidgetClass;

    UPROPERTY()
    class UTooltipWidget* SellTooltip;

    UPROPERTY()
    class UTooltipWidget* UpgradeTooltip;
};