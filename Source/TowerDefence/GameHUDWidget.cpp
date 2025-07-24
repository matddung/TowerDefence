#include "GameHUDWidget.h"
#include "GamePlayGameMode.h"
#include "TDPlayerController.h"
#include "TowerBase.h"
#include "AttackTower.h"
#include "SplashTower.h"
#include "CCTower.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "TooltipWidget.h"
#include "Components/TextBlock.h"

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
            AttackTowerCost = 0;
            if (const AAttackTower* DefaultAttack = Cast<AAttackTower>(AAttackTower::StaticClass()->GetDefaultObject()))
            {
                AttackTowerCost = DefaultAttack->GetBuildCost();
            }

            AttackTip->SetTooltipText(FText::FromString(FString::Printf(TEXT("Basic Attack Tower\nCost: %d Gold"), AttackTowerCost)));
            AttackBuildingButton->SetToolTip(AttackTip);
        }

        UTooltipWidget* SplashTip = CreateWidget<UTooltipWidget>(this, TooltipWidgetClass);
        if (SplashTip)
        {
            SplashTowerCost = 0;
            if (const ASplashTower* DefaultSplash = Cast<ASplashTower>(ASplashTower::StaticClass()->GetDefaultObject()))
            {
                SplashTowerCost = DefaultSplash->GetBuildCost();
            }

            SplashTip->SetTooltipText(FText::FromString(FString::Printf(TEXT("Splash Attack Tower\nCost: %d Gold"), SplashTowerCost)));
            SplashBuildingButton->SetToolTip(SplashTip);
        }

        UTooltipWidget* CCTip = CreateWidget<UTooltipWidget>(this, TooltipWidgetClass);
        if (CCTip)
        {
            CCTowerCost = 0;
            if (const ACCTower* DefaultCC = Cast<ACCTower>(ACCTower::StaticClass()->GetDefaultObject()))
            {
                CCTowerCost = DefaultCC->GetBuildCost();
            }

            CCTip->SetTooltipText(FText::FromString(FString::Printf(TEXT("Put on CC Tower\nCost: %d Gold\nSlows enemies"), CCTowerCost)));
            CCBuildingButton->SetToolTip(CCTip);
        }
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

    if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
    {
        SetGoldAmount(GM->GetGold());
    }
}

void UGameHUDWidget::OnStartClicked()
{
    if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
    {
        GM->StartNextWave();
    }
}

void UGameHUDWidget::OnBuildClicked()
{
    if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
    {
        if (GM->IsWaveInProgress())
        {
            return;
        }
    }
    if (MenuSwitcher)
    {
        MenuSwitcher->SetActiveWidgetIndex(1);
    }
}

void UGameHUDWidget::OnBackClicked()
{
    if (MenuSwitcher)
    {
        MenuSwitcher->SetActiveWidgetIndex(0);
    }
}

void UGameHUDWidget::OnAttackClicked()
{
    if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
    {
        if (GM->IsWaveInProgress())
        {
            return;
        }
    }

    if (ATDPlayerController* PC = Cast<ATDPlayerController>(GetOwningPlayer()))
    {
        PC->StartPlacingAttackTower();
    }
}

void UGameHUDWidget::OnSplashClicked()
{
    if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
    {
        if (GM->IsWaveInProgress())
        {
            return;
        }
    }

    if (ATDPlayerController* PC = Cast<ATDPlayerController>(GetOwningPlayer()))
    {
        PC->StartPlacingSplashTower();
    }
}

void UGameHUDWidget::OnCCClicked()
{
    if (AGamePlayGameMode* GM = GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
    {
        if (GM->IsWaveInProgress())
        {
            return;
        }
    }

    if (ATDPlayerController* PC = Cast<ATDPlayerController>(GetOwningPlayer()))
    {
        PC->StartPlacingCCTower();
    }
}

void UGameHUDWidget::SetStartButtonEnabled(bool bEnabled)
{
    if (StartRoundButton)
    {
        StartRoundButton->SetIsEnabled(bEnabled);
    }
}

void UGameHUDWidget::SetGoldAmount(int32 Amount)
{
    CurrentGold = Amount;

    if (GoldTextBlock1 && GoldTextBlock2)
    {
        GoldTextBlock1->SetText(FText::FromString(FString::Printf(TEXT("Gold: %d"), Amount)));
        GoldTextBlock2->SetText(FText::FromString(FString::Printf(TEXT("Gold: %d"), Amount)));
    }

    UpdateBuildButtonsEnabled();
}

void UGameHUDWidget::SetTowerHealth(int32 Health)
{
    if (TowerHealthTextBlock1 && TowerHealthTextBlock2)
    {
        TowerHealthTextBlock1->SetText(FText::FromString(FString::Printf(TEXT("HP : %d"), Health)));
        TowerHealthTextBlock2->SetText(FText::FromString(FString::Printf(TEXT("HP : %d"), Health)));
    }
}

void UGameHUDWidget::UpdateBuildButtonsEnabled()
{
    if (AttackBuildingButton)
    {
        AttackBuildingButton->SetIsEnabled(CurrentGold >= AttackTowerCost);
    }

    if (SplashBuildingButton)
    {
        SplashBuildingButton->SetIsEnabled(CurrentGold >= SplashTowerCost);
    }

    if (CCBuildingButton)
    {
        CCBuildingButton->SetIsEnabled(CurrentGold >= CCTowerCost);
    }
}