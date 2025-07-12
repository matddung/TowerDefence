#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUDWidget.generated.h"

UCLASS()
class TOWERDEFENCE_API UGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnStartClicked();

    UFUNCTION()
    void OnBuildClicked();

    UFUNCTION()
    void OnBackClicked();

    UFUNCTION()
    void OnAttackClicked();

    UFUNCTION()
    void OnSplashClicked();

    UFUNCTION()
    void OnCCClicked();

    void SetStartButtonEnabled(bool bEnabled);

    void SetGoldAmount(int32 Amount);

    UPROPERTY(meta = (BindWidget))
    class UWidgetSwitcher* MenuSwitcher;

    UPROPERTY(meta = (BindWidget))
    class UButton* StartRoundButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* BuildTowerButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* BackButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* AttackBuildingButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SplashBuildingButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CCBuildingButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GoldTextBlock1;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GoldTextBlock2;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UTooltipWidget> TooltipWidgetClass;
};
