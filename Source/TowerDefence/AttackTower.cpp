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
    Super::BeginPlay();

    if (TowerDataTable)
    {
        const FAttackTowerData* Data = TowerDataTable->FindRow<FAttackTowerData>(TowerRowName, TEXT("TowerData Lookup"));
        if (Data)
        {
            TowerData = *Data;
            TargetingRange = TowerData.AttackRange;
            AttackInterval = TowerData.AttackInterval;
            Damage = TowerData.Damage;

            if (AttackInterval > 0.f)
            {
                GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATowerBase::Attack, AttackInterval, true);
            }
        }
    }
}