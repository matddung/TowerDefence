#include "TowerMenuWidget.h"
#include "TowerBase.h"
#include "GamePlayGameMode.h"
#include "TooltipWidget.h"
#include "TDPlayerController.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTowerMenuWidget::Init(ATowerBase* InOwner)
{
    OwnerTower = InOwner;
    if (SellButton)
    {
        SellButton->OnClicked.AddDynamic(this, &UTowerMenuWidget::OnSellClicked);
    }
    if (UpgradeButton)
    {
        UpgradeButton->OnClicked.AddDynamic(this, &UTowerMenuWidget::OnUpgradeClicked);
    }
    if (TooltipWidgetClass)
    {
        SellTooltip = CreateWidget<UTooltipWidget>(this, TooltipWidgetClass);
        if (SellTooltip && SellButton)
        {
            SellButton->SetToolTip(SellTooltip);
        }

        UpgradeTooltip = CreateWidget<UTooltipWidget>(this, TooltipWidgetClass);
        if (UpgradeTooltip && UpgradeButton)
        {
            UpgradeButton->SetToolTip(UpgradeTooltip);
        }
    }

    UpdateInfo();
}

void UTowerMenuWidget::UpdateInfo()
{
    if (!OwnerTower)
    {
        return;
    }

    const int32 CurrentLevel = OwnerTower->GetCurrentLevel();
    const int32 NextCost = OwnerTower->GetBuildCost(CurrentLevel + 1);
    const int32 SellRefund = FMath::RoundToInt(OwnerTower->GetBuildCost(CurrentLevel) * 0.7f);

    if (CurrentLevelText)
    {
        FString LevelString;
        if (NextCost <= 0)
        {
            LevelString = TEXT("Current Level : Max");
        }
        else
        {
            LevelString = FString::Printf(TEXT("Current Level : %d"), CurrentLevel);
        }
        CurrentLevelText->SetText(FText::FromString(LevelString));
    }

    if (UpgradeButton)
    {
        UpgradeButton->SetIsEnabled(NextCost > 0);
    }

    if (SellTooltip)
    {
        FString SellString = FString::Printf(TEXT("Sell for: %d Gold"), SellRefund);
        SellTooltip->SetTooltipText(FText::FromString(SellString));
    }

    if (UpgradeTooltip)
    {
        FString UpgradeString;
        if (NextCost > 0)
        {
            UpgradeString = FString::Printf(TEXT("Upgrade Cost: %d Gold"), NextCost); 
        }
        else
        {
            UpgradeString = TEXT("Max Level");
        }
        UpgradeTooltip->SetTooltipText(FText::FromString(UpgradeString));
    }
}

void UTowerMenuWidget::OnSellClicked()
{
    if (OwnerTower)
    {
        ATDPlayerController* PC = Cast<ATDPlayerController>(OwnerTower->GetWorld()->GetFirstPlayerController());
        FVector FeedbackLoc = OwnerTower->GetActorLocation() + FVector(0.f, 0.f, 100.f);

        if (AGamePlayGameMode* GM = OwnerTower->GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
        {
            if (GM->IsWaveInProgress())
            {
                UE_LOG(LogTemp, Warning, TEXT("Cannot sell tower during wave"));
                if (PC)
                {
                    PC->SpawnFeedbackText(FText::FromString(TEXT("Cannot Sell")), FeedbackLoc);
                }
                return;
            }
        }

        OwnerTower->SellTower();

        if (PC)
        {
            PC->SpawnFeedbackText(FText::FromString(TEXT("Tower Sold")), FeedbackLoc);
        }
    }
}

void UTowerMenuWidget::OnUpgradeClicked()
{
    if (OwnerTower)
    {
        ATDPlayerController* PC = Cast<ATDPlayerController>(OwnerTower->GetWorld()->GetFirstPlayerController());
        FVector FeedbackLoc = OwnerTower->GetActorLocation() + FVector(0.f, 0.f, 100.f);
        int32 PrevLevel = OwnerTower->GetCurrentLevel();

        if (AGamePlayGameMode* GM = OwnerTower->GetWorld()->GetAuthGameMode<AGamePlayGameMode>())
        {
            if (GM->IsWaveInProgress())
            {
                UE_LOG(LogTemp, Warning, TEXT("Cannot upgrade tower during wave"));
                if (PC)
                {
                    PC->SpawnFeedbackText(FText::FromString(TEXT("Cannot Upgrade")), FeedbackLoc);
                }
                return;
            }
        }

        OwnerTower->UpgradeTower();

        bool bUpgraded = OwnerTower->GetCurrentLevel() > PrevLevel;
        if (PC)
        {
            PC->SpawnFeedbackText(bUpgraded ? FText::FromString(TEXT("Tower Upgraded")) : FText::FromString(TEXT("Cannot Upgrade")), FeedbackLoc);
        }
    }
}