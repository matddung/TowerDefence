#include "SplashTower.h"

ASplashTower::ASplashTower()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Game/Data/TowerData/SplashTowerData"));
    if (DataTableObj.Succeeded())
    {
        TowerDataTable = DataTableObj.Object;
    }
}

void ASplashTower::BeginPlay()
{
    Super::BeginPlay();

    if (TowerDataTable)
    {
        const FSplashTowerData* Data = TowerDataTable->FindRow<FSplashTowerData>(TowerRowName, TEXT("TowerData Lookup"));
        if (Data)
        {
            TowerData = *Data;
            TargetingRange = TowerData.AttackRange;
        }
    }
}