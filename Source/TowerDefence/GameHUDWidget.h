// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUDWidget.generated.h"

/**
 * 
 */
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

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UTooltipWidget> TooltipWidgetClass;
};
