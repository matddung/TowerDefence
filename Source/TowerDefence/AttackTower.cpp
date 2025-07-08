#include "AttackTower.h"

AAttackTower::AAttackTower()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Game/Data/TowerData/AttackTowerData"));
    if (DataTableObj.Succeeded())
    {
        TowerDataTable = DataTableObj.Object;
    }
}

void AAttackTower::BeginPlay()
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