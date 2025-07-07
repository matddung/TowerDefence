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
}