#include "TowerMenuWidget.h"
#include "TowerBase.h"

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
}

void UTowerMenuWidget::OnSellClicked()
{
    if (OwnerTower)
    {
        OwnerTower->SellTower();
    }
}

void UTowerMenuWidget::OnUpgradeClicked()
{
    if (OwnerTower)
    {
        OwnerTower->UpgradeTower();
    }
}