#include "GameHUDWidget.h"
#include "GamePlayGameMode.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "TooltipWidget.h"

void UGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MenuSwitcher)
    {
        MenuSwitcher->SetActiveWidgetIndex(0);
    }

    if (TooltipWidgetClass)
    {
        UTooltipWidget* AttackTip = CreateWidget<UTooltipWidget>(this, TooltipWidgetClass);
        if (AttackTip)
        {
            AttackTip->SetTooltipText(FText::FromString("Basic Attack Tower\nCost: 100 Gold"));
            AttackBuildingButton->SetToolTip(AttackTip);
        }

        UTooltipWidget* SplashTip = CreateWidget<UTooltipWidget>(this, TooltipWidgetClass);
        if (SplashTip)
        {
            SplashTip->SetTooltipText(FText::FromString("Splash Attack Tower\nCost: 150 Gold"));
            SplashBuildingButton->SetToolTip(SplashTip);
        }

        UTooltipWidget* CCTip = CreateWidget<UTooltipWidget>(this, TooltipWidgetClass);
        if (CCTip)
        {
            CCTip->SetTooltipText(FText::FromString("Put on CC Tower\nCost: 120 Gold\nSlows enemies"));
            CCBuildingButton->SetToolTip(CCTip);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("TooltipWidgetClass is NULL!"));
    }

    if (StartRoundButton)
    {
        StartRoundButton->OnClicked.AddDynamic(this, &UGameHUDWidget::OnStartClicked);
    }

    if (BuildTowerButton)
    {
        BuildTowerButton->OnClicked.AddDynamic(this, &UGameHUDWidget::OnBuildClicked);
    }

    if (BackButton)
    {
        BackButton->OnClicked.AddDynamic(this, &UGameHUDWidget::OnBackClicked);
    }

    if (AttackBuildingButton)
    {
        AttackBuildingButton->OnClicked.AddDynamic(this, &UGameHUDWidget::OnAttackClicked);
    }

    if (SplashBuildingButton)
    {
        SplashBuildingButton->OnClicked.AddDynamic(this, &UGameHUDWidget::OnSplashClicked);
    }

    if (CCBuildingButton)
    {
        CCBuildingButton->OnClicked.AddDynamic(this, &UGameHUDWidget::OnCCClicked);
    }
}

void UGameHUDWidget::OnStartClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Start button clicked"));
    if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
    {
        GM->StartNextWave();
    }
}

void UGameHUDWidget::OnBuildClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Build button clicked"));
    if (MenuSwitcher)
    {
        MenuSwitcher->SetActiveWidgetIndex(1);
    }
}

void UGameHUDWidget::OnBackClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Back to main menu"));
    if (MenuSwitcher)
    {
        MenuSwitcher->SetActiveWidgetIndex(0);
    }
}

void UGameHUDWidget::OnAttackClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Build Attack button clicked"));
}

void UGameHUDWidget::OnSplashClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Build Splash button clicked"));
}

void UGameHUDWidget::OnCCClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Build CC button clicked"));
}