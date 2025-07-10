#include "CCTower.h"
#include "Enemy.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

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
                GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ACCTower::Attack, AttackInterval, true);
            }
        }
    }
}

void ACCTower::FindTargetEnemy()
{
    TArray<AActor*> FoundEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

    AEnemy* ClosestUnslowed = nullptr;
    float ClosestUnslowedDistSq = TargetingRange * TargetingRange;
    AEnemy* ClosestSlowed = nullptr;
    float ClosestSlowedDistSq = TargetingRange * TargetingRange;

    for (AActor* Actor : FoundEnemies)
    {
        AEnemy* Enemy = Cast<AEnemy>(Actor);
        if (!Enemy)
            continue;

        float DistSq = FVector::DistSquared(Actor->GetActorLocation(), GetActorLocation());
        if (DistSq > TargetingRange * TargetingRange)
            continue;

        if (!Enemy->bIsSlowed)
        {
            if (DistSq < ClosestUnslowedDistSq)
            {
                ClosestUnslowed = Enemy;
                ClosestUnslowedDistSq = DistSq;
            }
        }
        else
        {
            if (DistSq < ClosestSlowedDistSq)
            {
                ClosestSlowed = Enemy;
                ClosestSlowedDistSq = DistSq;
            }
        }
    }

    CurrentTarget = ClosestUnslowed ? ClosestUnslowed : ClosestSlowed;
}

void ACCTower::Attack()
{
    if (!CurrentTarget) return;

    if (AttackEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackEffect, CurrentTarget->GetActorLocation());
    }

    CurrentTarget->ApplySlow(TowerData.SlowlyPercent);
}

int32 ACCTower::GetBuildCost(int32 Level) const
{
    if (Level < 0)
    {
        Level = CurrentLevel;
    }

    if (TowerDataTable)
    {
        FName Row = FName(*FString::FromInt(Level));
        const FCCTowerData* Data = TowerDataTable->FindRow<FCCTowerData>(Row, TEXT("GetBuildCost"));
        if (Data)
        {
            return Data->BuildCost;
        }
    }

    return 0;
}

void ACCTower::ReloadData()
{
    if (TowerDataTable)
    {
        TowerRowName = FName(*FString::FromInt(CurrentLevel));
        const FCCTowerData* Data = TowerDataTable->FindRow<FCCTowerData>(TowerRowName, TEXT("ReloadData"));
        if (Data)
        {
            TowerData = *Data;
            TargetingRange = TowerData.AttackRange;
            AttackInterval = TowerData.AttackInterval;

            GetWorldTimerManager().ClearTimer(AttackTimerHandle);
            if (AttackInterval > 0.f)
            {
                GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ACCTower::Attack, AttackInterval, true);
            }
        }
    }
}