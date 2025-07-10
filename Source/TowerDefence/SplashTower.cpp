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
            AttackInterval = TowerData.AttackInterval;
            Damage = TowerData.Damage;

            if (AttackInterval > 0.f)
            {
                GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATowerBase::Attack, AttackInterval, true);
            }
        }
    }
}

int32 ASplashTower::GetBuildCost(int32 Level) const
{
    if (Level < 0)
    {
        Level = CurrentLevel;
    }

    if (TowerDataTable)
    {
        FName Row = FName(*FString::FromInt(Level));
        const FSplashTowerData* Data = TowerDataTable->FindRow<FSplashTowerData>(Row, TEXT("GetBuildCost"));
        if (Data)
        {
            return Data->BuildCost;
        }
    }

    return 0;
}

void ASplashTower::ReloadData()
{
    if (TowerDataTable)
    {
        TowerRowName = FName(*FString::FromInt(CurrentLevel));
        const FSplashTowerData* Data = TowerDataTable->FindRow<FSplashTowerData>(TowerRowName, TEXT("ReloadData"));
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