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
}