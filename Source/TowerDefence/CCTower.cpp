#include "CCTower.h"

ACCTower::ACCTower()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Game/Data/TowerData/CCTowerData"));
    if (DataTableObj.Succeeded())
    {
        TowerDataTable = DataTableObj.Object;
    }
}

void ACCTower::BeginPlay()
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