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

protected:
    UFUNCTION()
    void OnSellClicked();

    UFUNCTION()
    void OnUpgradeClicked();

    UPROPERTY(meta = (BindWidget))
    class UButton* SellButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* UpgradeButton;

private:
    UPROPERTY()
    ATowerBase* OwnerTower;
};