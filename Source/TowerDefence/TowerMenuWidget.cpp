#include "TowerMenuWidget.h"
#include "TowerBase.h"

#include "Components/Button.h"

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