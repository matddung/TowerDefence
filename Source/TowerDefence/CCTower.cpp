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
    Super::BeginPlay();

    if (TowerDataTable)
    {
        const FCCTowerData* Data = TowerDataTable->FindRow<FCCTowerData>(TowerRowName, TEXT("TowerData Lookup"));
        if (Data)
        {
            TowerData = *Data;
            TargetingRange = TowerData.AttackRange;
            AttackInterval = TowerData.AttackInterval;

            if (AttackInterval > 0.f)
            {
                GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATowerBase::Attack, AttackInterval, true);
            }
        }
    }
}