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
        FName RowName = FName(*FString::FromInt(CurrentLevel));
        const FAttackTowerData* Data = TowerDataTable->FindRow<FAttackTowerData>(RowName, TEXT("TowerData Lookup"));
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

int32 AAttackTower::GetBuildCost(int32 Level) const
{
    if (Level < 0)
    {
        Level = CurrentLevel;
    }

    if (TowerDataTable)
    {
        return GetRowCost<FAttackTowerData>(TowerDataTable, Level);
    }

    return 0;
}

void AAttackTower::ReloadData()
{
    if (TowerDataTable)
    {
        FName RowName = FName(*FString::FromInt(CurrentLevel));
        const FAttackTowerData* Data = TowerDataTable->FindRow<FAttackTowerData>(RowName, TEXT("ReloadData"));
        if (Data)
        {
            TowerData = *Data;
            TargetingRange = TowerData.AttackRange;
            AttackInterval = TowerData.AttackInterval;
            Damage = TowerData.Damage;

            GetWorldTimerManager().ClearTimer(AttackTimerHandle);
            if (AttackInterval > 0.f)
            {
                GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATowerBase::Attack, AttackInterval, true);
            }
        }
    }
}