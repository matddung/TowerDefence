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
    if (TowerDataTable)
    {
        const FTowerData* Data = TowerDataTable->FindRow<FTowerData>(TowerRowName, TEXT("TowerData Lookup"));
        if (Data)
        {
            TowerData = *Data;
            TargetingRange = TowerData.Range;
        }
    }

    Super::BeginPlay();
}